// *************************************
// Ganymede Xpedia
// gXDTP (Data Transmission Protocol)
// 'gXDataTransmissionServer.cc'
// Author: jcjuarez
// *************************************

#include <thread>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "gXDataTransmissionServer.hh"
#include <iostream> // delete;

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
    // Default function for the default DTP packet tag. Possible to override it.
    //
    m_TagResolverTable.emplace(DataTransmissionServer::c_DefaultEndpointPacketTag, &DataTransmissionServer::DefaultEndpoint);
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
    // Set the execution models.
    //
    m_BlockingExecution = p_Configuration->m_BlockingExecution;
    m_CleanTermination = p_Configuration->m_CleanTermination;

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
    // This should be a proxy that should execute the speicified function (endpoint),
    // and once it returns it closes the connection.


    std::cout << "Message from client: " << p_Packet.c_str() << std::endl;
    std::thread::id this_id = std::this_thread::get_id();
    std::cout << "Thread function running on thread ID: " << this_id << std::endl;

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
        
        std::string str(reinterpret_cast<char*>(m_ReceiveBuffer.get()), numberBytesRead);

        auto boundFunction = std::bind(&DataTransmissionServer::DefaultEndpoint, std::placeholders::_1);


        std::function<StatusCode(std::string)> func = std::function<StatusCode(std::string)>(boundFunction);




        
        m_ThreadPool.EnqueueTask(
            &DataTransmissionServer::DispatcherProxy,
            func,
            connection,
            str);
    }

    if (m_CleanTermination)
    {
        //
        // Clean termination was specified; wait for all tasks to finish.
        // At this point it is guaranteed that no more tasks will be enqueued.
        //
        while (m_ThreadPool.GetNumberTasksInExecution() != 0)
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
    std::function<StatusCode(std::string)> p_Endpoint,
    const FileDescriptor p_Connection,
    std::string p_Packet)
{
    //
    // Execute endpoint in an async context and standardize its result.
    //
    StatusCode status = htonl(p_Endpoint(p_Packet));

    //
    // Send the response back to the client and close the connection.
    // This expects that the server socket handle is still open and active.
    //
    send(p_Connection, &status, sizeof(status), 0);
    close(p_Connection);
}

} // namespace gX.