#if !defined(__RANGE_CODER_ENCODER_H__)
#define __RANGE_CODER_ENCODER_H__

#include "CompressRangeCoderBase.h"
#include "izStd.h"

namespace izanagi {
    /**
    */
    class CCompressRangeCoderEncoder : public CCompressRangeCoderBase {
    public:
        struct SDesc {
            IZ_UINT org_size;           // 元のサイズ
            IZ_UINT compressed_size;    // 圧縮後のサイズ
        };

    public:
        CCompressRangeCoderEncoder()
        {
            m_pAllocator = IZ_NULL;

            m_pBuffer = IZ_NULL;
            m_nBufferSize = 0;

            m_bIsFirstOutByte = IZ_TRUE;
        }

        ~CCompressRangeCoderEncoder()
        {
            Release();
        }

        NO_COPIABLE(CCompressRangeCoderEncoder);

    public:
        // 初期化
        IZ_BOOL Init(
            IMemoryAllocator* pAllocator,
            IZ_UINT nBufferSize);
        IZ_BOOL InitEx(
            void* pBuf,
            IZ_UINT nBufferSize);

        // 符号化
        IZ_BOOL Encode(
            SDesc* pDesc,
            IInputStream* pInput,
            IOutputStream* pOutput);

        IZ_BOOL Encode(
            SDesc* pDesc,
            IZ_UINT* pCntTbl,
            IZ_UINT nTblSize,
            IInputStream* pInput,
            IOutputStream* pOutput);

    private:
        // フラッシュ
        IZ_BOOL Flush(IOutputStream* pOutput);

        // 符号化
        IZ_BOOL Encode(
            IZ_UINT nTotal,
            IZ_UINT nLow,
            IZ_UINT nRange,
            IOutputStream* pOutput);

        // Low と Range の正規化
        IZ_BOOL Normalize(IOutputStream* pOutput);

        // バッファ解放
        void Release();

        IZ_BOOL IsInitialized()
        {
            return ((m_pBuffer != IZ_NULL) && (m_nBufferSize > 0));
        }

        template <typename _T>
        _T GetValueFromBuffer(IZ_UINT pos)
        {
            _T* ret = reinterpret_cast<_T*>(&m_pBuffer[pos]);
            return *ret;
        }

    private:
        enum {
            DEFAULT_BUF_SIZE = 1024,
        };

        IMemoryAllocator* m_pAllocator;

        IZ_BYTE* m_pBuffer;
        IZ_UINT m_nBufferSize;

        IZ_BOOL m_bIsFirstOutByte;
    };
}   // namespace izanagi

#endif  // #if !defined(__RANGE_CODER_ENCODER_H__)
