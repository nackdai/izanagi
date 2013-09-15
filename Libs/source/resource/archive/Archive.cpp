#include "resource/archive/ArchiveImpl.h"

using namespace izanagi;

/**
* インスタンス作成
*/
CArchive* CArchive::CreateArchive(
    IMemoryAllocator* pAllocator,
    IInputStream* pInput)
{
    IZ_ASSERT(pInput != IZ_NULL);

    CArchive* pInstance = IZ_NULL;

    // ファイルヘッダ読み込み
    S_ARC_HEADER sHeader;
    IZ_BOOL result = IZ_INPUT_READ(pInput, &sHeader, 0, sizeof(sHeader));
    VGOTO(result, __EXIT__);

    // マジックナンバーチェック
    result = IS_ARC_FORMAT(sHeader.magic);
    VGOTO(result, __EXIT__);

    pInstance = CreateArchive(
                    sHeader,
                    pAllocator,
                    pInput);

__EXIT__:
    if (!result) {
        if (pInstance != IZ_NULL) {
            SAFE_RELEASE(pInstance);
        }
    }

    return pInstance;
}

// インスタンス作成
CArchive* CArchive::CreateArchive(
    const S_ARC_HEADER& sHeader,
    IMemoryAllocator* pAllocator,
    IInputStream* pInput)
{
    IZ_ASSERT(pAllocator != IZ_NULL);
    IZ_ASSERT(pInput != IZ_NULL);

    IZ_BOOL result = IZ_FALSE;
    IZ_UINT8* pBuf = IZ_NULL;
    IZ_UINT8* pBufTop = IZ_NULL;
    CArchive* pInstance = IZ_NULL;

    IZ_UINT nFileHeaderNum = sHeader.numFiles;

    // 確保メモリサイズ
    size_t nSize = sizeof(CArchive);
    nSize += sHeader.sizeName;
    nSize += sHeader.sizeFileHeader;
    nSize += nFileHeaderNum * sizeof(CArcHashItem);

    // メモリ確保
    pBuf = (IZ_UINT8*)ALLOC_ZERO(pAllocator, nSize);
    result = (pBuf != IZ_NULL);
    VGOTO(result, __EXIT__);

    // 先頭位置保持
    pBufTop = pBuf;

    // インスタンス作成
    pInstance = new(pBuf) CArchive;
    {
        pBuf += sizeof(CArchive);

        pInstance->AddRef();

        pInstance->m_Allocator = pAllocator;
        pInstance->m_pInput = pInput;

        // ヘッダ
        memcpy(&pInstance->m_sHeader, &sHeader, sizeof(sHeader));

        // ファイル情報
        if (nFileHeaderNum > 0) {
            pInstance->m_pFileHeaders = reinterpret_cast<S_ARC_FILE_HEADER*>(pBuf);

            size_t size = nFileHeaderNum * sizeof(S_ARC_FILE_HEADER);
            result = IZ_INPUT_READ(
                        pInput, 
                        pInstance->m_pFileHeaders, 
                        0, size);
            VGOTO(result, __EXIT__);

            pBuf += size;
        }

        // 名前バッファ
        if (sHeader.sizeName > 0) {
            pInstance->m_pNameBuffer = pBuf;

            result = IZ_INPUT_READ(
                        pInput, 
                        pInstance->m_pNameBuffer, 
                        0, 
                        sHeader.sizeName);
            VGOTO(result, __EXIT__);

            pBuf += sHeader.sizeName;
        }

        // ハッシュ要素
        if (pInstance->m_pFileHeaders != IZ_NULL) {
            pInstance->m_pHashItems = reinterpret_cast<CArcHashItem*>(pBuf);

            for (IZ_UINT i = 0; i < nFileHeaderNum; ++i) {
                const S_ARC_FILE_HEADER& sFileHeader = *(pInstance->GetFileHeader(i));

                IZ_PCSTR name = pInstance->GetName(sFileHeader.posName);
                IZ_ASSERT(name != IZ_NULL);

                // Set data to hash item,
                pInstance->m_pHashItems[i].Init(
                    CKey(name), 
                    const_cast<S_ARC_FILE_HEADER*>(&sFileHeader));

                // Add hash item to hash list.
                pInstance->m_HashList.Add(&pInstance->m_pHashItems[i]);
            }
        }
    }

__EXIT__:
    if (!result) {
        if (pBufTop != IZ_NULL) {
            FREE(pAllocator, pBufTop);
        }
        else if (pInstance != IZ_NULL) {
            SAFE_RELEASE(pInstance);
        }
    }

    return pInstance;
}

// コンストラクタ
CArchive::CArchive()
{
    m_pInput = IZ_NULL;

    FILL_ZERO(&m_sHeader, sizeof(m_sHeader));

    m_pNameBuffer = IZ_NULL;
    m_pFileHeaders = IZ_NULL;

    m_pCurFileHeader = IZ_NULL;
}

// デストラクタ
CArchive::~CArchive()
{
    if (m_pInput != IZ_NULL) {
        m_pInput->Finalize();
        m_pInput = IZ_NULL;
    }
}

