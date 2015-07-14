#if !defined(_IZANAGI_NETWORK_NETWORK_TCP_H__)
#define _IZANAGI_NETWORK_NETWORK_TCP_H__

#include "izDefs.h"
#include "izSystem.h"

namespace izanagi {
namespace net {
	/**
	 */
	class Tcp {
	public:
		Tcp();
		~Tcp();

	public:
		IZ_BOOL start(
			IMemoryAllocator* allocator,
			const char* ip,
			IZ_UINT port);

		void stop();

	private:
		sys::CThread m_thread;
	};
}	// namespace net
}	// namespace izanagi

#endif	// #if !defined(_IZANAGI_NETWORK_NETWORK_TCP_H__)
