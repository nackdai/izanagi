#include "network/NetworkTCP.h"
#include "network/NetworkRemote.h"
#include "network/IPv4Endpoint.h"

// NOTE
// http://www.jenkinssoftware.com/raknet/manual/systemoverview.html

namespace izanagi {
namespace net {
    Tcp::Tcp()
    {
        m_allocator = nullptr;

        m_socket = IZ_INVALID_SOCKET;

        m_isRunnning.store(IZ_FALSE);
        m_isConnected.store(IZ_FALSE);
    }

    Tcp::~Tcp()
    {
        stop();
    }

    // �T�[�o�[�Ƃ��ċN��.
    IZ_BOOL Tcp::startAsServer(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& hostEp,
        IZ_UINT maxConnections)
    {
        VRETURN(maxConnections > 0);

        if (m_isRunnning) {
            return IZ_TRUE;
        }

        m_allocator = allocator;

        IZ_BOOL result = IZ_FALSE;

        // �\�P�b�g�̐���
        m_socket = socket(
            AF_INET,        // �A�h���X�t�@�~��
            SOCK_STREAM,    // �\�P�b�g�^�C�v
            0);             // �v���g�R��
        VRETURN(isValidSocket(m_socket));

        // �ʐM�|�[�g�E�A�h���X�̐ݒ�
        sockaddr_in inAddr;
        {
            FILL_ZERO(&inAddr, sizeof(inAddr));

            inAddr.sin_family = AF_INET;
            inAddr.sin_port = htons(hostEp.getPort());

            auto address = hostEp.getAddress();

            if (address.isAny()) {
                inAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
            }
            else {
                IZ_CHAR ip[64];
                address.toString(ip, COUNTOF(ip));

                inAddr.sin_addr.S_un.S_addr = inet_addr(ip);
            }
        }

        // �\�P�b�g�ɃA�h���X�����т���
        result = (bind(m_socket, (const sockaddr*)&inAddr, sizeof(inAddr)) >= 0);
        VGOTO(result, __EXIT__);

        // �R�l�N�g�v���������܂ő҂���ݒ�
        result = (listen(m_socket, maxConnections) >= 0);
        VGOTO(result, __EXIT__);

        result = m_remotes.init(m_allocator, maxConnections);
        VGOTO(result, __EXIT__);

        m_isRunnning.store(IZ_TRUE);

    __EXIT__:
        if (!result) {
            IZ_ASSERT(IZ_FALSE);

            stop();
        }

        return result;
    }

    // �N���C�A���g�Ƃ��ċN��.
    IZ_BOOL Tcp::startAsClient(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& hostEp)
    {
        if (m_isRunnning.load()) {
            return IZ_TRUE;
        }

        m_allocator = allocator;

        IZ_BOOL result = IZ_FALSE;

        // �\�P�b�g�̐���
        m_socket = socket(
            AF_INET,        // �A�h���X�t�@�~��
            SOCK_STREAM,    // �\�P�b�g�^�C�v
            0);             // �v���g�R��
        VRETURN(isValidSocket(m_socket));

        // �ʐM�|�[�g�E�A�h���X�̐ݒ�
        sockaddr_in inAddr;
        {
            FILL_ZERO(&inAddr, sizeof(inAddr));

            inAddr.sin_family = AF_INET;
            inAddr.sin_port = htons(hostEp.getPort());

            auto address = hostEp.getAddress();
            result = !address.isAny();

            VGOTO(result, __EXIT__);

            IZ_CHAR ip[64];
            address.toString(ip, COUNTOF(ip));

            inAddr.sin_addr.S_un.S_addr = inet_addr(ip);
        }

        // �\�P�b�g�ɃA�h���X�����т���
        result = (bind(m_socket, (const sockaddr*)&inAddr, sizeof(inAddr)) >= 0);
        VGOTO(result, __EXIT__);

        m_isRunnning.store(IZ_TRUE);

    __EXIT__:
        if (!result) {
            IZ_ASSERT(IZ_FALSE);

            stop();
        }
        return result;
    }

