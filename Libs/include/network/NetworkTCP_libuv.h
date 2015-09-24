#if !defined(_IZANAGI_NETWORK_NETWORK_TCP_LIBUV_H__)
#define _IZANAGI_NETWORK_NETWORK_TCP_LIBUV_H__

#include <atomic>
#include "izDefs.h"
#include "izSystem.h"
#include "network/NetworkDefs.h"
#include "network/NetworkPacket.h"
#include "network/CallbackRegister.h"

namespace izanagi {
namespace net {
    template <typename Type>
    class NetworkTask {
    private:
        NetworkTask();
        ~NetworkTask();

    public:
        void Wait()
        {
            while (IZ_TRUE)
        }

        IZ_BOOL IsFinished() const
        {
            return m_hasResult;
        }

        const Type GetResult()
        {
            return m_Result;
        }

    private:
        std::atomic<IZ_BOOL> m_hasResult{ IZ_FALSE };
        Type m_Result;
    };

    /**
     */
    class TcpClient {
        friend class TcpListener;

    public:
        TcpClient();
        virtual ~TcpClient();

    public:
        /** 起動.
         */
        IZ_BOOL start(IMemoryAllocator* allocator);

        /** 接続.
         */
        IZ_BOOL connectTo(
            const IPv4Endpoint& remoteEp,
            std::function<void(IZ_BOOL)> onConnected);

        virtual void stop();

        IZ_INT recieve(
            void* buf,
            IZ_UINT size);

        /** データを送信.
         */
        IZ_BOOL sendData(
            const void* data,
            IZ_UINT size,
            std::function<void(IZ_BOOL)> onSent);

        inline IZ_BOOL IsConnected() const
        {
            return m_isConnected;
        }

        inline IZ_BOOL IsConnecting() const
        {
            return m_isConnecting;
        }

        inline IZ_BOOL IsSending() const
        {
            return m_isWriting;
        }

    private:
        void startRecieve();

        void OnConnect(uv_connect_t *req, int status);
        void OnWriteEnd(uv_write_t *req, int status);
        void OnAlloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
        void OnReadEnd(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);

    private:
        IMemoryAllocator* m_allocator{ nullptr };

        uv_tcp_t m_client;
        uv_stream_t* m_handle{ nullptr };

        // TCP コネクション用の構造体.
        uv_connect_t m_reqConnect;

        // 書き込みリクエスト.
        uv_write_t m_reqWrite;

        std::atomic<IZ_BOOL> m_isConnected{ IZ_FALSE };
        std::atomic<IZ_BOOL> m_isConnecting{ IZ_FALSE };
        std::atomic<IZ_BOOL> m_isWriting{ IZ_FALSE };
        std::atomic<IZ_BOOL> m_isReading{ IZ_FALSE };

        using CallbackConnected = std::function < void(uv_connect_t *req, int status) >;
        Callback<CallbackConnected> m_cbConnected;

        using CallbackOnWriteEnd = std::function < void(uv_write_t *req, int status) >;
        Callback<CallbackOnWriteEnd> m_cbWriteEnd;

        using CallbackOnAlloc = std::function < void(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) > ;
        Callback<CallbackOnAlloc> m_cbAllocated;

        using CallbackOnReadEnd = std::function < void(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) > ;
        Callback<CallbackOnReadEnd> m_cbReadEnd;
            
        std::function<void(IZ_BOOL)> m_onConnected{ nullptr };
        std::function<void(IZ_BOOL)> m_onSent{ nullptr };

        IPv4Endpoint m_remote;

        struct RecvData {
            CStdList<RecvData>::Item listItem;
            uv_buf_t* buf;

            IZ_INT length{ -1 };

            RecvData()
            {
                listItem.Init(this);
            }
            ~RecvData() {}

            IZ_DECL_PLACEMENT_NEW();
        };

        std::mutex m_listRecvDataLocker;
        CStdList<RecvData> m_listRecvData;
    };

    /**
     */
    class TcpListener {
        friend class TcpClient;

    public:
        TcpListener();
        virtual ~TcpListener();

    public:
        /** 起動.
         */
        IZ_BOOL start(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& hostEp,
            IZ_UINT maxConnections);

        void stop();

        TcpClient* acceptRemote();

        inline IZ_BOOL IsListening() const
        {
            return m_isListening;
        }

    private:
        void OnConnection(uv_stream_t* server, int status);

    private:
        class TcpRemote : public TcpClient {
            friend class TcpListener;

        public:
            TcpRemote()
            {
                m_listItem.Init(this);
            }
            virtual ~TcpRemote() {}

            CStdList<TcpRemote>::Item* getListItem()
            {
                return &m_listItem;
            }

            virtual void stop() override
            {
                TcpClient::stop();
                m_listItem.Leave();
            }

        private:
            CStdList<TcpRemote>::Item m_listItem;
        };

    private:
        IMemoryAllocator* m_allocator{ nullptr };

        IZ_UINT m_maxConnections{ 0 };

        uv_tcp_t m_server;

        std::mutex m_remotesLocker;
        CArray<TcpRemote> m_remotes;

        CStdList<TcpRemote> m_acceptedList;

        using CallbackOnConnection = std::function < void(uv_stream_t* server, int status) > ;
        Callback<CallbackOnConnection> m_cbConnection;

        std::atomic<IZ_BOOL> m_isListening{ IZ_FALSE };
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_TCP_LIBUV_H__)
