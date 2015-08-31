#if !defined(_IZANAGI_NETWORK_NETWORK_DEFS_H__)
#define _IZANAGI_NETWORK_NETWORK_DEFS_H__

#include "izDefs.h"

namespace izanagi {
namespace net {
#ifdef WINDOWS
    using IZ_SOCKET = SOCKET;

    static const IZ_SOCKET IZ_INVALID_SOCKET = INVALID_SOCKET;

    inline IZ_BOOL isValidSocket(IZ_SOCKET socket)
    {
        return (socket != INVALID_SOCKET);
    }

    inline IZ_INT getLastError()
    {
        auto ret = WSAGetLastError();
        return ret;
    }

    /** @internal
     * ソケットを閉じる.
     */
    inline void closeSocket(IZ_SOCKET sock)
    {
        closesocket(sock);
    }

    /** @internal
     * sockaddr_in 内の in_addr にアクセスする用.
     */
    inline IZ_UINT getIp(const sockaddr_in& addr)
    {
        return addr.sin_addr.S_un.S_addr;
}

    /** @internal
     * sockaddr_in 内の in_addr にアクセスする用.
     */
    inline void setIp(sockaddr_in& addr, IZ_UINT ip)
    {
        addr.sin_addr.S_un.S_addr = ip;
    }
#else
    using IZ_SOCKET = IZ_INT;

    static const IZ_SOCKET IZ_INVALID_SOCKET = -1;

    inline IZ_BOOL isValidSocket(IZ_SOCKET socket)
    {
        return (socket >= 0);
    }

    inline IZ_INT getLastError()
    {
        return errno;
    }

    /** @internal
     * ソケットを閉じる.
     */
    inline void closeSocket(IZ_SOCKET sock)
    {
        close(sock);
    }

    /** @internal
     * sockaddr_in 内の in_addr にアクセスする用.
     */
    inline IZ_UINT getIp(const sockaddr_in& addr)
    {
        return addr.sin_addr.s_addr;
    }

    /** @internal
     * sockaddr_in 内の in_addr にアクセスする用.
     */
    inline void setIp(sockaddr_in& addr, IZ_UINT ip)
    {
        addr.sin_addr.s_addr = ip;
    }
#endif

}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_DEFS_H__)
