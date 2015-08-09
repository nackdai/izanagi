#if !defined(_IZANAGI_NETWORK_IP_V4_ENDPOINT_H__)
#define _IZANAGI_NETWORK_IP_V4_ENDPOINT_H__

#include "izDefs.h"
#include "network/NetworkDefs.h"

namespace izanagi {
namespace net {
	/**
	 */
	class IPv4Address {
		friend class IPv4Endpoint;

	public:
		IPv4Address();
		IPv4Address(IZ_UINT8 a, IZ_UINT8 b, IZ_UINT8 c, IZ_UINT8 d);
		IPv4Address(IZ_UINT32 ip);
		IPv4Address(const IZ_CHAR* ip);

		~IPv4Address();

		IPv4Address(const IPv4Address& rhs);

		const IPv4Address& operator=(const IPv4Address& rhs);

	public:
		void set(IZ_UINT8 a, IZ_UINT8 b, IZ_UINT8 c, IZ_UINT8 d);
		void set(IZ_UINT32 ip);
		void set(const IZ_CHAR* ip);

		IZ_BOOL setHost(const IZ_CHAR* host);

		IZ_UINT32 getValue() const;

		void toString(IZ_CHAR* ret, IZ_UINT size) const;

		IZ_BOOL operator==(const IPv4Address& rhs);

		IZ_BOOL isAny() const;

	private:
		union {
			IZ_UINT8 p[4];
			IZ_UINT32 value;
		} m_ip;
	};

	/**
	 */
	class IPv4Endpoint {
		friend class Tcp;

	public:
		IPv4Endpoint();
		IPv4Endpoint(
			const IPv4Address& address,
			IZ_UINT port);

		~IPv4Endpoint();

		IPv4Endpoint(const IPv4Endpoint& rhs);

		const IPv4Endpoint& operator=(const IPv4Endpoint& rhs);

	public:
		void set(
			const IPv4Address& address,
			IZ_UINT port);

		const IPv4Address& getAddress() const;

		IZ_UINT getPort() const;

		IZ_BOOL operator==(const IPv4Endpoint& rhs);

	private:
		void set(const sockaddr_in& addr);

	private:
		IPv4Address m_address;
		IZ_UINT m_port;
	};
}	// namespace net
}	// namespace izanagi

#endif	// #if !defined(_IZANAGI_NETWORK_IP_V4_ENDPOINT_H__)