    // �T�[�o�[�Ɛڑ�.
    IZ_BOOL Tcp::connectToServer(const IPv4Endpoint& remoteEp)
    {
        VRETURN(m_isRunnning);

        IZ_BOOL result = IZ_TRUE;

        // �ʐM�|�[�g�E�A�h���X�̐ݒ�
        sockaddr_in serverAddr;
        {
            FILL_ZERO(&serverAddr, sizeof(serverAddr));

            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(remoteEp.getPort());

            auto address = remoteEp.getAddress();
            result = !address.isAny();

            VGOTO(result, __EXIT__);

            IZ_CHAR ip[64];
            address.toString(ip, COUNTOF(ip));

            serverAddr.sin_addr.S_un.S_addr = inet_addr(ip);
        }

        result = (connect(
            m_socket,
            (sockaddr*)&serverAddr,
            sizeof(serverAddr)) >= 0);

        if (result) {
            TcpRemote& remote = m_remotes.at(0);

            sys::Lock lock(remote);
            remote.setSocket(m_socket);

            m_isConnected.store(IZ_TRUE);
        }
        else {
            // TODO
            IZ_ASSERT(IZ_FALSE);
        }

    __EXIT__:
        return result;
    }

    // ��~.
    void Tcp::stop()
    {
        m_isRunnning.store(IZ_FALSE);
        m_isConnected.store(IZ_FALSE);

        onStop();

        if (isValidSocket(m_socket)) {
            shutdown(m_socket, SD_BOTH);
            closesocket(m_socket);
            m_socket = IZ_INVALID_SOCKET;
        }

        m_remotes.traverse(
            [this] (TcpRemote& remote) {
            remote.close();
        });

        m_remotes.clear();
    }

    IZ_UINT Tcp::getRemoteNum()
    {
        std::unique_lock<std::mutex> lock(m_remotesLocker);

        IZ_UINT ret = m_remotes.getNum();
        return ret;
    }

    const IPv4Endpoint* Tcp::getRemote(IZ_UINT idx)
    {
        std::unique_lock<std::mutex> lock(m_remotesLocker);

        const TcpRemote& remote = m_remotes.at(idx);
        
        if (!remote.isActive()) {
            return IZ_NULL;
        }

        return &remote.m_endpoint;
    }

    const IPv4Endpoint* Tcp::acceptRemote()
    {
        sockaddr_in addr;
        sockaddr* paddr = (sockaddr*)&addr;
        IZ_INT len = sizeof(addr);

        // �N���C�A���g����̐ڑ��҂�
        auto socket = accept(m_socket, paddr, &len);

        if (!isValidSocket(socket)) {
            return nullptr;
        }

        for (IZ_UINT i = 0; i < m_remotes.getNum(); i++) {
            TcpRemote& remote = m_remotes.at(i);

            sys::Lock lock(remote);

            // �󂢂Ă�����̂�T��
            if (!remote.isActive()) {
                remote.setSocket(socket);
                remote.m_endpoint.set(addr);

                return &remote.m_endpoint;
            }
        }

        return nullptr;
    }

    IZ_BOOL Tcp::isEnabledRemote(const IPv4Endpoint& remoteEp)
    {
        auto remote = findRemote(remoteEp);
        return (remote != nullptr);
    }

    IZ_INT Tcp::recieveFrom(
        IZ_UINT8* buf,
        IZ_UINT size,
        const IPv4Endpoint& remoteEp)
    {
        auto remote = findRemote(remoteEp);

        IZ_INT ret = 0;

        if (remote && remote->isActive()) {
            sys::Lock lock(*remote);

            ret = recv(remote->m_socket, (char*)buf, size, 0);

            if (ret < 0) {
                // TODO
            }
        }

        return ret;
    }

