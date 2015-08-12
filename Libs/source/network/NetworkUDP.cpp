#include "network/NetworkUDP.h"
#include "network/NetworkRemote.h"
#include "network/IPv4Endpoint.h"

// NOTE
// http://www.ne.jp/asahi/hishidama/home/tech/socket/

namespace izanagi {
namespace net {
    Udp::Packet* Udp::Packet::create(IMemoryAllocator* allocator, IZ_UINT len)
    {
        // �Ō�ɂO�����邽�߂P��������
        auto alignedLen = len + 1;

        // TODO
        // �S�o�C�g�A���C�����Ă���
        auto rest = alignedLen & 0x03;
        if (rest > 0) {
            rest = 4 - rest;
            alignedLen += rest;
        }

        size_t size = sizeof(Packet) + alignedLen;

        IZ_CHAR* buf = (IZ_CHAR*)ALLOC_ZERO(allocator, size);
        IZ_ASSERT(buf != nullptr);

        Packet* p = new(buf)Packet;
        buf += sizeof(Packet);

        p->data = buf;

        p->size = len;
        p->data[len] = 0;

        return p;
    }

    ///////////////////////////////////////////////////////////

    Udp::Udp()
    {
        m_allocator = nullptr;

        m_socket = IZ_INVALID_SOCKET;

        m_isRunnning.store(IZ_FALSE);
    }

    Udp::~Udp()
    {
        stop();
    }

