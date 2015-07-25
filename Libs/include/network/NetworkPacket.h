#if !defined(_IZANAGI_NETWORK_NETWORK_PACKET_H__)
#define _IZANAGI_NETWORK_NETWORK_PACKET_H__

#include "izDefs.h"
#include "izSystem.h"
#include "network/IPv4Endpoint.h"

namespace izanagi {
namespace net {
	/**
	 */
	struct Packet {
		IPv4Endpoint endpoint;

		IZ_UINT size;
		IZ_CHAR* data;
	};
}	// namespace net
}	// namespace izanagi

#endif	// #if !defined(_IZANAGI_NETWORK_NETWORK_PACKET_H__)
