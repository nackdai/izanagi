#if !defined(_IZANAGI_NETWORK_NETWORK_TCP_H__)
#define _IZANAGI_NETWORK_NETWORK_TCP_H__

#include "izDefs.h"
#include "izSystem.h"
#include "network/NetworkDefs.h"
#include "network/NetworkPacket.h"

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

	////////////////////////////////////////////////////////

	/**
	 */
	class Tcp {
	public:
		Tcp();
		~Tcp();

	public:
		/** �T�[�o�[�Ƃ��ċN��.
		 */
		IZ_BOOL startAsServer(
			IMemoryAllocator* allocator,
			const IPv4Endpoint& endpoint,
			IZ_UINT maxConnections);

		/** �N���C�A���g�Ƃ��ċN��.
		 */
		IZ_BOOL startAsClient(
			IMemoryAllocator* allocator,
			const IPv4Endpoint& endpoint,
			IZ_BOOL isBlock);

		/** ��~.
		 */
		void stop();

		/** �T�[�o�[�Ƃ��ċN�����Ă���Ƃ��ɐڑ�����Ă��郊���[�g�̐����擾.
		 */
		IZ_UINT getRemoteNum() const;

		/** �T�[�o�[�Ƃ��ċN�����Ă���Ƃ��Ɏw�肳�ꂽ�ڑ�����Ă��郊���[�g�̏����擾.
		 */
		const IPv4Endpoint* getRemote(IZ_UINT idx) const;

		/** �N���C�A���g�����M�����f�[�^���擾.
		 */
		IZ_BOOL recieve(std::function<void(const net::Packet&)> func);

		IZ_BOOL sendData(const void* data, IZ_UINT size);

		IZ_BOOL sendData(
			const IPv4Endpoint& endpoint,
			const void* data, IZ_UINT size);

		IZ_UINT sendDataToAllRemote(const void* data, IZ_UINT size);

	private:
		IZ_BOOL connectServer();

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
		sys::CThread m_threadSub;
	};
}	// namespace net
}	// namespace izanagi

#endif	// #if !defined(_IZANAGI_NETWORK_NETWORK_TCP_H__)
