#include "network/NetworkTCP.h"
#include "network/IPv4Endpoint.h"

// NOTE
// http://www.jenkinssoftware.com/raknet/manual/systemoverview.html

namespace izanagi {
namespace net {
    // �N���C�A���g.
    class TcpClient : public CPlacementNew, sys::CSpinLock {
        friend class Tcp;
        friend class CArray < TcpClient >;

    private:
        static TcpClient* create(IMemoryAllocator* allocator);

        static void deteteClient(
            IMemoryAllocator* allocator,
            TcpClient* client);

    private:
        TcpClient();
        ~TcpClient();

    private:
        // �N���C�A���g�Ɛڑ����Ă���\�P�b�g�����蓖�Ă�.
        void setSocket(IZ_SOCKET socket);

        // �N���C�A���g�Ɛڑ����Ă���\�P�b�g���擾.
        IZ_SOCKET getSocekt();

        // �\�P�b�g���L�����ǂ������擾.
        IZ_BOOL isActive() const;

        // �f�[�^�𑗐M.
        IZ_INT sendData(const void* data, IZ_UINT size);

        // �o�^����Ă���f�[�^�𑗐M.
        IZ_INT sendData();

        // �f�[�^����M.
        IZ_INT recieveData(void* data, IZ_UINT size);

        // ���M�f�[�^��o�^.
        IZ_BOOL registerData(
            IMemoryAllocator* allocator,
            IZ_UINT num,
            const void** data, IZ_UINT* size);

        void close();

        void reset();

    private:
        IZ_SOCKET m_socket;
        IPv4Endpoint m_endpoint;

        IMemoryAllocator* m_allocator;
        Packet m_sendPacket;

        IZ_BOOL m_isRegistered;
    };

    TcpClient* TcpClient::create(IMemoryAllocator* allocator)
    {
        void* buf = ALLOC(allocator, sizeof(TcpClient));
        TcpClient* client = new(buf) TcpClient;
        return client;
    }

    void TcpClient::deteteClient(
        IMemoryAllocator* allocator,
        TcpClient* client)
    {
        FREE(allocator, client);
    }

    TcpClient::TcpClient()
    {
        m_socket = IZ_INVALID_SOCKET;
        
        m_allocator = nullptr;
        m_sendPacket.size = 0;
        m_sendPacket.data = nullptr;

        m_isRegistered = IZ_FALSE;
    }

    TcpClient::~TcpClient()
    {}

    // �N���C�A���g�Ɛڑ����Ă���\�P�b�g�����蓖�Ă�.
    void TcpClient::setSocket(IZ_SOCKET socket)
    {
        m_socket = socket;
    }

    // �N���C�A���g�Ɛڑ����Ă���\�P�b�g���擾.
    IZ_SOCKET TcpClient::getSocekt()
    {
        return m_socket;
    }

    // �\�P�b�g���L�����ǂ������擾.
    IZ_BOOL TcpClient::isActive() const
    {
        return (m_socket != IZ_INVALID_SOCKET);
    }

    // �f�[�^�𑗐M.
    IZ_INT TcpClient::sendData(const void* data, IZ_UINT size)
    {
        VRETURN_VAL(isValidSocket(m_socket), 0);
        IZ_INT ret = send(m_socket, (const char*)data, size, 0);
        return ret;
    }

    // �o�^����Ă���f�[�^�𑗐M.
    IZ_INT TcpClient::sendData()
    {
        if (m_sendPacket.data == nullptr
            || !m_isRegistered)
        {
            return 0;
        }

        VRETURN_VAL(isValidSocket(m_socket), 0);

        IZ_INT ret = send(m_socket, (const char*)m_sendPacket.data, m_sendPacket.size, 0);

        if (ret > 0) {
            m_isRegistered = IZ_FALSE;
        }

        return ret;
    }

    // �f�[�^����M.
    IZ_INT TcpClient::recieveData(void* data, IZ_UINT size)
    {
        VRETURN_VAL(isValidSocket(m_socket), 0);
        IZ_INT ret = recv(m_socket, (char*)data, size, 0);
        return ret;
    }

