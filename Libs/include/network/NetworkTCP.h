#if !defined(_IZANAGI_NETWORK_NETWORK_TCP_H__)
#define _IZANAGI_NETWORK_NETWORK_TCP_H__

#include "izDefs.h"
#include "izSystem.h"
#include "NetworkDefs.h"
#include "network/NetworkPacket.h"

namespace izanagi {
namespace net {
	class IPv4Endpoint;

	// 
	class TcpClient : public CPlacementNew, sys::CSpinLock {
		friend class Tcp;

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

		IZ_INT send(const void* data, IZ_UINT size);

		IZ_INT recieve(void* data, IZ_UINT size);

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
			IPv4Endpoint endpoint,
			IZ_UINT maxConnections);

		void stop();

	private:
		IZ_BOOL startAsServer(
			IPv4Endpoint endpoint,
			IZ_UINT maxConnections);

		void loop();

	private:
		class Packet : public net::Packet, CPlacementNew {
		public:
			static Packet* create(IMemoryAllocator* allocator)
			{
				void* buf = ALLOC(allocator, sizeof(Packet));
				IZ_ASSERT(buf != nullptr);
				Packet* p = new(buf)Packet;
				return p;
			}

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

		CStdList<Packet> m_recvData;

		std::atomic<IZ_BOOL> m_isRunnning;
		sys::CThread m_thread;
	};
}	// namespace net
}	// namespace izanagi

#endif	// #if !defined(_IZANAGI_NETWORK_NETWORK_TCP_H__)
