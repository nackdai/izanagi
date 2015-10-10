#if !defined(_IZANAGI_NETWORK_NETWORK_TCP_LIBUV_H__)
#define _IZANAGI_NETWORK_NETWORK_TCP_LIBUV_H__

#include <atomic>
#include "izDefs.h"
#include "izSystem.h"
#include "network/NetworkDefs.h"
#include "network/NetworkPacket.h"
#include "network/rudp/CallbackRegister.h"

namespace izanagi {
namespace net {
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

        virtual IZ_BOOL stop();

        IZ_INT recieve(
            void* buf,
            IZ_UINT size);

#if 0
        /** データを送信.
         */
        IZ_BOOL sendData(
            const void* data,
            IZ_UINT size);
#else
		IZ_INT sendData(
			const void* data,
			IZ_UINT size);
#endif

		IZ_BOOL isConnected() const;

		IZ_BOOL isConnecting() const;

		IZ_BOOL isClosing() const;

		IZ_BOOL isRunning() const;

    private:
        void startRecieve();

        void OnConnect(uv_connect_t *req, int status);
        void OnAlloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
        void OnReadEnd(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
		void OnWriteEnd(uv_write_t* req, int status);
		void OnClosed(uv_handle_t* handle);

		inline void SetConnected();
		inline void SetConnecting();

    private:
        IMemoryAllocator* m_allocator{ nullptr };

        uv_tcp_t m_client;
        uv_stream_t* m_handle{ nullptr };

        // TCP コネクション用の構造体.
        uv_connect_t m_reqConnect;

        // 書き込みリクエスト.
        uv_write_t m_reqWrite;

		enum State : IZ_UINT
		{
			Closed = 1 << 0,
			WillClose = 1 << 1,

			Connecting = 1 << 2,
			Connected = 1 << 3,

			Running = 1 << 16,
		};

		std::atomic<State> m_State{ State::Closed };
        std::atomic<IZ_BOOL> m_isReading{ IZ_FALSE };

        using CallbackConnected = std::function < void(uv_connect_t *req, int status) >;
        Callback<CallbackConnected> m_cbConnected;

        using CallbackOnAlloc = std::function < void(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) > ;
        Callback<CallbackOnAlloc> m_cbAllocated;

        using CallbackOnReadEnd = std::function < void(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) > ;
        Callback<CallbackOnReadEnd> m_cbReadEnd;

		using CallbackOnWriteEnd = std::function < void(uv_write_t* req, int status) >;
		Callback<CallbackOnWriteEnd> m_cbWriteEnd;

		using CallbackOnClosed = std::function < void(uv_handle_t* handle) >;
		Callback<CallbackOnClosed> m_cbClosed;
            
        std::function<void(IZ_BOOL)> m_onConnected{ nullptr };

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

		IZ_BOOL stop();

        TcpClient* acceptRemote();

		IZ_BOOL isRunning() const;
        
		IZ_BOOL isClosing() const;

    private:
        void OnConnection(uv_stream_t* server, int status);
		void OnClosed(uv_handle_t* handle);

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

            virtual IZ_BOOL stop() override
            {
                IZ_BOOL ret = TcpClient::stop();
                m_listItem.Leave();
				return ret;
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

		using CallbackOnClosed = std::function < void(uv_handle_t* handle) >;
		Callback<CallbackOnClosed> m_cbClosed;

		enum State : IZ_UINT
		{
			Closed,
			Listening,
			WillClose,
		};

		std::atomic<State> m_State{ State::Closed };
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_TCP_LIBUV_H__)