/**
* Seek file by key.
*/
IZ_BOOL CArchive::Seek(IZ_UINT nKey)
{
    IZ_ASSERT(m_pFileHeaders != IZ_NULL);

    // TODO
    IZ_UINT nIdx = nKey;
    IZ_ASSERT(nIdx < m_sHeader.numFiles);

    // Get file's header.
    const S_ARC_FILE_HEADER* pFileHeader = GetFileHeader(nIdx);
    VRETURN(pFileHeader != IZ_NULL);

    // Seek in stream.
    IZ_BOOL ret = SeekStream(pFileHeader->pos);

    if (ret) {
        m_pCurFileHeader = pFileHeader;
    }

    return IZ_TRUE;
}

namespace {
    // Return whether character is path separator.
    inline IZ_BOOL _IsPathSeparator(IZ_CHAR ch)
    {
        return ((ch == '/') || (ch == '\\'));
    }

    // Replace '\\' to '/'
    inline void _ReplacePathSeparetor(IZ_CHAR* str)
    {
        for (IZ_UINT i = 0; ; ++i) {
            IZ_CHAR ch = str[i];
            if (ch == '\0') {
                break;
            }
            
            if (ch == '\\') {
                str[i] = '/';
            }
        }
    }
}   // namespace

// Seek archive by name
IZ_BOOL CArchive::Seek(IZ_PCSTR path)
{
    IZ_ASSERT(m_pFileHeaders != IZ_NULL);

    // Replace '\\' to '/'
    //_ReplacePathSeparetor(const_cast<IZ_CHAR*>(path));

    // Create key for seeking.
    CKey cSeekKey(path);

    // Find file's header.
    S_ARC_FILE_HEADER* pFileHeader = m_HashList.FindData(cSeekKey);
    VRETURN(pFileHeader != IZ_NULL);

    // Seek in stream.
    IZ_BOOL ret = SeekStream(pFileHeader->pos);

    if (ret) {
        m_pCurFileHeader = pFileHeader;
    }

    return IZ_TRUE;
}

IZ_BOOL CArchive::SeekStream(IZ_UINT nPos)
{
    IZ_ASSERT(m_pInput != IZ_NULL);
    IZ_ASSERT(nPos > 0);

    IZ_BOOL ret = IZ_FALSE;
    
    // Get current position
    const IZ_UINT nCurPos = static_cast<const IZ_UINT>(m_pInput->GetCurPos());

    if (nCurPos == nPos) {
        // Nothing is done.
        ret = IZ_TRUE;
    }
    else if (nCurPos == 0) {
        ret = m_pInput->Seek(nPos, E_IO_STREAM_SEEK_POS_START);
    }
    else {
        IZ_INT nOffset = nPos - nCurPos;
        E_IO_STREAM_SEEK_POS nSeekPos = E_IO_STREAM_SEEK_POS_CUR;
        ret = m_pInput->Seek(nOffset, nSeekPos);
    }

    IZ_ASSERT(ret);

    return ret;
}

/**
* Return current file's description.
*/
IZ_BOOL CArchive::GetFileDesc(SArchiveFileDesc* pDesc)
{
    IZ_BOOL ret = (m_pCurFileHeader != IZ_NULL);

    if (ret) {
        // TODO

        IZ_UINT nKey = CStdUtil::GetPtrAdvanced(
                        m_pFileHeaders,
                        const_cast<S_ARC_FILE_HEADER*>(m_pCurFileHeader));
        
        ret = GetFileDesc(nKey, pDesc);
    }

    return ret;
}

// Get file's description by key.
IZ_BOOL CArchive::GetFileDesc(
    IZ_UINT nKey,
    SArchiveFileDesc* pDesc)
{
    IZ_ASSERT(pDesc != IZ_NULL);

    const S_ARC_FILE_HEADER* pHeader = GetFileHeader(nKey);
    VRETURN(pHeader != IZ_NULL);

    pDesc->size = GetSize(nKey);
    pDesc->sizeCompressed = 0;
    
    pDesc->isCompressed = IZ_FALSE;
    pDesc->isEncrypted = IZ_FALSE;

    pDesc->name = GetName(pHeader->posName);
    pDesc->key = nKey;
    
    pDesc->sizeCompressed = pHeader->sizeCompressed;
    pDesc->isCompressed = pHeader->isCompressed;
    pDesc->isEncrypted = pHeader->isEncrypted;

    return IZ_TRUE;
}

// Get file's size by key.
IZ_UINT CArchive::GetSize(IZ_UINT nKey) const
{
    const S_ARC_FILE_HEADER* pFileHeader = GetFileHeader(nKey);
    VRETURN_VAL(pFileHeader != IZ_NULL, 0);

    return pFileHeader->size;
}

// Get file's name.
IZ_PCSTR CArchive::GetName(IZ_UINT nPos) const
{
    IZ_ASSERT(m_pNameBuffer != IZ_NULL);
    IZ_ASSERT(nPos < m_sHeader.sizeName);

    return reinterpret_cast<IZ_PCSTR>(m_pNameBuffer + nPos);
}
