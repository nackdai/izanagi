#include <winsock2.h>

#include "network/Network.h"

namespace izanagi {
namespace net {
	IZ_UINT32 Network::s_refCnt = 0;

	void Network::begin()
	{
		if (s_refCnt == 0) {
			WSADATA wsData;
			if (::WSAStartup(MAKEWORD(2, 2), &wsData) != 0) {
				// TODO
				return;
			}

			s_refCnt++;
		}
	}

	void Network::end()
	{
		if (s_refCnt > 0) {
			::WSACleanup();
			s_refCnt--;
		}
	}
}	// namespace net
}	// namespace izanagi
