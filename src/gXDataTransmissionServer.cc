// *************************************
// Ganymede Xpedia
// gXDTP (Data Transmission Protocol)
// 'gXDataTransmissionServer.cc'
// Author: jcjuarez
// *************************************

#include <thread>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include "gXDataTransmissionServer.hh"

namespace gX
{

DataTransmissionServerConfiguration::DataTransmissionServerConfiguration()
    : m_Port(c_DefaultPortNumber),
      m_ReceiveBufferSize(c_DefaultReceiveBufferSize),
      m_ThreadPoolSize(c_DefaultThreadPoolSize),
      m_MaxNumberAllowedConnections(c_DefaultMaxNumberAllowedConnections),
      m_BlockingExecution(c_DefaultBlockingExecution),
      m_CleanTermination(c_DefaultCleanTermination)
{
    //
    // Default function for the default DTP packet tag. Possible to override it (and recommended for production scenarios).
    //
    m_PacketTagResolverTable.emplace(DataTransmissionServer::c_DefaultEndpointPacketTag, &DataTransmissionServer::DefaultEndpoint);
}

DataTransmissionServer::DataTransmissionServer(
    const std::string& p_ServiceIdentifier)
    : m_IsInitialized(false),
      m_IsStopped(false),
      m_ServiceIdentifier(p_ServiceIdentifier)
{}

DataTransmissionServer::~DataTransmissionServer()
{
    //
    // Stop the server execution.
    //
    m_IsStopped = true;

    if (m_DispatchRequestsThreadHandle.joinable())
    {
        try
        {
            //
            // Wait for the server to complete its execution.
            //
            m_DispatchRequestsThreadHandle.join();
        }
        catch (const std::system_error& p_Exception)
        {
            //
            // This can happen in the rare situation in which the thread became non-joinable just
            // after checking its state. This should not be considered an error at this point; continue.
            //
        }
    }
}

StatusCode
DataTransmissionServer::Init(
    const DataTransmissionServerConfiguration* p_Configuration)
{

    if (m_IsInitialized)
    {
        return Status::AlreadyInitialized;
    }

    DataTransmissionServerConfiguration defaultConfiguration;

    if (p_Configuration == nullptr)
    {
        //
        // If no configurations are specified, use the default ones.
        //
        p_Configuration = &defaultConfiguration;
    }

    //
    // Set the execution models and the packet tag resolver table.
    //
    m_BlockingExecution = p_Configuration->m_BlockingExecution;
    m_CleanTermination = p_Configuration->m_CleanTermination;
    m_PacketTagResolverTable = std::move(p_Configuration->m_PacketTagResolverTable);

    //
    // Initialize the thread pool.
    //
    StatusCode status = m_ThreadPool.Init(p_Configuration->m_ThreadPoolSize);

    if (Status::Failed(status))
    {
        return status;
    }

    try
    {
        //
        // Allocate memory for the dynamic-size receive buffer.
        //
        m_ReceiveBufferSize = p_Configuration->m_ReceiveBufferSize;
        m_ReceiveBuffer = std::unique_ptr<Byte[]>(new Byte[m_ReceiveBufferSize]);
    }
    catch (const std::bad_alloc& e)
    {
        return Status::OutOfMemory;
    }

    //
    // Create socket handle for handling incoming requests.
    //
    if ((m_ServerSocketHandle = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        return Status::SocketCreationFailed;
    }

    //
    // Configure socket handle.
    //
    int32_t opt = 1;

    if (setsockopt(m_ServerSocketHandle, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        close(m_ServerSocketHandle);
        
        return Status::SocketConfigurationFailed;
    }
    
    //
    // Set up socket address configurations.
    //
    m_Address.sin_family = AF_INET;
    m_Address.sin_addr.s_addr = INADDR_ANY;
    m_Address.sin_port = htons(p_Configuration->m_Port);
    m_AddressLength = sizeof(m_Address);

    //
    // Bind the socket handle to the socket address information.
    //
    if (bind(m_ServerSocketHandle, reinterpret_cast<const sockaddr*>(&m_Address), sizeof(m_Address)) < 0)
    {
        close(m_ServerSocketHandle);

        return Status::SocketBindFailed;
    }

    //
    // Start listening for connections on the specified port.
    //
    if (listen(m_ServerSocketHandle, p_Configuration->m_MaxNumberAllowedConnections) < 0)
    {
        close(m_ServerSocketHandle);

        return Status::SocketListenFailed;
    }
    
    //
    // Set the fact that the server has been correctly initialized.
    //
    m_IsInitialized = true;

    return Status::Success;
}

void
DataTransmissionServer::Run()
{
    if (!m_IsInitialized)
    {
        return;
    }

    if (m_IsStopped)
    {
        return;
    }

    //
    // Spawn a new background thread for running the server.
    //
    m_DispatchRequestsThreadHandle = std::thread(&DataTransmissionServer::DispatchRequests, this);

    if (m_BlockingExecution)
    {
        //
        // Blocking execution was specified; use the same caller thread for running the server.
        //
        m_DispatchRequestsThreadHandle.join();
    }
}

StatusCode
DataTransmissionServer::Stop()
{
    if (m_IsStopped)
    {
        return Status::ServiceIsStopped;
    }

    m_IsStopped = true;

    return Status::Success;
}

StatusCode
DataTransmissionServer::DefaultEndpoint(
    std::string p_Packet)
{
    std::cout << p_Packet.c_str() << std::endl;

    return Status::Success;
}

void
DataTransmissionServer::DispatchRequests()
{
    FOREVER
    {   
        if (m_IsStopped)
        {
            //
            // The server has been instructed to be terminated; stop its execution.
            //
            break;
        }

        FileDescriptor connection;

        //
        // Accept an incoming connection.
        //
        if ((connection = accept(m_ServerSocketHandle, (struct sockaddr *)&m_Address, (socklen_t *)&m_AddressLength)) < 0)
        {
            //
            // Invalid connection; continue.
            //
            continue;
        }

        //
        // Read data from the established connection.
        //
        uint32_t numberBytesRead = read(connection, m_ReceiveBuffer.get(), m_ReceiveBufferSize);

        if (numberBytesRead <= 0)
        {
            //
            // Invalid reading; continue.
            //
            continue;
        }
        
        //
        // Deserialize message.
        //
        std::string packet(reinterpret_cast<char*>(m_ReceiveBuffer.get()), numberBytesRead);
        const PacketTag packetTag = c_DefaultEndpointPacketTag;

        //
        // Resolve function to execute based on the lookup table.
        //
        if (m_PacketTagResolverTable.find(packetTag) == m_PacketTagResolverTable.end())
        {
            //
            // Unknown packet tag.
            // Do not enqueue the request and send the response back immediately.
            //
            SendResponseAndCloseConnection(Status::UnknownPacketTag, connection);

            continue;
        }
        
        const EndpointType boundFunction = m_PacketTagResolverTable.at(packetTag);

        //
        // Enqueue task for async execution and increase the number of requests in execution.
        //
        auto enqueueResult = m_ThreadPool.EnqueueTask(
            &DataTransmissionServer::DispatcherProxy,
            this,
            boundFunction,
            connection,
            packet);

        if (enqueueResult != std::nullopt)
        {
            //
            // It is better to increment the counter here as it guarantees change decoupled from the new thread execution.
            //
            ++m_NumberRequestsInExecution;
        }
    }

    if (m_CleanTermination)
    {
        //
        // Clean termination was specified; wait for all tasks to finish.
        // At this point it is guaranteed that no more tasks will be enqueued.
        //
        while (m_NumberRequestsInExecution != 0)
        {}
    }

    //
    // Close the server socket after stopping execution.
    // If clean termination was not specified, it is possible that the TCP socket is
    // closed before sending responses back to requests that had already been acknowledged.
    //
    close(m_ServerSocketHandle);
}

void
DataTransmissionServer::DispatcherProxy(
    DataTransmissionServer* p_DataTransmissionServer,
    const EndpointType p_Endpoint,
    const FileDescriptor p_Connection,
    std::string p_Packet)
{
    //
    // Execute endpoint in an async context.
    //
    SendResponseAndCloseConnection(p_Endpoint(p_Packet), p_Connection);

    //
    // Decrement the counter once the response has been sent back to the client.
    //
    --(p_DataTransmissionServer->m_NumberRequestsInExecution);
}

void
DataTransmissionServer::SendResponseAndCloseConnection(
    const StatusCode p_Status,
    const FileDescriptor p_Connection)
{
    StatusCode standardizedStatusCode = htonl(p_Status);

    //
    // Send the response back to the client and close the connection.
    // This expects that the server socket handle is still open and active.
    //
    send(p_Connection, &standardizedStatusCode, sizeof(standardizedStatusCode), 0);
    close(p_Connection);
}

} // namespace gX.