    TcpRemote* Tcp::findRemote(const IPv4Endpoint ep)
    {
        std::unique_lock<std::mutex> lock(m_remotesLocker);

        for (IZ_UINT i = 0; i < m_remotes.getNum(); i++) {
            auto& remote = m_remotes.at(i);

            if (remote.m_endpoint == ep) {
                return &remote;
            }
        }

        return nullptr;
    }

    // �f�[�^�𑗐M.
    IZ_BOOL Tcp::sendData(
        const void* data, 
        IZ_UINT size)
    {
        TcpRemote& remote = m_remotes.at(0);

        IZ_INT ret = 0;

        if (remote.isActive()) {
            sys::Lock locker(remote);

            ret = send(remote.m_socket, (const char*)data, size, 0);

            if (ret < 0) {
                // TODO
            }
        }

        return ret;
    }

    // �w�肵���ڑ���Ƀf�[�^�𑗐M.
    IZ_INT Tcp::sendData(
        const void* data,
        IZ_UINT size,
        const IPv4Endpoint& remoteEp)
    {
        TcpRemote* remote = findRemote(remoteEp);

        IZ_INT ret = 0;

        if (remote && remote->isActive()) {
            sys::Lock locker(*remote);

            ret = send(remote->m_socket, (const char*)data, size, 0);

            if (ret < 0) {
                // TODO
            }
        }

        return ret;
    }

    // �S�Ă̐ڑ���Ƀf�[�^�𑗐M.
    IZ_UINT Tcp::sendDataToAllRemote(const void* data, IZ_UINT size)
    {
        IZ_UINT succeededNum = 0;;

        for (IZ_UINT i = 0; i < m_remotes.getNum(); i++) {
            TcpRemote& remote = m_remotes.at(i);

            if (remote.isActive()) {
                sys::Lock locker(remote);

                // TODO
                IZ_BOOL result = send(remote.m_socket, (const char*)data, size, 0);

                succeededNum = (result ? succeededNum + 1 : succeededNum);
            }
        }

        return succeededNum;
    }

    // �T�[�o�[�Ɛڑ�����Ă��邩�ǂ���.
    IZ_BOOL Tcp::isConnected()
    {
        return m_isConnected.load();
    }

    // NOTE
    // select(2)�̑������Ƀf�B�X�N���v�^�̍ő�l��n���̂͊ԈႢ�HAdd Star
    //  http://mattn.kaoriya.net/software/lang/c/20090114140035.htm
    // libevent
    //  http://www.ninxit.com/blog-data/src/libevent/main.cpp.html

    IZ_INT Tcp::wait(
        IZ_UINT sec/*= 0*/,
        IZ_UINT usec/*= 0*/)
    {
        VRETURN(m_isRunnning.load());

        timeval t_val = { sec, usec };

        timeval* pTval = nullptr;

        if (sec > 0 || usec > 0) {
            pTval = &t_val;
        }

        fd_set readFD;
        fd_set exceptionFD;
        fd_set writeFD;

        FD_ZERO(&readFD);
        FD_ZERO(&exceptionFD);
        FD_ZERO(&writeFD);

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

                    // ���M����f�[�^������Ƃ�����
                    if (remote.isRegistered()) {
                        FD_SET(remote.m_socket, &writeFD);
                    }
                }
            }
        }

        // �t�@�C���f�B�X�N���v�^�i�\�P�b�g�j�̏�ԑJ�ڑ҂�
        auto resSelect = select(
            //FD_SETSIZE,
            0,
            &readFD,
            &writeFD,
            &exceptionFD,
            &t_val);

        if (resSelect <= 0) {
            return 0;
        }

        if (FD_ISSET(m_socket, &exceptionFD)) {
            return -1;
        }


        {
            std::unique_lock<std::mutex> lock(m_remotesLocker);

            for (IZ_UINT i = 0; i < m_remotes.getNum(); i++) {
                TcpRemote& remote = m_remotes.at(i);

                sys::Lock locker(remote);

                if (FD_ISSET(remote.m_socket, &exceptionFD)) {
                    return -1;
                }
            }
        }

        return 1;
    }    
}    // namespace net
}    // namespace izanagi
