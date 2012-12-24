#include "compress/CompressSlideDictionaryEncoder.h"
#include "compress/CompressIoStreamUtil.h"

using namespace izanagi;

/**
* 初期化
*/
IZ_BOOL CCompressSlideDictionaryEncoder::Init(
    IMemoryAllocator* pAllocator,
    IZ_UINT nDictionarySize,
    IZ_UINT nMaxMatchLen)
{
    IZ_ASSERT(pAllocator != IZ_NULL);
    IZ_ASSERT(nDictionarySize > 1);
    IZ_ASSERT(nMaxMatchLen > 1);
    IZ_ASSERT(nDictionarySize * 2 > nMaxMatchLen);

    Clear();

    // 辞書
    IZ_BOOL ret = InitDictionary(pAllocator, nDictionarySize);
    VGOTO(ret, __EXIT__);

    m_nMaxMatchLen = nMaxMatchLen;

__EXIT__:
    return ret;
}

IZ_BOOL CCompressSlideDictionaryEncoder::InitEx(
    void* pBuf,
    IZ_UINT nDictionarySizeTwice,
    IZ_UINT nMaxMatchLen)
{
    IZ_ASSERT(nDictionarySizeTwice > 1);
    IZ_ASSERT(nMaxMatchLen > 1);
    IZ_ASSERT(nDictionarySizeTwice > nMaxMatchLen);

    Clear();

    m_pDic = reinterpret_cast<IZ_BYTE*>(pBuf);
    m_nDictionarySize = nDictionarySizeTwice;
    m_nMaxMatchLen = nMaxMatchLen;

    return IZ_TRUE;
}

/**
* 圧縮
*/
IZ_BOOL CCompressSlideDictionaryEncoder::Encode(
    SDesc* pDesc,
    IInputStream* pInput,
    IOutputStream* pOutput)
{
    IZ_ASSERT(m_pDic != IZ_NULL);
    IZ_ASSERT(pInput != IZ_NULL);
    IZ_ASSERT(pOutput != IZ_NULL);

    if (pDesc != IZ_NULL) {
        pDesc->org_size = pInput->GetSize();
        pDesc->compressed_size = pOutput->GetCurPos();
    }

    m_nPos = 0;
    m_nReadSize = 0;

    // 読み込み
    ReadData(pInput);

    SMatchInfo sNextMatch;

    Search(sNextMatch, 0);

    for (; m_nPos < m_nReadSize; ) {
        SMatchInfo sMatch = sNextMatch;

        // 探索
        Search(sNextMatch, 1);

        if ((sMatch.len < sNextMatch.len)   // 次が大きいなら次にする
            || (sMatch.len <= 2))           // ２文字以下ならそのまま出力
        {
            IZ_UINT16 n = m_pDic[m_nPos];

            VRETURN(CIoStreamUtil::Write(pOutput, n));

            Next(pInput);

            continue;
        }

        // TODO
        // SMatchInfo 出力
        {
            IZ_UINT16 nLen = sMatch.len + THRESHOLD_CODE;

            IZ_OUTPUT_WRITE_VRETURN(
                pOutput,
                &nLen,
                0,
                sizeof(nLen));
            
            IZ_OUTPUT_WRITE_VRETURN(
                pOutput,
                &sMatch.dist,
                0,
                sizeof(sMatch.dist));
        }

        // 処理位置をすすめる
        for (IZ_UINT i = 0; i < sMatch.len; ++i) {
            if (!Next(pInput)) {
                break;
            }
            Search(sNextMatch, 0);
        }
    }

    if (pDesc != IZ_NULL) {
        //pDesc->org_size = m_nReadSize;
        pDesc->compressed_size = pOutput->GetCurPos() - pDesc->compressed_size;
    }

    return IZ_TRUE;
}

// 処理位置を次の位置に移動
IZ_BOOL CCompressSlideDictionaryEncoder::Next(IInputStream* pInput)
{
    IZ_ASSERT(m_pDic != IZ_NULL);
    IZ_ASSERT(pInput != IZ_NULL);

    ++m_nPos;

    if (m_nPos < m_nDictionarySize * 2 - m_nMaxMatchLen) {
        // まだ辞書をスライドさせる必要がない
        return IZ_TRUE;
    }

    // 位置を前半分に移動
    m_nPos -= m_nDictionarySize;

    // 辞書をスライド
    // 後半分を前に持ってくる
    memcpy(&m_pDic[0], &m_pDic[m_nDictionarySize], m_nDictionarySize);

    // 読み込みサイズを減らす
    m_nReadSize = (m_nReadSize < m_nDictionarySize
                    ? 0
                    : m_nReadSize - m_nDictionarySize);

    // 空いた後半分に追加読み込み
    IZ_UINT nReadSize = ReadData(pInput);

    return (nReadSize > 0);
}

// データ読み込み
IZ_UINT CCompressSlideDictionaryEncoder::ReadData(IInputStream* pInput)
{
    IZ_ASSERT(pInput != IZ_NULL);

    if (CIoStreamUtil::IsEnableInput(pInput)) {
        IZ_BYTE* pBuf = &m_pDic[m_nReadSize];
        IZ_UINT nReadSize = m_nDictionarySize * 2 - m_nReadSize;

        IZ_UINT ret = pInput->Read(pBuf, 0, nReadSize);
        m_nReadSize += ret;

        return ret;
    }

    return 0;
}

// 辞書内を探す
void CCompressSlideDictionaryEncoder::Search(
    SMatchInfo& refMatchInfo,
    IZ_UINT nOffset)
{
    refMatchInfo.len = 0;
    refMatchInfo.dist = 0;

    IZ_UINT pos = m_nPos + nOffset;

    // どこまで探索するか
    const IZ_INT nMin = (pos < m_nDictionarySize
                            ? 0
                            : pos - m_nDictionarySize);

    for (IZ_INT i = pos - 1; i >= nMin; i--) {
        // 一致の可能性があるところを探す
        if (m_pDic[pos] == m_pDic[i]) {
            // 一致長
            IZ_UINT len = 1;

            // どのくらい一致するか
            for (; len < m_nMaxMatchLen && pos + len < m_nReadSize; ++len) {
                if (m_pDic[pos + len] != m_pDic[i + len]) {
                    break;
                }
            }

            // より長い一致
            if (refMatchInfo.len < len) {
                refMatchInfo.len = len;             // 一致長
                refMatchInfo.dist = pos - i;        // 相対距離
            }
        }
    }
}
