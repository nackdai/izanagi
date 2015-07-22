#if !defined(_IZANAGI_NETWORK_NETWORK_H__)
#define _IZANAGI_NETWORK_NETWORK_H__

#include "izDefs.h"

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

	/**
	 */
	class IPv4Address {
	public:
		IPv4Address();
		IPv4Address(IZ_UINT8 a, IZ_UINT8 b, IZ_UINT8 c, IZ_UINT8 d);
		IPv4Address(IZ_UINT32 ip);
		IPv4Address(const IZ_CHAR* ip);

		~IPv4Address();

		IPv4Address(const IPv4Address& rhs);

		const IPv4Address& operator=(const IPv4Address& rhs);

	public:
		IZ_UINT32 getValue() const;

		const IZ_CHAR* toString() const;

		IZ_BOOL operator==(const IPv4Address& rhs);

	private:
		union {
			struct {
				IZ_UINT8 a;
				IZ_UINT8 b;
				IZ_UINT8 c;
				IZ_UINT8 d;
			};
			IZ_UINT32 value;
		} m_ip;
	};

	/**
	 */
	class IPv4Endpoint {
	public:
		IPv4Endpoint();
		IPv4Endpoint(
			const IPv4Address& ip,
			IZ_UINT port);

		~IPv4Endpoint();

		IPv4Endpoint(const IPv4Endpoint& rhs);

		const IPv4Endpoint& operator=(const IPv4Endpoint& rhs);

	public:
		void set(
			const IPv4Address& ip,
			IZ_UINT port);

		const IPv4Endpoint& getAddress();

		IZ_UINT getPort() const;

		IZ_BOOL operator==(const IPv4Endpoint& rhs);

	private:
		IPv4Address m_address;
		IZ_UINT m_port;
	};
}	// namespace net
}	// namespace izanagi

#endif	// #if !defined(_IZANAGI_NETWORK_NETWORK_H__)