    // �T�[�o�[�Ƃ��ċN��.
    IZ_BOOL Udp::startAsServer(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& endpoint)
    {
        if (m_isRunnning.load()) {
            return IZ_TRUE;
        }

        m_allocator = allocator;

        IZ_BOOL result = IZ_FALSE;

        // �\�P�b�g�̐���
        m_socket = socket(
            AF_INET,    // �A�h���X�t�@�~��
            SOCK_DGRAM, // �\�P�b�g�^�C�v
            0);         // �v���g�R��
        VRETURN(isValidSocket(m_socket));

        // �ʐM�|�[�g�E�A�h���X�̐ݒ�
        sockaddr_in inAddr;
        {
            FILL_ZERO(&inAddr, sizeof(inAddr));

            inAddr.sin_family = AF_INET;
            inAddr.sin_port = htons(endpoint.getPort());

            auto address = endpoint.getAddress();

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

        m_isRunnning.store(IZ_TRUE);

    __EXIT__:
        if (!result) {
            IZ_ASSERT(IZ_FALSE);

            stop();
        }

        return result;
    }

    // �N���C�A���g�Ƃ��ċN��.
    IZ_BOOL Udp::startAsClient(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& endpoint)
    {
        if (m_isRunnning.load()) {
            return IZ_TRUE;
        }

        auto address = endpoint.getAddress();

        VRETURN(!address.isAny());

        m_allocator = allocator;

        IZ_BOOL result = IZ_FALSE;

        // �\�P�b�g�̐���
        m_socket = socket(
            AF_INET,        // �A�h���X�t�@�~��
            SOCK_DGRAM,    // �\�P�b�g�^�C�v
            0);             // �v���g�R��
        VRETURN(isValidSocket(m_socket));

        // TODO
        // �T�[�o�[�P�̂�
        auto remote = UdpRemote::create(m_allocator);
        remote->m_endpoint = endpoint;
        m_remotes.AddTail(remote->getListItem());

        m_isRunnning.store(IZ_TRUE);

        // TODO
        result = IZ_TRUE;

        if (!result) {
            IZ_ASSERT(IZ_FALSE);

            stop();
        }

        return result;
    }

    // ��~.
    void Udp::stop()
    {
        m_isRunnning.store(IZ_FALSE);

        onStop();

        if (isValidSocket(m_socket)) {
            closesocket(m_socket);
            m_socket = IZ_INVALID_SOCKET;
        }

        {
            auto item = m_remotes.GetTop();

            while (item != IZ_NULL) {
                auto data = item->GetData();
                item = item->GetNext();

                UdpRemote::deteteRemote(
                    m_allocator,
                    data);
            }

            m_remotes.Clear();
        }

        {
            auto item = m_recvData.GetTop();

            while (item != IZ_NULL) {
                auto packet = item->GetData();
                auto next = item->GetNext();

                FREE(m_allocator, packet);

                item = next;
            }

            m_recvData.Clear();
        }
    }

    // ��M�����f�[�^���擾.
    IZ_BOOL Udp::recieve(std::function<void(const net::Packet&)> func)
    {
        Packet* src = nullptr;

        {
            std::unique_lock<std::mutex> lock(m_recvDataLocker);

            auto item = m_recvData.GetTop();
            if (item == IZ_NULL) {
                return IZ_FALSE;
            }

            item->Leave();
            src = item->GetData();
        }

        func(*src);

        FREE(m_allocator, src);

        return IZ_TRUE;
    }

    // ��M�����S�f�[�^���擾.
    IZ_BOOL Udp::recieveAll(std::function<void(const net::Packet&)> func)
    {
        std::unique_lock<std::mutex> lock(m_recvDataLocker);

        auto item = m_recvData.GetTop();
        if (item == IZ_NULL) {
            return IZ_FALSE;
        }

        while (item != IZ_NULL) {
            auto next = item->GetNext();

            auto src = item->GetData();
            item->Leave();

            func(*src);

            FREE(m_allocator, src);

            item = next;
        }

        return IZ_TRUE;
    }

    // �f�[�^�𑗐M.
    IZ_BOOL Udp::sendData(const void* data, IZ_UINT size)
    {
        IZ_BOOL result = IZ_FALSE;

        auto item = m_remotes.GetAt(0);

        if (item != IZ_NULL) {
            auto remote = item->GetData();

            if (remote->isActive()) {
                result = remote->registerData(m_allocator, 1, &data, &size);
            }
        }

        return result;
    }

    // �w�肵���ڑ���Ƀf�[�^�𑗐M.
    IZ_BOOL Udp::sendData(
        const IPv4Endpoint& endpoint,
        const void* data, IZ_UINT size)
    {
        IZ_BOOL result = IZ_FALSE;

        // �ΏۂƂȂ郊���[�g����T��
        UdpRemote* remote = findRemote(endpoint);
        VRETURN(remote != nullptr);

        if (remote && remote->isActive()) {
            // TODO
            result = remote->registerData(
                m_allocator,
                1,
                &data, &size);
            IZ_ASSERT(result);
        }

        return result;
    }

    UdpRemote* Udp::findRemote(const IPv4Endpoint& ep)
    {
        std::unique_lock<std::mutex> lock(m_remotesLocker);

        auto item = m_remotes.GetTop();

        while (item != IZ_NULL) {
            auto remote = item->GetData();

            if (remote->m_endpoint == ep) {
                return remote;
            }

            item = item->GetNext();
        }

        return nullptr;
    }

    UdpRemote* Udp::findRemote(const sockaddr_in& addr)
    {
        IPv4Endpoint ep;
        ep.set(addr);

        UdpRemote* ret = findRemote(ep);

        return ret;
    }

    void Udp::traverseRemotes(std::function<IZ_BOOL(UdpRemote*)> func)
    {
        auto item = m_remotes.GetTop();

        while (item != IZ_NULL) {
            auto remote = item->GetData();

            if (func(remote)) {
                break;
            }

            item = item->GetNext();
        }
    }

#if 0
    IZ_BOOL Udp::run(IZ_CHAR* recvBuf, IZ_UINT size)
    {
        VRETURN(m_isRunnning.load());

        // TODO
        timeval t_val = { 0, 1000 };

        fd_set readFD;
        fd_set exceptionFD;
        fd_set writeFD;

        FD_ZERO(&readFD);
        FD_ZERO(&exceptionFD);
        FD_ZERO(&writeFD);

        FD_SET(m_socket, &readFD);
        FD_SET(m_socket, &exceptionFD);

        traverseRemotes(
            [&](UdpRemote* remote)->IZ_BOOL {
                sys::Lock lock(*remote);

                if (remote->isRegistered()) {
                    FD_SET(m_socket, &writeFD);
                    return IZ_TRUE;
                }

                return IZ_FALSE;
        });

        // �t�@�C���f�B�X�N���v�^�i�\�P�b�g�j�̏�ԑJ�ڑ҂�
        auto resSelect = select(
            //FD_SETSIZE,
            0,
            &readFD,
            &writeFD,
            &exceptionFD,
            &t_val);

        if (resSelect <= 0) {
            return IZ_FALSE;
        }

        // ��M.
        if (FD_ISSET(m_socket, &readFD)) {
            sockaddr_in addr;
            sockaddr* paddr = (sockaddr*)&addr;
            IZ_INT len = sizeof(addr);

            IZ_INT ret = recvfrom(
                m_socket,
                recvBuf,
                size,
                0,
                paddr,
                &len);

            auto remote = findRemote(addr);

            if (ret > 0) {
                if (!remote) {
                    // �����[�g��ǉ�
                    remote = UdpRemote::create(m_allocator);
                    remote->m_endpoint.set(addr);

                    std::unique_lock<std::mutex> lock(m_remotesLocker);
                    m_remotes.AddTail(remote->getListItem());
                }

                auto packet = Packet::create(m_allocator, len);
                packet->endpoint = remote->m_endpoint;
                memcpy(packet->data, recvBuf, len);

                std::unique_lock<std::mutex> lock(m_recvDataLocker);
                m_recvData.AddTail(&packet->listItem);
            }
            else {
                // TODO
                // �ؒf���ꂽ
            }

            if (len <= 0) {
                // TODO
                // �ؒf���ꂽ
            }
        }

        // ���M.
        if (FD_ISSET(m_socket, &writeFD)) {
            std::unique_lock<std::mutex> lock(m_remotesLocker);

            traverseRemotes(
                [this](UdpRemote* remote)->IZ_BOOL {
                    sys::Lock lock(*remote);

                    IZ_INT ret = remote->sendData(m_socket);

                    if (ret <= 0) {
                        // TODO
                        // �ؒf���ꂽ
                    }

                    return IZ_FALSE;
            });
        }

        if (FD_ISSET(m_socket, &exceptionFD)) {
            // TODO
            IZ_ASSERT(IZ_FALSE);
        }

        return IZ_TRUE;
    }
#else
    IZ_BOOL Udp::run(IZ_CHAR* recvBuf, IZ_UINT size)
    {
        VRETURN(m_isRunnning.load());

        // TODO
        timeval t_val = { 0, 1000 };

        fd_set readFD;
        fd_set exceptionFD;

        FD_ZERO(&readFD);
        FD_ZERO(&exceptionFD);

        FD_SET(m_socket, &readFD);
        FD_SET(m_socket, &exceptionFD);

        // ���M.
        {
            std::unique_lock<std::mutex> lock(m_remotesLocker);

            traverseRemotes(
                [this](UdpRemote* remote)->IZ_BOOL {
                    sys::Lock lock(*remote);

                    if (remote->isRegistered()) {
                        IZ_INT ret = remote->sendData(m_socket);

                        if (ret <= 0) {
                            // TODO
                            // �ؒf���ꂽ
                        }
                    }

                    return IZ_FALSE;
            });
        }

        // �t�@�C���f�B�X�N���v�^�i�\�P�b�g�j�̏�ԑJ�ڑ҂�
        auto resSelect = select(
            //FD_SETSIZE,
            0,
            &readFD,
            NULL,
            &exceptionFD,
            &t_val);

        if (resSelect <= 0) {
            return IZ_FALSE;
        }

        // ��M.
        if (FD_ISSET(m_socket, &readFD)) {
            sockaddr_in addr;
            sockaddr* paddr = (sockaddr*)&addr;
            IZ_INT len = sizeof(addr);

            IZ_INT ret = recvfrom(
                m_socket,
                recvBuf,
                size,
                0,
                paddr,
                &len);

            auto remote = findRemote(addr);

            if (ret > 0) {
                if (!remote) {
                    // �����[�g��ǉ�
                    remote = UdpRemote::create(m_allocator);
                    remote->m_endpoint.set(addr);

                    std::unique_lock<std::mutex> lock(m_remotesLocker);
                    m_remotes.AddTail(remote->getListItem());
                }

                auto packet = Packet::create(m_allocator, len);
                packet->endpoint = remote->m_endpoint;
                memcpy(packet->data, recvBuf, len);

                std::unique_lock<std::mutex> lock(m_recvDataLocker);
                m_recvData.AddTail(&packet->listItem);
            }
            else {
                // TODO
                // �ؒf���ꂽ
            }

            if (len <= 0) {
                // TODO
                // �ؒf���ꂽ
            }
        }

        if (FD_ISSET(m_socket, &exceptionFD)) {
            // TODO
            IZ_ASSERT(IZ_FALSE);
        }

        return IZ_TRUE;
    }
#endif

    void Udp::loop()
    {
        // TODO
        IZ_UINT size = 1 * 1024 * 1024;
        IZ_CHAR* recvBuf = (IZ_CHAR*)ALLOC_ZERO(m_allocator, size);

        if (recvBuf == nullptr) {
            IZ_ASSERT(IZ_FALSE);
            return;
        }

        for (;;) {
            if (!m_isRunnning.load()) {
                break;
            }

            run(recvBuf, size);
        }

        FREE(m_allocator, recvBuf);
    }
}    // namespace net
}    // namespace izanagi
