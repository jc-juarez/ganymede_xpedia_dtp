// *************************************
// Ganymede Xpedia
// gXDTP (Data Transmission Protocol)
// 'gXDataTransmissionServer.hh'
// Author: jcjuarez
// *************************************

#ifndef GX_DATA_TRANSMISSION_SERVER_
#define GX_DATA_TRANSMISSION_SERVER_

#include <string>
#include <memory>
#include <atomic>
#include <cstdint>
#include <functional>
#include "gXStatus.hh"
#include <netinet/in.h>
#include <unordered_map>
#include "gXThreadPool.hh"

namespace gX
{

using DtpEndpointType = std::function<StatusCode(std::string)>;

//
// Configurations for the DTP server.
//
struct DataTransmissionServerConfiguration
{
    
    //
    // Constructor.
    //
    DataTransmissionServerConfiguration();

    //
    // Port for the DTP server.
    //
    uint32_t m_Port;

    //
    // Receive buffer size for the DTP server.
    //
    uint32_t m_ReceiveBufferSize;

    //
    // Number of threads in the thread pool for the DTP server.
    //
    uint16_t m_ThreadPoolSize;

    //
    // Maximum number of TCP connections allowed on the internal queue.
    //
    uint16_t m_MaxNumberAllowedConnections;

    //
    // Flag for selecting blocking or non-blocking execution.
    //
    bool m_BlockingExecution;

    //
    // Flag for selecting clean or abrupt termination of the server.
    // Ensures that the internal thread pool waits for all tasks to finish upon object destruction.
    //
    bool m_CleanTermination;

    //
    // DTP tag to function map. Maps a tag to the appropriate function binding to be executed.
    // Establishes the signature needed to be used by all functions using the DTP protocol <StatusCode F(DataTransmissionPacket)>.
    //
    std::unordered_map<uint32_t, DtpEndpointType> m_TagResolverTable;

    //
    // Default port.
    //
    static constexpr uint32_t c_DefaultPortNumber = 9090u;

    //
    // Default receive buffer size.
    //
    static constexpr uint32_t c_DefaultReceiveBufferSize = 4096u;

    //
    // Default thread pool size.
    //
    static constexpr uint16_t c_DefaultThreadPoolSize = 20u;

    //
    // Default maximum number of allowed connections.
    //
    static constexpr uint16_t c_DefaultMaxNumberAllowedConnections = 10u;

    //
    // Default blocking execution model.
    //
    static constexpr bool c_DefaultBlockingExecution = true;

    //
    // Default clean termination model.
    //
    static constexpr bool c_DefaultCleanTermination = true;

};

//
// High performace TCP-based data transmission protocol server.
//
class DataTransmissionServer
{

public:

    //
    // Constructor.
    //
    DataTransmissionServer(
        const std::string& p_ServiceIdentifier);

    //
    // Destructor.
    //
    ~DataTransmissionServer();

    //
    // Initializes the main server component.
    //
    StatusCode
    Init(
        const DataTransmissionServerConfiguration* p_Configuration = nullptr);

    //
    // Starts the server for handling incoming requests.
    //
    void
    Run();

    //
    // Manually stops the server to stop receiving more requests.
    // Should be used cautiously as it brings the server into an unusable state.
    // It is also not advisable to call Stop when not using the clean termination
    // model as it can lead to undefined behavior depending on the current execution state.
    //
    StatusCode
    Stop();

    //
    // Default server endpoint. Specifies the required signature for all endpoints.
    // Only used for debugging purposes.
    //
    static
    StatusCode
    DefaultEndpoint(
        std::string p_Packet);

    //
    // Default DTP packet tag for the default endpoint.
    //
    static constexpr uint32_t c_DefaultEndpointPacketTag = 0u;

private:

    //
    // Dispatches the incoming TCP requests.
    // This can be executed on a sync or async context depending on the server configuration.
    //
    void
    DispatchRequests();

    //
    // Dispatcher proxy. Executes the specified function and then closes the connection.
    //
    static
    void
    DispatcherProxy(
        const std::function<StatusCode(std::string)> p_Endpoint,
        const FileDescriptor p_Connection,
        std::string p_Packet);

    //
    // Handle for the DispatchRequests method execution.
    //
    std::thread m_DispatchRequestsThreadHandle;

    //
    // Determines if the server has already been initialized.
    //
    std::atomic<bool> m_IsInitialized;

    //
    // Determines if the server has been stopped.
    //
    std::atomic<bool> m_IsStopped;

    //
    // Service identifier for the server.
    //
    const std::string m_ServiceIdentifier;

    //
    // Server internal socket handle.
    //
    FileDescriptor m_ServerSocketHandle;

    //
    // Holds the internal server socket address information.
    //
    sockaddr_in m_Address;

    //
    // Size of the scoket address information field.
    //
    uint32_t m_AddressLength;

    //
    // Internal data buffer for handling incoming TCP data flushes.
    //
    std::unique_ptr<Byte[]> m_ReceiveBuffer;

    //
    // Size of the receive buffer.
    //
    uint32_t m_ReceiveBufferSize;

    //
    // Blocking execution model.
    //
    bool m_BlockingExecution;

    //
    // Clean termination model.
    //
    bool m_CleanTermination;

    //
    // Thread pool for handling concurrent requests.
    //
    ThreadPool m_ThreadPool;

};

} // namespace gX.

#endif