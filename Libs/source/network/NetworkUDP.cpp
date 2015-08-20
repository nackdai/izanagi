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

        m_isRunning.store(IZ_FALSE);

        m_recvBuf = IZ_NULL;
        m_bufSize = 0;
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
        if (m_isRunning.load()) {
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

        // TODO
        m_bufSize = 1 * 1024 * 1024;
        m_recvBuf = (IZ_CHAR*)ALLOC_ZERO(m_allocator, m_bufSize);

        m_isRunning.store(IZ_TRUE);

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
        if (m_isRunning.load()) {
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

        // TODO
        m_bufSize = 1 * 1024 * 1024;
        m_recvBuf = (IZ_CHAR*)ALLOC_ZERO(m_allocator, m_bufSize);

        m_isRunning.store(IZ_TRUE);

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
        m_isRunning.store(IZ_FALSE);

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

        FREE(m_allocator, m_recvBuf);
    }

    // ��M�����f�[�^���擾.
    IZ_BOOL Udp::recieve(std::function<void(const net::Packet&)> func)
    {
        Packet* packet = nullptr;

        IZ_BOOL result = onRecieve(packet);

        if (result) {
            func(*packet);

            FREE(m_allocator, packet);
        }

        return result;
    }

    IZ_BOOL Udp::onRecieve(Udp::Packet*& packet)
    {
        sockaddr_in addr;
        sockaddr* paddr = (sockaddr*)&addr;
        IZ_INT len = sizeof(addr);

        IZ_INT ret = recvfrom(
            m_socket,
            m_recvBuf,
            m_bufSize,
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

            packet = Packet::create(m_allocator, len);
            packet->endpoint = remote->m_endpoint;
            memcpy(packet->data, m_recvBuf, len);
        }
        else {
            // TODO
            // �ؒf���ꂽ

            if (remote) {
                // TODO
            }

            return IZ_FALSE;
        }

        return IZ_TRUE;
    }

    // �f�[�^�𑗐M.
    IZ_BOOL Udp::send(const void* data, IZ_UINT size)
    {
        IZ_BOOL result = IZ_FALSE;

        auto item = m_remotes.GetAt(0);

        if (item != IZ_NULL) {
            auto remote = item->GetData();

            if (remote->isActive()) {
                sockaddr_in addr;
                remote->m_endpoint.get(addr);

                IZ_INT len = sendto(
                    m_socket,
                    (const char*)data,
                    size,
                    0,
                    (sockaddr*)&addr,
                    sizeof(addr));

                result = (len >= 0);

                if (!result) {
                    // TODO
                    // �ؒf���ꂽ
                }
            }
        }

        return result;
    }

    // �w�肵���ڑ���Ƀf�[�^�𑗐M.
    IZ_BOOL Udp::sendTo(
        IPv4Endpoint& endpoint,
        const void* data, IZ_UINT size)
    {
        IZ_BOOL result = IZ_FALSE;
        
        sockaddr_in addr;
        endpoint.get(addr);

        IZ_INT len = sendto(
            m_socket,
            (const char*)data,
            size,
            0,
            (sockaddr*)&addr,
            sizeof(addr));

        result = (len >= 0);

        if (!result) {
            // TODO
            // �ؒf���ꂽ

            auto remote = findRemote(endpoint);
            if (remote) {
                // TODO
            }
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

    Udp::Packet* Udp::createPacket(IZ_UINT len)
    {
        return Packet::create(m_allocator, len);
    }

    void Udp::deletePacket(Udp::Packet* packet)
    {
        FREE(m_allocator, packet);
    }

    IZ_BOOL Udp::isRunning()
    {
        return m_isRunning;
    }

    // NOTE
    // UDP�ł�select�͕s�v
    // http://stackoverflow.com/questions/19758152/select-for-udp-connection

    IZ_INT Udp::waitForRecieving()
    {
        IZ_ASSERT(m_isRunning.load());

        // TODO
        timeval t_val = { 0, 1000 };

        fd_set readFD;
        fd_set exceptionFD;

        FD_ZERO(&readFD);
        FD_ZERO(&exceptionFD);

        FD_SET(m_socket, &readFD);
        FD_SET(m_socket, &exceptionFD);

        // �t�@�C���f�B�X�N���v�^�i�\�P�b�g�j�̏�ԑJ�ڑ҂�
        auto resSelect = select(
            //FD_SETSIZE,
            0,
            &readFD,
            NULL,
            &exceptionFD,
            &t_val);

        if (resSelect <= 0) {
            return 0;
        }

        // ��M.
        if (FD_ISSET(m_socket, &readFD)) {
            return 1;
        }

        if (FD_ISSET(m_socket, &exceptionFD)) {
            // TODO
            IZ_ASSERT(IZ_FALSE);
            return -1;
        }

        return 0;

    }
}    // namespace net
}    // namespace izanagi
