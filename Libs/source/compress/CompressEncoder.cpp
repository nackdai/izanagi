#ifdef IZ_ENABLE_COMPRESS_ENCODER

#include "compress/Compressor.h"
#include "compress/CMPFormat.h"
#include "std/allocator/MemoryAllocator.h"
#include "math/MathUtil.h"

#include "io/stream/InputStream.h"
#include "io/stream/MemoryInputStream.h"
#include "io/stream/OutputStream.h"
#include "io/stream/MemoryOutputStream.h"

#include "compress/CompressSlideDictionaryEncoder.h"
#include "compress/CompressRangeCoderEncoder.h"

using namespace izanagi;

//#define ENABLE_ALLOC_MEMORY

////////////////////////////////////////////////////////////////////

class CCompressor::CEncoderOutputStreamProxy : public IOutputStream {
public:
    CEncoderOutputStreamProxy(
        void* buf,
        size_t size,
        IOutputStream* pOut);

    ~CEncoderOutputStreamProxy() { Finalize(); }

public:
    void Finalize();

    // 出力
    IZ_UINT Write(const void* buf, IZ_UINT offset, size_t size);

    // サイズ取得
    IZ_UINT GetSize();

    // 現在位置取得
    IZ_UINT GetCurPos();

    // シーク
    IZ_BOOL Seek(IZ_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos);

    IZ_BOOL IsValid();

    IZ_BOOL IsEnableOutput();

    IZ_BOOL Flush();
    
    IZ_UINT GetCompressedSize() const { return m_nCompressedSize; }

protected:
    CMemoryOutputStream m_Tmp;
    IOutputStream* m_pBody;

    IZ_UINT m_nCompressedSize;
};

CCompressor::CEncoderOutputStreamProxy::CEncoderOutputStreamProxy(
    void* buf,
    size_t size,
    IOutputStream* pOut)
{
    m_Tmp.Init(buf, size);
    m_pBody = pOut;

    m_nCompressedSize = 0;
}

void CCompressor::CEncoderOutputStreamProxy::Finalize()
{
    IZ_UINT nTmpRestSize = m_Tmp.GetCurPos();
    if (nTmpRestSize > 0) {
        Flush();
    }
}

// 出力
IZ_UINT CCompressor::CEncoderOutputStreamProxy::Write(const void* buf, IZ_UINT offset, size_t size)
{
    IZ_ASSERT(m_Tmp.IsValid());

    IZ_UINT nPos = m_Tmp.GetCurPos();
    IZ_UINT nSize = m_Tmp.GetSize();

    if (nPos + size > nSize) {
        Flush();
    }

    IZ_OUTPUT_WRITE_ASSERT(&m_Tmp, buf, 0, size);

    return static_cast<IZ_UINT>(size);
}

// サイズ取得
IZ_UINT CCompressor::CEncoderOutputStreamProxy::GetSize()
{
    // Nothing is done.
    IZ_ASSERT(IZ_FALSE);
    return 0;
}

// 現在位置取得
IZ_UINT CCompressor::CEncoderOutputStreamProxy::GetCurPos()
{
    // Nothing is done.
    IZ_ASSERT(IZ_FALSE);
    return 0;
}

// シーク
IZ_BOOL CCompressor::CEncoderOutputStreamProxy::Seek(IZ_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos)
{
    // Nothing is done.
    IZ_ASSERT(IZ_FALSE);
    return IZ_FALSE;
}

IZ_BOOL CCompressor::CEncoderOutputStreamProxy::IsValid()
{
    // Nothing is done.
    IZ_ASSERT(IZ_FALSE);
    return IZ_FALSE;
}

IZ_BOOL CCompressor::CEncoderOutputStreamProxy::IsEnableOutput()
{
    // Nothing is done.
    IZ_ASSERT(IZ_FALSE);
    return IZ_FALSE;
}

IZ_BOOL CCompressor::CEncoderOutputStreamProxy::Flush()
{
    IZ_ASSERT(m_Tmp.IsValid());
    IZ_ASSERT(m_pBody != IZ_NULL);
    
    static const IZ_UINT BUF_SIZE = 4096;
    static IZ_BYTE RC_BUF[BUF_SIZE];

    CCompressRangeCoderEncoder cRangeCoderEncoder;

    // Initialize RangeCoder.
    VRETURN(cRangeCoderEncoder.InitEx(RC_BUF, sizeof(RC_BUF)));

    CMemoryInputStream cIn;
    cIn.Init(
        m_Tmp.GetBuffer(),
        m_Tmp.GetSize());

    CCompressRangeCoderEncoder::SDesc sDesc;

    // Encode by RangeCoder.
    VRETURN(cRangeCoderEncoder.Encode(
                &sDesc,
                &cIn,
                m_pBody));

    m_nCompressedSize += sDesc.compressed_size;

    m_Tmp.Reset();

    return IZ_TRUE;
}

