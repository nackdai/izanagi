#include "network/IPv4Endpoint.h"

namespace izanagi {
namespace net {
    IPv4Address IPv4Address::Any;

    IPv4Address::IPv4Address()
    {
        m_ip.value = 0;
    }

    IPv4Address::IPv4Address(IZ_UINT8 a, IZ_UINT8 b, IZ_UINT8 c, IZ_UINT8 d)
        : IPv4Address()
    {
        set(a, b, c, d);
    }

    IPv4Address::IPv4Address(IZ_UINT32 ip)
        : IPv4Address()
    {
        set(ip);
    }

    IPv4Address::IPv4Address(const IZ_CHAR* host)
        : IPv4Address()
    {
        setByHostName(host);
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

    // IPv4を [a.b.c.d] と分解して設定.
    void IPv4Address::set(IZ_UINT8 a, IZ_UINT8 b, IZ_UINT8 c, IZ_UINT8 d)
    {
        m_ip.p[3] = a;
        m_ip.p[2] = b;
        m_ip.p[1] = c;
        m_ip.p[0] = d;
    }

    // IPv4アドレスを32bit整数値で設定.
    void IPv4Address::set(IZ_UINT32 ip)
    {
        m_ip.value = ip;
    }

    // IPv4アドレスを文字列（"WW.XX.YY.ZZ"）で設定.
    void IPv4Address::set(const IZ_CHAR* ip)
    {
        auto len = strlen(ip);

        IZ_UINT pos = COUNTOF(m_ip.p) - 1;

        for (IZ_UINT i = 0; i < len; i++) {
            if (ip[i] == '.') {
                pos--;
                IZ_ASSERT(pos >= 0);
                continue;
            }
            else if ('0' <= ip[i] && ip[i] <= '9') {
                auto n = ip[i] - '0';

                m_ip.p[pos] = m_ip.p[pos] * 10 + n;
            }
        }
    }

    // 指定されたホスト名からIPv4アドレスを自動で取得.
    IZ_BOOL IPv4Address::setByHostName(const IZ_CHAR* host)
    {
        // NOTE
        // Use getaddrinfo?
        // http://d.hatena.ne.jp/takehikom/20071009/1191880360

        auto server = gethostbyname(host);
        VRETURN(server != IZ_NULL);

        // TODO
        VRETURN(server->h_length == 4);

        IZ_UINT8* iptbl = (IZ_UINT8*)server->h_addr;

        for (IZ_INT i = 0; i < server->h_length; i++) {
            m_ip.p[i] = iptbl[i];
        }

        return IZ_TRUE;
    }

    IZ_BOOL IPv4Address::setByHostName()
    {
        IZ_CHAR host[128];

        IZ_INT err = gethostname(host, sizeof(host));
        VRETURN(err >= 0);

        return setByHostName(host);
    }

    // IPv4アドレスを32bit整数値として取得.
    IZ_UINT32 IPv4Address::getValue() const
    {
        return m_ip.value;
    }

    // IPv4アドレスを文字列として取得.
    void IPv4Address::toString(IZ_CHAR* ret, IZ_UINT size) const
    {
        IZ_SPRINTF(
            ret, size,
            "%d.%d.%d.%d\0",
            m_ip.p[0],
            m_ip.p[1],
            m_ip.p[2],
            m_ip.p[3]);
    }

    IZ_BOOL IPv4Address::operator==(const IPv4Address& rhs)
    {
        return (m_ip.value == rhs.m_ip.value);
    }

    IZ_BOOL IPv4Address::operator!=(const IPv4Address& rhs)
    {
        return !(*this == rhs);
    }

    // すべてのネットワークインターフェイスでクライアントによるネットワーク利用を待機する必要があることを示すアドレスかどうかを取得.
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

    // IPv4アドレスとポート番号を設定.
    void IPv4Endpoint::set(
        const IPv4Address& address,
        IZ_UINT port)
    {
        m_address = address;
        m_port = port;
    }

    // 設定されているIPv4アドレスを取得.
    const IPv4Address& IPv4Endpoint::getAddress() const
    {
        return m_address;
    }

    // 設定されているポート番号を取得.
    IZ_UINT IPv4Endpoint::getPort() const
    {
        return m_port;
    }

    IZ_BOOL IPv4Endpoint::operator==(const IPv4Endpoint& rhs)
    {
        return (m_address == rhs.m_address && m_port == rhs.m_port);
    }

    IZ_BOOL IPv4Endpoint::operator!=(const IPv4Endpoint& rhs)
    {
        return !(*this == rhs);
    }

    IZ_BOOL IPv4Endpoint::operator==(const sockaddr_in& addr)
    {
        IPv4Endpoint ep;
        ep.set(addr);

        return *this == ep;
    }

    void IPv4Endpoint::set(const sockaddr_in& addr)
    {
        m_port = ntohs(addr.sin_port);

        auto ip = getIp(addr);
        IZ_ASSERT(sizeof(ip) == 4 * sizeof(IZ_CHAR));

        IZ_CHAR* iptbl = (IZ_CHAR*)&ip;

        for (IZ_INT i = 0; i < COUNTOF(m_address.m_ip.p); i++) {
            m_address.m_ip.p[i] = iptbl[i];
        }
    }

    void IPv4Endpoint::get(sockaddr_in& addr)
    {
        addr.sin_family = AF_INET;
        addr.sin_port = htons(m_port);

        if (m_address.isAny()) {
            setIp(addr, htonl(INADDR_ANY));
        }
        else {
            IZ_CHAR ip[64];
            m_address.toString(ip, COUNTOF(ip));

            setIp(addr, inet_addr(ip));
        }
    }
}
}