    // ���M�f�[�^��o�^.
    IZ_BOOL TcpClient::registerData(
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

    void TcpClient::close()
    {
        if (isValidSocket(m_socket)) {
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

    void TcpClient::reset()
    {
        if (isValidSocket(m_socket)) {
            closesocket(m_socket);
            m_socket = IZ_INVALID_SOCKET;
        }

        m_isRegistered = IZ_FALSE;
    }

    ///////////////////////////////////////////////////////////

    Tcp::Packet* Tcp::Packet::create(IMemoryAllocator* allocator, IZ_UINT len)
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

        IZ_CHAR* buf = (IZ_CHAR*)ALLOC(allocator, size);
        IZ_ASSERT(buf != nullptr);

        Packet* p = new(buf)Packet;
        buf += sizeof(Packet);

        p->data = buf;

        p->size = len;
        p->data[len] = 0;

        return p;
    }

    ///////////////////////////////////////////////////////////

    Tcp::Tcp()
    {
        m_allocator = nullptr;

        m_socket = IZ_INVALID_SOCKET;

        m_isRunnning.store(IZ_FALSE);
    }

    Tcp::~Tcp()
    {
        stop();
    }

    // �T�[�o�[�Ƃ��ċN��.
    IZ_BOOL Tcp::startAsServer(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& endpoint,
        IZ_UINT maxConnections)
    {
        VRETURN(maxConnections > 0);

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

        // �R�l�N�g�v���������܂ő҂���ݒ�
        result = (listen(m_socket, maxConnections) >= 0);
        VGOTO(result, __EXIT__);

        result = m_clients.init(m_allocator, maxConnections);
        VGOTO(result, __EXIT__);

        m_isRunnning.store(IZ_TRUE);

    __EXIT__:
        if (!result) {
            IZ_ASSERT(IZ_FALSE);

#if 0
            if (isValidSocket(m_socket)) {
                closesocket(m_socket);
            }

            m_clients.clear();
#else
            stop();
#endif
        }

        return result;
    }

    // �N���C�A���g�Ƃ��ċN��.
    IZ_BOOL Tcp::startAsClient(
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
            SOCK_STREAM,    // �\�P�b�g�^�C�v
            0);             // �v���g�R��
        VRETURN(isValidSocket(m_socket));

        // �T�[�o�[�P�̂�
        result = m_clients.init(m_allocator, 1);
        VGOTO(result, __EXIT__);

        m_clients.at(0).m_endpoint = endpoint;

        m_isRunnning.store(IZ_TRUE);

    __EXIT__:
        if (!result) {
            IZ_ASSERT(IZ_FALSE);

#if 0
            if (isValidSocket(m_socket)) {
                closesocket(m_socket);
            }

            m_clients.clear();
#else
            stop();
#endif
        }
        return result;
    }

    // ��~.
    void Tcp::stop()
    {
        m_isRunnning.store(IZ_FALSE);

        onStop();

        if (isValidSocket(m_socket)) {
            shutdown(m_socket, SD_BOTH);
            closesocket(m_socket);
            m_socket = IZ_INVALID_SOCKET;
        }

        m_clients.traverse(
            [this] (TcpClient& client) {
            client.close();
        });

        m_clients.clear();
    }

    IZ_UINT Tcp::getRemoteNum() const
    {
        IZ_UINT ret = m_clients.getNum();
        return ret;
    }

    const IPv4Endpoint* Tcp::getRemote(IZ_UINT idx) const
    {
        const TcpClient& c = m_clients.at(idx);
        
        if (!c.isActive()) {
            return IZ_NULL;
        }

        return &c.m_endpoint;
    }

    IZ_BOOL Tcp::recieve(std::function<void(const net::Packet&)> func)
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

    IZ_BOOL Tcp::sendData(const void* data, IZ_UINT size)
    {
        TcpClient& c = m_clients.at(0);
        return c.registerData(m_allocator, 1, &data, &size);
    }

    IZ_BOOL Tcp::sendData(
        const IPv4Endpoint& endpoint,
        const void* data, IZ_UINT size)
    {
        TcpClient* remote = nullptr;

        // �ΏۂƂȂ郊���[�g����T��
        for (IZ_UINT i = 0; i < m_clients.getNum(); i++) {
            TcpClient& c = m_clients.at(i);

            if (c.isActive() && c.m_endpoint == endpoint) {
                remote = &c;
                break;
            }
        }

        VRETURN(remote != nullptr);

        // TODO
        IZ_BOOL result = remote->registerData(
            m_allocator,
            1,
            &data, &size);
        VRETURN(result);

        return result;
    }

    IZ_UINT Tcp::sendDataToAllRemote(const void* data, IZ_UINT size)
    {
        IZ_UINT succeededNum = 0;;

        for (IZ_UINT i = 0; i < m_clients.getNum(); i++) {
            TcpClient& c = m_clients.at(i);

            if (c.isActive()) {
                // TODO
                IZ_BOOL result = c.registerData(
                    m_allocator,
                    1,
                    &data, &size);

                succeededNum = (result ? succeededNum + 1 : succeededNum);
            }
        }

        return succeededNum;
    }

    IZ_BOOL Tcp::connectServer()
    {
        const IPv4Endpoint& endpoint = m_clients.at(0).m_endpoint;

        // �ʐM�|�[�g�E�A�h���X�̐ݒ�
        sockaddr_in serverAddr;
        {
            FILL_ZERO(&serverAddr, sizeof(serverAddr));

            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(endpoint.getPort());

            auto address = endpoint.getAddress();

            IZ_CHAR ip[64];
            address.toString(ip, COUNTOF(ip));

            serverAddr.sin_addr.S_un.S_addr = inet_addr(ip);
        }

        IZ_BOOL result = (connect(
            m_socket,
            (sockaddr*)&serverAddr,
            sizeof(serverAddr)) >= 0);

        if (result) {
            TcpClient& c = m_clients.at(0);

            sys::Lock lock(c);
            c.setSocket(m_socket);
        }
        else {
            // TODO
            IZ_ASSERT(IZ_FALSE);
        }

        return result;
    }

    // NOTE
    // select(2)�̑������Ƀf�B�X�N���v�^�̍ő�l��n���̂͊ԈႢ�HAdd Star
    //  http://mattn.kaoriya.net/software/lang/c/20090114140035.htm
    // libevent
    //  http://www.ninxit.com/blog-data/src/libevent/main.cpp.html

    IZ_BOOL Tcp::run(IZ_CHAR* recvBuf, IZ_UINT size)
    {
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

        for (IZ_UINT i = 0; i < m_clients.getNum(); i++) {
            TcpClient& c = m_clients.at(i);

            sys::Lock lock(c);

            if (c.isActive()) {
                FD_SET(c.m_socket, &readFD);
                FD_SET(c.m_socket, &writeFD);
                FD_SET(c.m_socket, &exceptionFD);
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
            return IZ_FALSE;
        }

        if (FD_ISSET(m_socket, &readFD)) {
            sockaddr_in addr;
            sockaddr* paddr = (sockaddr*)&addr;
            IZ_INT len = sizeof(addr);

            // �N���C�A���g����̐ڑ��҂�
            auto socket = accept(m_socket, paddr, &len);

            if (!isValidSocket(socket)) {
                return IZ_FALSE;
            }

            for (IZ_UINT i = 0; i < m_clients.getNum(); i++) {
                TcpClient& c = m_clients.at(i);

                sys::Lock lock(c);

                // �󂢂Ă�����̂�T��
                if (!c.isActive()) {
                    c.setSocket(socket);
                    c.m_endpoint.set(addr);
                    break;
                }
            }
        }
        else if (FD_ISSET(m_socket, &exceptionFD)) {
            // TODO
        }

        for (IZ_UINT i = 0; i < m_clients.getNum(); i++) {
            TcpClient& c = m_clients.at(i);

            if (FD_ISSET(c.m_socket, &exceptionFD)) {
                // TODO
            }
            else {
                // ��M
                if (FD_ISSET(c.m_socket, &readFD)) {
                    sys::Lock lock(c);

                    auto len = c.recieveData(recvBuf, size);

                    if (len > 0) {
                        auto packet = Packet::create(m_allocator, len);
                        packet->endpoint = c.m_endpoint;
                        memcpy(packet->data, recvBuf, len);

                        std::unique_lock<std::mutex> lock(m_recvDataLocker);
                        m_recvData.AddTail(&packet->listItem);
                    }
                    else {
                        // �ؒf���ꂽ
                        c.reset();
                    }
                }

                // ���M
                if (FD_ISSET(c.m_socket, &writeFD)) {
                    sys::Lock lock(c);

                    auto len = c.sendData();

                    if (len < 0) {
                        // �ؒf���ꂽ
                        c.reset();
                    }
                }
            }
        }

        return IZ_TRUE;
    }
    
    void Tcp::loop()
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
