// *************************************
// Ganymede Xpedia
// Common
// 'gXStatus.hh'
// Author: jcjuarez
// *************************************

#ifndef GX_STATUS_
#define GX_STATUS_

#include <cstdint>

namespace gX
{

//
// Byte type alias.
//
using Byte = uint8_t;

//
// File descriptpr type alias.
//
using FileDescriptor = int32_t;

//
// Status code type alias.
//
using StatusCode = uint32_t;

//
// Status code definition macro.
//
#define STATUS_CODE_DEFINITION(p_StatusCodeName, p_StatusCodeValue) static constexpr StatusCode p_StatusCodeName = p_StatusCodeValue

//
// Infinite repetition macro.
//
#define FOREVER while (true)

//
// Status class for indicating errors across the system.
//
class Status
{

    //
    // Static class.
    //
    Status() = delete;

public:

    //
    // Determines whether a given status is considered as failure.
    //
    inline static
    bool
    Failed(
        const StatusCode p_StatusCode)
    {
        return static_cast<int32_t>(p_StatusCode) < 0;
    }

    //
    // Determines whether a given status is considered as success.
    //
    inline static
    bool
    Succeeded(
        const StatusCode& p_StatusCode)
    {
        return p_StatusCode == Success;
    }

    //
    // Operation succeeded.
    //
    STATUS_CODE_DEFINITION(Success, 0x0'0000000);

    //
    // Generic operation failed.
    //
    STATUS_CODE_DEFINITION(Fail, 0x8'0000000);

    //
    // Component not yet initialized.
    //
    STATUS_CODE_DEFINITION(NotInitialized, 0x8'0000001);

    //
    // Component already initialized.
    //
    STATUS_CODE_DEFINITION(AlreadyInitialized, 0x8'0000002);

    //
    // Failure to launch a new thread.
    //
    STATUS_CODE_DEFINITION(ThreadLaunchFailed, 0x8'0000003);

    //
    // System is out of memory for new allocations.
    //
    STATUS_CODE_DEFINITION(OutOfMemory, 0x8'0000004);

    //
    // Socket creation failed.
    //
    STATUS_CODE_DEFINITION(SocketCreationFailed, 0x8'0000005);

    //
    // Socket configuration failed.
    //
    STATUS_CODE_DEFINITION(SocketConfigurationFailed, 0x8'0000006);

    //
    // Socket bind failed.
    //
    STATUS_CODE_DEFINITION(SocketBindFailed, 0x8'0000007);

    //
    // Socket listen process failed.
    //
    STATUS_CODE_DEFINITION(SocketListenFailed, 0x8'0000008);

    //
    // Service has been already stopped.
    //
    STATUS_CODE_DEFINITION(ServiceIsStopped, 0x8'0000009);

};

} // namespace gX.

#endif