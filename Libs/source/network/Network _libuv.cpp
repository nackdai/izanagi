#include <thread>
#include <uv.h>
#include "network/Network.h"

namespace izanagi {
namespace net {
    std::atomic<IZ_UINT32> Network::s_refCnt = 0;

    static std::thread s_Th;

	static uv_idle_t idler;

	static void idle_cb(uv_idle_t *handle) {
	}

    // ネットワーク処理開始.
    void Network::begin()
    {
        uv_loop_init(uv_default_loop());
    }

    // ネットワーク処理終了.
    void Network::end()
    {
        if (s_refCnt > 0) {
			uv_close((uv_handle_t*)&idler, NULL);

			s_Th.join();

			// NOTE
			// Maybe, call closing handle functions forcibly.
			uv_run(uv_default_loop(), UV_RUN_DEFAULT);

			// close libuv internal parameters.
            while (uv_loop_close(uv_default_loop()) == UV_EBUSY) {
            }

            s_refCnt = 0;
        }
    }

    void Network::start()
    {
        if (s_refCnt == 0) {
			// TODO
			// uv_runの中でpollでブロックされないようにするため.
			uv_idle_init(uv_default_loop(), &idler);
			uv_idle_start(&idler, idle_cb);

            s_Th = std::thread([] {
                uv_run(uv_default_loop(), UV_RUN_DEFAULT);
            });

            s_refCnt++;
        }
    }
}    // namespace net
}    // namespace izanagi
