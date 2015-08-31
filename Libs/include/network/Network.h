#if !defined(_IZANAGI_NETWORK_NETWORK_H__)
#define _IZANAGI_NETWORK_NETWORK_H__

#include "izDefs.h"

// NOTE
// http://www.geekpage.jp/programming/winsock/select.php

namespace izanagi {
namespace net {
    class Network {
    private:
        Network();
        ~Network();

    public:
        /** ネットワーク処理開始
         * 全てのネットワーク処理を開始する前に必ず呼ぶこと.
         */
        static void begin();

        /** ネットワーク処理終了
         * 全てのネットワーク処理を終了した後に必ず呼ぶこと.
         */
        static void end();

    private:
        static IZ_UINT32 s_refCnt;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_H__)
