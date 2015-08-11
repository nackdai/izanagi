#include "network/NetworkRemote.h"

namespace izanagi {
namespace net {
    Remote* Remote::create(IMemoryAllocator* allocator)
    {
        void* buf = ALLOC(allocator, sizeof(Remote));
        Remote* client = new(buf) Remote;
        return client;
    }

    void Remote::deteteRemote(
        IMemoryAllocator* allocator,
        Remote* client)
    {
        FREE(allocator, client);
    }

    Remote::Remote()
    {
        m_socket = IZ_INVALID_SOCKET;
        
        m_allocator = nullptr;
        m_sendPacket.size = 0;
        m_sendPacket.data = nullptr;

        m_isRegistered = IZ_FALSE;
    }

    Remote::~Remote()
    {}

    // �N���C�A���g�Ɛڑ����Ă���\�P�b�g�����蓖�Ă�.
    void Remote::setSocket(IZ_SOCKET socket)
    {
        m_socket = socket;
    }

    // �N���C�A���g�Ɛڑ����Ă���\�P�b�g���擾.
    IZ_SOCKET Remote::getSocekt()
    {
        return m_socket;
    }

    // �\�P�b�g���L�����ǂ������擾.
    IZ_BOOL Remote::isActive() const
    {
        return (m_socket != IZ_INVALID_SOCKET);
    }

    // �f�[�^�𑗐M.
    IZ_INT Remote::sendData(const void* data, IZ_UINT size)
    {
        VRETURN_VAL(isValidSocket(m_socket), 0);
        IZ_INT ret = send(m_socket, (const char*)data, size, 0);
        return ret;
    }

    // �o�^����Ă���f�[�^�𑗐M.
    IZ_INT Remote::sendData()
    {
        if (m_sendPacket.data == nullptr
            || !m_isRegistered)
        {
            return 0;
        }

        VRETURN_VAL(isValidSocket(m_socket), 0);

        IZ_INT ret = sendData(m_sendPacket.data, m_sendPacket.size);

        if (ret > 0) {
            m_isRegistered = IZ_FALSE;
        }

        return ret;
    }

    // �f�[�^����M.
    IZ_INT Remote::recieveData(void* data, IZ_UINT size)
    {
        VRETURN_VAL(isValidSocket(m_socket), 0);
        IZ_INT ret = recv(m_socket, (char*)data, size, 0);
        return ret;
    }

    // ���M�f�[�^��o�^.
    IZ_BOOL Remote::registerData(
        IMemoryAllocator* allocator,
        IZ_UINT num,
        const void** data, IZ_UINT* size)
    {
        IZ_ASSERT(allocator != IZ_NULL);

        IZ_UINT total = 0;
        for (IZ_UINT i = 0; i < num; i++) {
            total += size[i];
        }

        if (m_sendPacket.data == nullptr) {
            // ���������m�ۂ���Ă��Ȃ��ꍇ
            m_sendPacket.data = (IZ_CHAR*)ALLOC(allocator, total);
            VRETURN(m_sendPacket.data);

            m_sendPacket.size = total;

            m_allocator = allocator;
        }
        else if (m_sendPacket.size < total) {
            // �T�C�Y���������ꍇ
            VRETURN(m_allocator == allocator);

            m_sendPacket.data = (IZ_CHAR*)REALLOC(allocator, m_sendPacket.data, total);
            VRETURN(m_sendPacket.data);

            m_sendPacket.size = total;
        }

        auto dst = m_sendPacket.data;

        for (IZ_UINT i = 0; i < num; i++) {
            auto s = size[i];

            memcpy(dst, data[i], s);
            dst += s;
        }

        m_isRegistered = IZ_TRUE;

        return IZ_TRUE;
    }

    void Remote::close()
    {
        if (isValidSocket(m_socket)) {
            shutdown(m_socket, SD_BOTH);
            closesocket(m_socket);
            m_socket = IZ_INVALID_SOCKET;
        }

        if (m_allocator != nullptr) {
            FREE(m_allocator, m_sendPacket.data);

            m_sendPacket.size = 0;
            m_sendPacket.data = nullptr;

            m_allocator = nullptr;
        }
    }

    void Remote::reset()
    {
        if (isValidSocket(m_socket)) {
            closesocket(m_socket);
            m_socket = IZ_INVALID_SOCKET;
        }

        m_isRegistered = IZ_FALSE;
    }
}    // namespace net
}    // namespace izanagi
