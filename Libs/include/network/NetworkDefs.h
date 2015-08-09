#if !defined(_IZANAGI_NETWORK_NETWORK_DEFS_H__)
#define _IZANAGI_NETWORK_NETWORK_DEFS_H__

#include "izDefs.h"

namespace izanagi {
namespace net {

#ifdef WINDOWS
    #include <WinSock2.h>

    typedef SOCKET IZ_SOCKET;

    #define IZ_INVALID_SOCKET    INVALID_SOCKET

    inline IZ_BOOL isValidSocket(IZ_SOCKET socket)
    {
        return (socket != INVALID_SOCKET);
    }
#else
    typedef IZ_INT IZ_SOCKET;

    #define IZ_INVALID_SOCKET    -1

    inline IZ_BOOL isValidSocket(IZ_SOCKET socket)
    {
        return (socket >= 0);
    }
#endif

}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_DEFS_H__)
