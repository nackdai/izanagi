#if !defined(__IZANAGI_IO_OUTPUT_STREAM_H__)
#define __IZANAGI_IO_OUTPUT_STREAM_H__

#include "IODefs.h"

namespace izanagi {
    /**
    */
    class IOutputStream {
    public:
        IOutputStream() {}
        virtual ~IOutputStream() {}

    protected:
        IOutputStream(const IOutputStream& rhs);
        const IOutputStream& operator=(const IOutputStream& rhs);

    public:
        virtual void Finalize() = 0;

        // 出力
        virtual IZ_UINT Write(const void* buf, IZ_UINT offset, size_t size) = 0;

        // サイズ取得
        virtual IZ_UINT GetSize() = 0;

        // 現在位置取得
        virtual IZ_UINT GetCurPos() = 0;

        // シーク
        virtual IZ_BOOL Seek(IZ_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos) = 0;

        virtual IZ_BOOL IsValid() = 0;

        virtual IZ_BOOL IsEnableOutput() = 0;
    };
}   // namespace izanagi

// ユーティリティ
#ifndef IZ_OUTPUT_WRITE
    #define IZ_OUTPUT_WRITE(out, p, offset, size)           ((out)->Write((p), (offset), (size)) == (size))
#endif  // #ifndef IZ_INPUT_READ

#ifndef IZ_OUTPUT_WRITE_ASSERT
    #define IZ_OUTPUT_WRITE_ASSERT(out, p, offset, size)    if (!IZ_OUTPUT_WRITE(out, p, offset, size)) { IZ_ASSERT(IZ_FALSE); }
#endif  // #ifndef IZ_INPUT_READ

#ifndef IZ_OUTPUT_WRITE_VRETURN
    #define IZ_OUTPUT_WRITE_VRETURN(out, p, offset, size)   VRETURN(IZ_OUTPUT_WRITE(out, p, offset, size))
#endif  // #ifndef IZ_INPUT_READ

#ifndef IZ_OUTPUT_READ_VGOTO
    #define IZ_OUTPUT_READ_VGOTO(in, p, offset, size, label)    VGOTO(IZ_OUTPUT_READ(in, p, offset, size), lable)
#endif  // #ifndef IZ_OUTPUT_READ_VGOTO

#endif  // #if !defined(__IZANAGI_IO_OUTPUT_STREAM_H__)
