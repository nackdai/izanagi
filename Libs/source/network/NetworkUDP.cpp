#include "network/NetworkUDP.h"
#include "network/NetworkRemote.h"
#include "network/IPv4Endpoint.h"

// NOTE
// http://www.ne.jp/asahi/hishidama/home/tech/socket/

namespace izanagi {
namespace net {
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
            AF_INET,        // �A�h���X�t�@�~��
            SOCK_DGRAM,     // �\�P�b�g�^�C�v
            IPPROTO_UDP);   // �v���g�R��
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

    // ��~.
    void Udp::stop()
    {
        m_isRunnning.store(IZ_FALSE);

        //onStop();

        if (isValidSocket(m_socket)) {
            closesocket(m_socket);
            m_socket = IZ_INVALID_SOCKET;
        }

        auto item = m_remotes.GetTop();

        while (item != IZ_NULL) {
            auto data = item->GetData();
            item = item->GetNext();
            
            //data->close();

            UdpRemote::deteteRemote(
                data->m_allocator,
                data);
        }

        m_remotes.Clear();
    }

    UdpRemote* Udp::findRemote(const sockaddr_in& addr)
    {
        auto item = m_remotes.GetTop();

        while (item != IZ_NULL) {
            auto remote = item->GetData();

            if (remote->m_endpoint == addr) {
                return remote;
            }

            item = item->GetNext();
        }

        return nullptr;
    }

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

            if (ret > 0) {
                // TODO
            }
            else {
                // TODO
                // �ؒf���ꂽ
            }

            if (len <= 0) {
                // TODO
                // �ؒf���ꂽ
            }

            auto remote = findRemote(addr);

            if (!remote) {
                // �����[�g��ǉ�
                remote = UdpRemote::create(m_allocator);
                remote->m_endpoint.set(addr);
                m_remotes.AddTail(remote->getListItem());
            }
        }

        // ���M.
        if (FD_ISSET(m_socket, &writeFD)) {
            auto item = m_remotes.GetTop();

            while (item != IZ_NULL) {
                auto remote = item->GetData();



                item = item->GetNext();
            }
        }

        if (FD_ISSET(m_socket, &exceptionFD)) {
            // TODO
            IZ_ASSERT(IZ_FALSE);
        }

        return IZ_TRUE;
    }
}    // namespace net
}    // namespace izanagi
