#if !defined(_IZANAGI_NETWORK_NETWORK_H__)
#define _IZANAGI_NETWORK_NETWORK_H__

#include "izDefs.h"

// NOTE
// http://www.geekpage.jp/programming/winsock/select.php

namespace izanagi {
namespace net {
	class Network {
	private:
		Network();
		~Network();

	public:
		static void begin();
		static void end();

	private:
		static IZ_UINT32 s_refCnt;
	};
}	// namespace net
}	// namespace izanagi

#endif	// #if !defined(_IZANAGI_NETWORK_NETWORK_H__)
