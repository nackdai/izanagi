#include "network/NetworkTCPUtility.h"
#include "network/NetworkRemote.h"

namespace izanagi {
namespace net {
    void TcpProxy::run(
        std::function<void(const Packet&)> onRecv,
        IZ_UINT sec/*= 0*/,
        IZ_UINT usec/*= 0*/)
    {
        timeval t_val = { sec, usec };

        timeval* pTval = nullptr;

        if (sec > 0 || usec > 0) {
            pTval = &t_val;
        }

        fd_set readFD;
        fd_set exceptionFD;

        FD_ZERO(&readFD);
        FD_ZERO(&exceptionFD);

        FD_SET(m_socket, &readFD);
        FD_SET(m_socket, &exceptionFD);

        {
            std::unique_lock<std::mutex> lock(m_remotesLocker);

            for (IZ_UINT i = 0; i < m_remotes.getNum(); i++) {
                TcpRemote& remote = m_remotes.at(i);

                sys::Lock lock(remote);

                if (remote.isActive()) {
                    FD_SET(remote.m_socket, &readFD);
                    FD_SET(remote.m_socket, &exceptionFD);
                }
            }
        }

        // ファイルディスクリプタ（ソケット）の状態遷移待ち
        auto resSelect = select(
            //FD_SETSIZE,
            0,
            &readFD,
            IZ_NULL,
            &exceptionFD,
            pTval);

        if (resSelect <= 0) {
            return;
        }

        if (FD_ISSET(m_socket, &readFD)) {
            sockaddr_in addr;
            sockaddr* paddr = (sockaddr*)&addr;
            IZ_INT len = sizeof(addr);

            // クライアントからの接続待ち
            auto socket = accept(m_socket, paddr, &len);

            if (!isValidSocket(socket)) {
                return;
            }

            {
                std::unique_lock<std::mutex> lock(m_remotesLocker);

                for (IZ_UINT i = 0; i < m_remotes.getNum(); i++) {
                    TcpRemote& remote = m_remotes.at(i);

                    sys::Lock lock(remote);

                    // 空いているものを探す
                    if (!remote.isActive()) {
                        remote.setSocket(socket);
                        remote.m_endpoint.set(addr);
                        break;
                    }
                }
            }
        }
        else if (FD_ISSET(m_socket, &exceptionFD)) {
            // TODO
        }

        {
            std::unique_lock<std::mutex> lock(m_remotesLocker);

            for (IZ_UINT i = 0; i < m_remotes.getNum(); i++) {
                TcpRemote& remote = m_remotes.at(i);

                sys::Lock lock(remote);

                if (FD_ISSET(remote.m_socket, &exceptionFD)) {
                    // TODO
                }
                else {
                    // 受信
                    if (FD_ISSET(remote.m_socket, &readFD)) {
                        auto len = recv(remote.m_socket, (char*)m_recvBuf, m_recvBufSize, 0);

                        if (len > 0) {
                            if (onRecv) {
                                Packet packet;

                                packet.endpoint = remote.m_endpoint;
                                packet.data = m_recvBuf;
                                packet.size = m_recvBufSize;

                                if (onRecv) {
                                    onRecv(packet);
                                }
                            }
                        }
                        else {
                            // TODO
                            // 切断された
                        }
                    }
                }
            }
        }
    }

    void TcpProxy::loop(std::function<void(const Packet&)> onRecv)
    {
        for (;;) {
            if (!m_isRunnning) {
                break;
            }

            // TODO
            run(onRecv, 0, 1000);
        }
    }

    ////////////////////////////////////////////////////

    // サーバーとして起動.
    IZ_BOOL TcpServer::start(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& endpoint,
        IZ_UINT maxConnections)
    {
        IZ_BOOL result = Tcp::startAsServer(allocator, endpoint, maxConnections);
        IZ_ASSERT(result);

        if (result) {
            result = m_thread.Start(
                [this] (void* data) {
                    loop();
                }, nullptr);

            IZ_ASSERT(result);
        }

        if (!result) {
            stop();
        }

        return result;
    }

    void TcpServer::onStop()
    {
        m_thread.Join();
    }

    // クライアントとして起動.
    IZ_BOOL TcpClient::start(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& endpoint,
        IZ_BOOL isBlock)
    {
        IZ_BOOL result = Tcp::startAsClient(allocator, endpoint);

        if (result) {
            if (isBlock) {
                result = connectServer();
            }
            else {
                result = m_thread.Start(
                    [this] (void* data) {
                        connectServer();
                    }, nullptr);
                VGOTO(result, __EXIT__);

                result = m_threadSub.Start(
                    [this] (void* data) {
                        loop();
                    }, nullptr);
            }
        }

    __EXIT__:
        if (!result) {
            stop();
        }

        return result;
    }

    void TcpClient::onStop()
    {
        m_thread.Join();
        m_threadSub.Join();
    }
}    // namespace net
}    // namespace izanagi
