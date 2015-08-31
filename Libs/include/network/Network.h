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
        /** �l�b�g���[�N�����J�n
         * �S�Ẵl�b�g���[�N�������J�n����O�ɕK���ĂԂ���.
         */
        static void begin();

        /** �l�b�g���[�N�����I��
         * �S�Ẵl�b�g���[�N�������I��������ɕK���ĂԂ���.
         */
        static void end();

    private:
        static IZ_UINT32 s_refCnt;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_H__)