////////////////////////////////////////////////////////////////////

#define BUF_SIZE    (IZ_CMP_MAX_BLOCK_SIZE << 1)

static IZ_BYTE IN_TMP_BUF[BUF_SIZE];
static IZ_BYTE OUT_TMP_BUF[BUF_SIZE];

static IZ_BYTE PROXY_TMP_BUF[BUF_SIZE];

CCompressor CCompressor::s_cInstance;

/**
*/
IZ_BOOL CCompressor::Encode(
    IInputStream* pIn,
    IOutputStream* pOut,
    IMemoryAllocator* pAllocator,
    IZ_UINT nBlockSize,
    IZ_FLOAT fThreshold)
{
    IZ_ASSERT(pIn != IZ_NULL);
    IZ_ASSERT(pOut != IZ_NULL);
    IZ_ASSERT(pAllocator != IZ_NULL);

    m_nOrgSize = pIn->GetSize();
    m_nCmpSize = 0;

    IZ_BOOL ret = IZ_FALSE;

    // Initialize file's header.
    S_CMP_HEADER sHeader;
    {
        FILL_ZERO(&sHeader, sizeof(sHeader));

        sHeader.magic = CMP_MAGIC_NUMBER;
        sHeader.version = E_CMP_VERSION_CURRENT;
        sHeader.sizeHeader = sizeof(sHeader);
    }

    // Compute block size.
    nBlockSize = (nBlockSize > 0 ? nBlockSize : IZ_CMP_MAX_BLOCK_SIZE);
    nBlockSize = IZ_MIN(nBlockSize, IZ_CMP_MAX_BLOCK_SIZE);

    // Clamp threshold.
    fThreshold = CMath::Clamp(fThreshold, 0.1f, 1.0f);

    // Get current output stream position.
    IZ_UINT nCurOutPos = pOut->GetCurPos();

    // Create space for CMP file's header.
    VRETURN(pOut->Seek(sizeof(sHeader), E_IO_STREAM_SEEK_POS_CUR));

#ifdef ENABLE_ALLOC_MEMORY
    // Allocate temporary buffer.
    m_pInBuf = reinterpret_cast<IZ_BYTE*>(ALLOC_ZERO(pAllocator, BUF_SIZE));
    VRETURN(m_pInBuf != IZ_NULL);

    m_pOutBuf = reinterpret_cast<IZ_BYTE*>(ALLOC_ZERO(pAllocator, BUF_SIZE));
    VRETURN(m_pOutBuf != IZ_NULL);
#else   // #ifdef ENABLE_ALLOC_MEMORY
    m_pInBuf = IN_TMP_BUF;
    m_pOutBuf = OUT_TMP_BUF;
#endif  // #ifdef ENABLE_ALLOC_MEMORY

    // Alocator temporary buffer for SlideDictionaryEncoder.
    void* pSlideDicEncoderBuf = ALLOC_ZERO(pAllocator, nBlockSize << 1);
    VRETURN(pSlideDicEncoderBuf != IZ_NULL);

    // TODO
    IZ_UINT nInputSize = static_cast<IZ_UINT>(pIn->GetSize());

    for (IZ_UINT nCnt = 0; nInputSize >= nBlockSize; nInputSize -= nBlockSize, ++nCnt) {
        // Encode
        ret = EncodeBlock(
                pIn,
                pOut,
                nBlockSize,
                fThreshold,
                pSlideDicEncoderBuf);
        VGOTO(ret, __EXIT__);

        sHeader.sizeOrg += nBlockSize;
        ++(sHeader.numBlock);
    }

    if (nInputSize > 0) {
        // If encoding does not finish...
        ret = EncodeBlock(
                pIn,
                pOut,
                nInputSize,
                fThreshold,
                pSlideDicEncoderBuf);
        VGOTO(ret, __EXIT__);

        sHeader.sizeOrg += nInputSize;
        ++(sHeader.numBlock);
    }

    // Get output size.
    sHeader.sizeFile = pOut->GetCurPos() - nCurOutPos;

    // Seek output stream position.
    VRETURN(pOut->Seek(nCurOutPos, E_IO_STREAM_SEEK_POS_START));

    // Write CMP file's header.
    VGOTO(
        IZ_OUTPUT_WRITE(pOut, &sHeader, 0, sizeof(sHeader)),
        __EXIT__);

    m_nCmpSize += sizeof(sHeader);

__EXIT__:
    m_cOutTmp.Finalize();

#ifdef ENABLE_ALLOC_MEMORY
    // Free temorary buffer
    FREE(pAllocator, m_pInBuf);
    FREE(pAllocator, m_pOutBuf);
#endif

    m_nCmpSize = (ret ? m_nCmpSize : m_nOrgSize);

    return ret;
}

