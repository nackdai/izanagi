#include <thread>
#include <uv.h>
#include "network/Network.h"

namespace izanagi {
namespace net {
    std::atomic<IZ_UINT32> Network::s_refCnt = 0;

    static std::thread s_Th;

    // ネットワーク処理開始.
    void Network::begin()
    {
        uv_loop_init(uv_default_loop());
    }

    // ネットワーク処理終了.
    void Network::end()
    {
        if (s_refCnt > 0) {
            // TODO
            while (uv_loop_close(uv_default_loop()) != UV_EBUSY) {
            }
            s_Th.join();
            s_refCnt = 0;
        }
    }

    void Network::start()
    {
        if (s_refCnt == 0) {
            s_Th = std::thread([] {
                uv_run(uv_default_loop(), UV_RUN_DEFAULT);
            });

            s_refCnt++;
        }
    }
}    // namespace net
}    // namespace izanagi
