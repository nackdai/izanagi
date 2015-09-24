#if !defined(_IZANAGI_NETWORK_IP_V4_ENDPOINT_H__)
#define _IZANAGI_NETWORK_IP_V4_ENDPOINT_H__

#include "izDefs.h"
#include "network/NetworkDefs.h"

namespace izanagi {
namespace net {
    /** IPv4アドレス.
     */
    class IPv4Address {
        friend class IPv4Endpoint;

    public:
        /** サーバーがすべてのネットワークインターフェイスでクライアントによるネットワーク利用を待機する必要があることを示すアドレス.
         */
        static IPv4Address Any;

        IPv4Address();

        /** コンストラクタ.
         * IPv4アドレスを [a.b.c.d] と分解して設定.
         */
        IPv4Address(IZ_UINT8 a, IZ_UINT8 b, IZ_UINT8 c, IZ_UINT8 d);

        /** コンストラクタ.
         * IPv4アドレスを32bit整数値で設定.
         */
        IPv4Address(IZ_UINT32 ip);

        /** コンストラクタ.
         * ホスト名からIPv4アドレスを自動で設定.
         */
        IPv4Address(const IZ_CHAR* host);

        ~IPv4Address();

        IPv4Address(const IPv4Address& rhs);

        const IPv4Address& operator=(const IPv4Address& rhs);

    public:
        /** IPv4を [a.b.c.d] と分解して設定.
         */
        void set(IZ_UINT8 a, IZ_UINT8 b, IZ_UINT8 c, IZ_UINT8 d);

        /** IPv4アドレスを32bit整数値で設定.
         */
        void set(IZ_UINT32 ip);

        /** IPv4アドレスを文字列（"WW.XX.YY.ZZ"）で設定.
         */
        void set(const IZ_CHAR* ip);

        /** 指定されたホスト名からIPv4アドレスを自動で取得.
         */
        IZ_BOOL setByHostName(const IZ_CHAR* host);

        /** IPv4アドレスを32bit整数値として取得.
         * @return IPv4アドレスを32bit整数値であらわしたものを返す.
         */
        IZ_UINT32 getValue() const;

        /** IPv4アドレスを文字列として取得.
         * @param [out] ret 文字列格納先のバッファを示すポインタ.
         * @param [in] size 文字列格納先のバッファのサイズ（bytes）.
         */
        void toString(IZ_CHAR* ret, IZ_UINT size) const;

        /** 指定された IPv4Address と比較.
         * @param [in] rhs 比較したい IPv4Address.
         * @return 一致する場合は true、一致しない場合は false を返す.
         */
        IZ_BOOL operator==(const IPv4Address& rhs);

        /** 指定された IPv4Address と比較.
         * @param [in] rhs 比較したい IPv4Address.
         * @return 一致しない場合は true、一致する場合は false を返す.
         */
        IZ_BOOL operator!=(const IPv4Address& rhs);

        /** すべてのネットワークインターフェイスでクライアントによるネットワーク利用を待機する必要があることを示すアドレスかどうかを取得.
         * @return 待機する必要があるアドレスなら true、そうでなかれば false を返す.
         */
        IZ_BOOL isAny() const;

    private:
        IZ_BOOL setByHostName();

    private:
        union {
            IZ_UINT8 p[4];
            IZ_UINT32 value;
        } m_ip;
    };

    /** IPv4アドレスとポート番号でネットワークエンドポイントを表す.
     */
    class IPv4Endpoint {
    public:
        /** コンストラクタ
         * IPv4アドレスをAny、ポート番号を0で初期化.
         */
        IPv4Endpoint();

        /** コンストラクタ.
         * 指定されたIPv4アドレスとポート番号で初期化.
         */
        IPv4Endpoint(
            const IPv4Address& address,
            IZ_UINT port);

        ~IPv4Endpoint();

        IPv4Endpoint(const IPv4Endpoint& rhs);

        const IPv4Endpoint& operator=(const IPv4Endpoint& rhs);

    public:
        /** IPv4アドレスとポート番号を設定.
         * @param [in] address IPv4アドレス.
         * @paran [in] port ポート番号.
         */
        void set(
            const IPv4Address& address,
            IZ_UINT port);

        /** 設定されているIPv4アドレスを取得.
         * @return 設定されているIPv4アドレスを返す.
         */
        const IPv4Address& getAddress() const;

        /** 設定されているポート番号を取得.
         * @return 設定されているポート番号を返す.
         */
        IZ_UINT getPort() const;

        /** 指定された IPv4Endpoint と比較.
         * @param [in] rhs 比較したい IPv4Endpoint.
         * @return 一致する場合は true、一致しない場合は false を返す.
         */
        IZ_BOOL operator==(const IPv4Endpoint& rhs);

        /** 指定された IPv4Endpoint と比較.
         * @param [in] rhs 比較したい IPv4Address.
         * @return 一致しない場合は true、一致する場合は false を返す.
         */
        IZ_BOOL operator!=(const IPv4Endpoint& rhs);

        void set(const sockaddr_in& addr);
        void get(sockaddr_in& addr) const;
        IZ_BOOL operator==(const sockaddr_in& addr);

    private:
        IPv4Address m_address;
        IZ_UINT m_port;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_IP_V4_ENDPOINT_H__)
