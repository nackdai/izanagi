#if !defined(__IZANAGI_STD_STD_BIT_STREAM_H__)
#define __IZANAGI_STD_STD_BIT_STREAM_H__

#include "izDefs.h"

namespace izanagi {
    /**
    */
    class CStdBitInput {
    public:
        CStdBitInput(void* src, size_t size);
        ~CStdBitInput() {}

        NO_COPIABLE(CStdBitInput);

    public:
        /** Read bits.
         */
        IZ_UINT Read(IZ_UINT bitCnt);

        /** Return read bytes count.
         */
        IZ_UINT GetReadBytes() const;

        /** Return whether this is valid.
         */
        IZ_BOOL IsValid() const;

    private:
        // Source data.
        IZ_BYTE* m_Src;

        // Source data size.
        IZ_UINT m_Size;

        // Current bit position.
        IZ_UINT m_CurBitPos;

        // Read bytes.
        IZ_UINT m_ReadBytes;

        // Current 1byte(= 8bit) data.
        IZ_BYTE m_CurData;
    };
    

    /////////////////////////////////////////////////////////

    /**
    */
    class CStdBitOutput {
    public:
        CStdBitOutput(void* dst, size_t size);
        CStdBitOutput(size_t size);
        ~CStdBitOutput();

        NO_COPIABLE(CStdBitOutput);

    public:
        /** Write bits.
         */
        IZ_BOOL Write(IZ_UINT val, IZ_UINT count);

        /** Flush buffering data.
         */
        IZ_BOOL Flush();

        /** Return destination buffer.
         */
        const void* GetDst();

        /** Return wrote bytes count.
         */
        IZ_UINT GetWriteBytes() const;

        /** Return whether this is valid.
         */
        IZ_BOOL IsValid() const;

    private:
        // Destination buffer.
        IZ_BYTE* m_Dst;

        // Destination buffer size.
        IZ_UINT m_Size;

        // Whether destination buffer is allocated.
        IZ_BOOL m_IsAllocated;

        // Current bit position.
        IZ_UINT m_CurBitPos;

        // Wrote bytes.
        IZ_UINT m_WriteBytes;

        // Current 1byte(= 8bit) data.
        IZ_BYTE m_CurData;
    };    
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_STD_BIT_STREAM_H__)