IZ_BOOL CCompressor::EncodeBlock(
    IInputStream* pIn,
    IOutputStream* pOut,
    IZ_UINT nBlockSize,
    IZ_FLOAT fThreshold,
    void* pSlideDicEncoderBuf)
{
    IZ_ASSERT(pIn != IZ_NULL);
    IZ_ASSERT(pOut != IZ_NULL);
    IZ_ASSERT(m_pInBuf != IZ_NULL);
    IZ_ASSERT(m_pOutBuf != IZ_NULL);
    IZ_ASSERT(pSlideDicEncoderBuf != IZ_NULL);

    // Read data.
    IZ_INPUT_READ_VRETURN(pIn, m_pInBuf, 0, nBlockSize);

    // Temporary output
    m_cInTmp.Init(m_pInBuf, nBlockSize);
    m_cOutTmp.Init(m_pOutBuf, BUF_SIZE);

    S_CMP_BLOCK_HEADER sBlockHeader;
    FILL_ZERO(&sBlockHeader, sizeof(sBlockHeader));

    // Encode
    IZ_UINT nCompressedSize = 0;
    {
        // Initialize Slide Dictionary Encoder.
        CCompressSlideDictionaryEncoder cSlideDicEncoder;
        VRETURN(cSlideDicEncoder.InitEx(
                    pSlideDicEncoderBuf,
                    nBlockSize << 1,
                    IZ_CMP_MAX_MATCH_LEN));

        // Initialize proxy for output stream.
        CEncoderOutputStreamProxy cEncoderOutputStreamProxy(
            PROXY_TMP_BUF,
            sizeof(PROXY_TMP_BUF),
            &m_cOutTmp);

        cSlideDicEncoder.Encode(
            IZ_NULL,
            &m_cInTmp,
            &cEncoderOutputStreamProxy);

        VRETURN(cEncoderOutputStreamProxy.Flush());

        // Get compressed size.
        nCompressedSize = cEncoderOutputStreamProxy.GetCompressedSize();
    }
    VRETURN(nCompressedSize > 0);

    // Export block's header.
    {
        sBlockHeader.sizeCompressed = nCompressedSize;
        sBlockHeader.sizeOrg = nBlockSize;
        sBlockHeader.sizeDictMatchLen = IZ_CMP_MAX_MATCH_LEN;
    }
    IZ_OUTPUT_WRITE_VRETURN(pOut, &sBlockHeader, 0, sizeof(sBlockHeader));

    // Export data.
    {
        void* pBuf = IZ_NULL;
        if (nCompressedSize == nBlockSize) {
            // Export raw data
            pBuf = m_pInBuf;
        }
        else {
            // Export compressed data
            pBuf = m_cOutTmp.GetBuffer();
        }
        IZ_OUTPUT_WRITE_VRETURN(pOut, pBuf, 0, nCompressedSize);
    }

    m_nCmpSize += nCompressedSize + sizeof(S_CMP_BLOCK_HEADER);

    m_cOutTmp.Finalize();

    return IZ_TRUE;
}

IZ_FLOAT CCompressor::GetCompressRatio() const
{
    IZ_ASSERT(m_nOrgSize > 0);
    IZ_FLOAT ret = (IZ_FLOAT)m_nCmpSize / m_nOrgSize;
    return ret;
}
#else   // #ifdef IZ_ENABLE_COMPRESS_ENCODER
IZ_BOOL CCompressor::Encode(
    IInputStream* pIn,
    IOutputStream* pOut,
    IMemoryAllocator* pAllocator,
    IZ_UINT nBlockSize,
    IZ_FLOAT fThreshold)
{
    // Nothing is done.
    IZ_ASSERT(IZ_FALSE);
    return IZ_FALSE;
}

IZ_FLOAT CCompressor::GetCompressRatio() const
{
    return 1.0f;
}
#endif  // #ifdef IZ_ENABLE_COMPRESS_ENCODER
