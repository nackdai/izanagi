#include "network/IPv4Endpoint.h"

namespace izanagi {
namespace net {
	IPv4Address::IPv4Address()
	{
		m_ip.value = 0;
	}

	IPv4Address::IPv4Address(IZ_UINT8 a, IZ_UINT8 b, IZ_UINT8 c, IZ_UINT8 d)
	{
		set(a, b, c, d);
	}

	IPv4Address::IPv4Address(IZ_UINT32 ip)
	{
		set(ip);
	}

	IPv4Address::IPv4Address(const IZ_CHAR* ip)
	{
		set(ip);
	}

	IPv4Address::IPv4Address(const IPv4Address& rhs)
	{
		*this = rhs;
	}

	IPv4Address::~IPv4Address()
	{
	}

	const IPv4Address& IPv4Address::operator = (const IPv4Address& rhs)
	{
		m_ip.value = rhs.m_ip.value;
		return *this;
	}

	void IPv4Address::set(IZ_UINT8 a, IZ_UINT8 b, IZ_UINT8 c, IZ_UINT8 d)
	{
		m_ip.p[0] = a;
		m_ip.p[1] = b;
		m_ip.p[2] = c;
		m_ip.p[3] = d;
	}

	void IPv4Address::set(IZ_UINT32 ip)
	{
		m_ip.value = ip;
	}

	void IPv4Address::set(const IZ_CHAR* ip)
	{
		auto len = strlen(ip);

		IZ_UINT pos = 0;

		for (IZ_UINT i = 0; i < len; i++) {
			if (ip[i] == ':') {
				pos++;
				continue;
			}
			else if ('0' <= ip[i] && ip[i] <= '9') {
				auto n = ip[i] - '0';

				m_ip.p[pos] = m_ip.p[pos] * 10 + n;
			}
		}
	}

	IZ_UINT32 IPv4Address::getValue() const
	{
		return m_ip.value;
	}

	void IPv4Address::toString(IZ_CHAR* ret, IZ_UINT size) const
	{
		IZ_SPRINTF(
			ret, size,
			"%d:%d:%d:%d\0",
			m_ip.p[0],
			m_ip.p[1],
			m_ip.p[2],
			m_ip.p[3]);
	}

	IZ_BOOL IPv4Address::operator==(const IPv4Address& rhs)
	{
		return (m_ip.value == rhs.m_ip.value);
	}

	IZ_BOOL IPv4Address::isAny() const
	{
		return (m_ip.value == 0);
	}

	//////////////////////////////////////////////////////////////////

	IPv4Endpoint::IPv4Endpoint()
	{

	}

	IPv4Endpoint::IPv4Endpoint(
		const IPv4Address& address,
		IZ_UINT port)
	{
		set(address, port);
	}

	IPv4Endpoint::~IPv4Endpoint()
	{
	}

	IPv4Endpoint::IPv4Endpoint(const IPv4Endpoint& rhs)
	{
		*this = rhs;
	}

	const IPv4Endpoint& IPv4Endpoint::operator = (const IPv4Endpoint& rhs)
	{
		m_address = rhs.m_address;
		m_port = rhs.m_port;
		return *this;
	}

	void IPv4Endpoint::set(
		const IPv4Address& address,
		IZ_UINT port)
	{
		m_address = address;
		m_port = port;
	}

	void IPv4Endpoint::set(const sockaddr_in& addr)
	{
		// TODO
		IZ_ASSERT(IZ_FALSE);
	}

	const IPv4Address& IPv4Endpoint::getAddress() const
	{
		return m_address;
	}

	IZ_UINT IPv4Endpoint::getPort() const
	{
		return m_port;
	}

	IZ_BOOL IPv4Endpoint::operator==(const IPv4Endpoint& rhs)
	{
		return (m_address == rhs.m_address && m_port == rhs.m_port);
	}
}
}