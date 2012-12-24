#if !defined(__IZANAGI_COMPRESS_IO_STREAM_UTIL_H__)
#define __IZANAGI_COMPRESS_IO_STREAM_UTIL_H__

#include "izStd.h"
#include "izIo.h"

namespace izanagi {
    class CIoStreamUtil {
    private:
        CIoStreamUtil();
        ~CIoStreamUtil();

        NO_COPIABLE(CIoStreamUtil);

    public:
        // １バイト入力
        static IZ_BYTE ReadByte(IInputStream* pInput)
        {
            return Read<IZ_BYTE>(pInput);
        }

        // ２バイト入力
        static IZ_UINT16 ReadByte2(IInputStream* pInput)
        {
            return Read<IZ_UINT16>(pInput);
        }

        // 出力
        template <typename _T>
        static IZ_BOOL Write(
            IOutputStream* pOutput,
            const _T& n)
        {
            IZ_BOOL ret = IZ_OUTPUT_WRITE(pOutput, &n, 0, sizeof(_T));
            return ret;
        }

        // １バイト出力
        static IZ_BOOL WriteByte(
            IOutputStream* pOutput,
            IZ_BYTE n)
        {
            return Write(pOutput, n);
        }

        // 入力可能かどうか
        static IZ_BOOL IsEnableInput(IInputStream* pInput)
        {
            IZ_ASSERT(pInput != IZ_NULL);
            return (pInput->GetCurPos() < pInput->GetSize());
        }

    private:
        template <typename _T>
        static _T Read(IInputStream* pInput)
        {
            _T ret = 0;
            IZ_BOOL result = IZ_INPUT_READ(pInput, &ret, 0, sizeof(_T));
            IZ_ASSERT(result);
            return ret;
        }
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COMPRESS_IO_STREAM_UTIL_H__)
