#if !defined(_IZANAGI_NETWORK_NETWORK_TCP_H__)
#define _IZANAGI_NETWORK_NETWORK_TCP_H__

#include "izDefs.h"
#include "izSystem.h"
#include "network/NetworkDefs.h"
#include "network/NetworkPacket.h"

namespace izanagi {
namespace net {
	// 
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
		void setSocket(IZ_SOCKET socket);

		IZ_SOCKET getSocekt();

		IZ_BOOL isActive();

		IZ_INT sendData(const void* data, IZ_UINT size);

		IZ_INT recieveData(void* data, IZ_UINT size);

		void close();

	private:
		IZ_SOCKET m_socket;
		sockaddr_in m_address;
	};

	////////////////////////////////////////////////////////

	/**
	 */
	class Tcp {
	public:
		Tcp();
		~Tcp();

	public:
		IZ_BOOL start(
			IMemoryAllocator* allocator,
			const IPv4Endpoint& endpoint,
			IZ_UINT maxConnections);

		void stop();

		IZ_BOOL recieve(std::function<void(const net::Packet&)> func);

	private:
		IZ_BOOL startAsServer(
			const IPv4Endpoint& endpoint,
			IZ_UINT maxConnections);

		void loop();

	private:
		class Packet : public net::Packet, CPlacementNew {
		public:
			static Packet* create(IMemoryAllocator* allocator, IZ_UINT len);

			Packet()
			{
				listItem.Init(this);
			}
			~Packet() {}

			CStdList<Packet>::Item listItem;
		};

	private:
		IMemoryAllocator* m_allocator;

		IZ_SOCKET m_socket;

		CArray<TcpClient> m_clients;

		std::mutex m_recvDataLocker;
		CStdList<Packet> m_recvData;

		std::atomic<IZ_BOOL> m_isRunnning;
		sys::CThread m_thread;
	};
}	// namespace net
}	// namespace izanagi

#endif	// #if !defined(_IZANAGI_NETWORK_NETWORK_TCP_H__)
