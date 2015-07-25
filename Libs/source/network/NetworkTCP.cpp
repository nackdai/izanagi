#include "network/NetworkTCP.h"
#include "network/IPv4Endpoint.h"

// NOTE
// http://www.jenkinssoftware.com/raknet/manual/systemoverview.html

namespace izanagi {
namespace net {
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
	}

	TcpClient::~TcpClient()
	{}

	void TcpClient::setSocket(IZ_SOCKET socket)
	{
		m_socket = socket;
	}

	IZ_SOCKET TcpClient::getSocekt()
	{
		return m_socket;
	}

	IZ_BOOL TcpClient::isActive()
	{
		return (m_socket != IZ_INVALID_SOCKET);
	}

	IZ_INT TcpClient::sendData(const void* data, IZ_UINT size)
	{
		VRETURN_VAL(isValidSocket(m_socket), 0);
		IZ_INT ret = send(m_socket, (const char*)data, size, 0);
		return ret;
	}

	IZ_INT TcpClient::recieveData(void* data, IZ_UINT size)
	{
		VRETURN_VAL(isValidSocket(m_socket), 0);
		IZ_INT ret = recv(m_socket, (char*)data, size, 0);
		return ret;
	}

	void TcpClient::close()
	{
		if (isValidSocket(m_socket)) {
			closesocket(m_socket);
			m_socket = IZ_INVALID_SOCKET;
		}
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

	IZ_BOOL Tcp::start(
		IMemoryAllocator* allocator,
		IPv4Endpoint endpoint,
		IZ_UINT maxConnections)
	{
		if (m_isRunnning.load()) {
			return IZ_TRUE;
		}

		m_allocator = allocator;

		IZ_BOOL ret = IZ_FALSE;

		if (maxConnections > 0) {
			ret = startAsServer(endpoint, maxConnections);
		}

		return ret;
	}

	IZ_BOOL Tcp::startAsServer(
		IPv4Endpoint endpoint,
		IZ_UINT maxConnections)
	{
		IZ_BOOL result = IZ_FALSE;

		// ソケットの生成
		m_socket = socket(
			AF_INET,        // アドレスファミリ
			SOCK_STREAM,    // ソケットタイプ
			0);             // プロトコル
		VRETURN(isValidSocket(m_socket));

		// 通信ポート・アドレスの設定
		sockaddr_in inAddr;
		{
			FILL_ZERO(&inAddr, sizeof(inAddr));

			inAddr.sin_family = AF_INET;
			inAddr.sin_port = htons(endpoint.getPort());

			auto address = endpoint.getAddress();

			if (address.isAny()) {
				inAddr.sin_addr.S_un.S_addr = INADDR_ANY;
			}
			else {
				IZ_CHAR ip[64];
				address.toString(ip, COUNTOF(ip));

				inAddr.sin_addr.S_un.S_addr = inet_addr(ip);
			}
		}

		// ソケットにアドレスを結びつける
		result = (bind(m_socket, (const sockaddr*)&inAddr, sizeof(inAddr)) > 0);
		VGOTO(result, __EXIT__);

		// コネクト要求をいくつまで待つかを設定
		result = listen(m_socket, maxConnections);
		VGOTO(result, __EXIT__);

		result = m_clients.init(m_allocator, maxConnections);
		VGOTO(result, __EXIT__);

		m_isRunnning.store(IZ_TRUE);

		result = m_thread.Start(m_allocator);
		VGOTO(result, __EXIT__);

		m_thread.Init(
			[this](void* data) {
			loop();
		},
			nullptr);

	__EXIT__:
		if (!result) {
			IZ_ASSERT(IZ_FALSE);

			if (isValidSocket(m_socket)) {
				closesocket(m_socket);
			}

			m_clients.clear();
		}

		return result;
	}

	void Tcp::stop()
	{
		m_isRunnning.store(IZ_FALSE);
		m_thread.Join();

		if (isValidSocket(m_socket)) {
			shutdown(m_socket, SD_BOTH);
			closesocket(m_socket);
			m_socket = IZ_INVALID_SOCKET;
		}

		m_clients.traverse(
			[this] (TcpClient& client) {
			client.close();
		});
	}

	// NOTE
	// select(2)の第一引数にディスクリプタの最大値を渡すのは間違い？Add Star
	//  http://mattn.kaoriya.net/software/lang/c/20090114140035.htm
	// libevent
	//  http://www.ninxit.com/blog-data/src/libevent/main.cpp.html
	
	void Tcp::loop()
	{
		// TODO
		IZ_UINT size = 1 * 1024 * 1024;
		IZ_CHAR* recvBuf = (IZ_CHAR*)ALLOC_ZERO(m_allocator, size);

		if (recvBuf == nullptr) {
			IZ_ASSERT(IZ_FALSE);
			return;
		}

		fd_set readFD;
		fd_set exceptionFD;
		fd_set writeFD;

		for (;;) {
			if (!m_isRunnning.load()) {
				break;
			}

			FD_ZERO(&readFD);
			FD_ZERO(&exceptionFD);
			FD_ZERO(&writeFD);

			FD_SET(m_socket, &readFD);
			FD_SET(m_socket, &exceptionFD);

			for (IZ_UINT i = 0; i < m_clients.getNum(); i++) {
				TcpClient& c = m_clients.at(i);

				sys::Lock lock(c);

				if (!c.isActive()) {
					FD_SET(c.m_socket, &readFD);
					FD_SET(c.m_socket, &exceptionFD);

					// TODO
					// FD_SET(c.m_socket, &writeFD);
				}
			}

			auto resSelect = select(
				FD_SETSIZE,
				&readFD,
				&writeFD,
				&exceptionFD,
				nullptr);

			if (resSelect <= 0) {
				continue;
			}

			if (FD_ISSET(m_socket, &readFD)) {
				sockaddr_in addr;
				sockaddr* paddr = (sockaddr*)&addr;
				IZ_INT len = sizeof(addr);

				// クライアントからの接続待ち
				auto socket = accept(m_socket, paddr, &len);

				if (!isValidSocket(socket)) {
					continue;
				}

				for (IZ_UINT i = 0; i < m_clients.getNum(); i++) {
					TcpClient& c = m_clients.at(i);

					sys::Lock lock(c);

					// 空いているものを探す
					if (!c.isActive()) {
						c.setSocket(socket);
						memcpy(&c.m_address, paddr, len);
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
					// 受信
					if (FD_ISSET(c.m_socket, &readFD)) {
						sys::Lock lock(c);

						auto len = c.recieveData(recvBuf, size);

						if (len > 0) {
							auto packet = Packet::create(m_allocator);
							packet->endpoint.set(c.m_address);
							packet->size = len;
							packet->data = (IZ_CHAR*)ALLOC(m_allocator, len + 1);
							memcpy(packet->data, recvBuf, len);
							packet->data[len] = 0;
						}
						else {
							// 切断された
						}
					}

					// 送信
					if (FD_ISSET(c.m_socket, &writeFD)) {
						sys::Lock lock(c);
					}
				}
			}
		}

		FREE(m_allocator, recvBuf);
	}
}	// namespace net
}	// namespace izanagi
