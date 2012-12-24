#include "resource/archive/ArchiveZip.h"
//#include "../../zlib/zlib.h"

using namespace izanagi;

struct CArchiveZip::ZipDirHeader
{
    enum
    {
        SIGNATURE = 0x06054b50
    };

    IZ_DWORD sig;
    IZ_WORD  nDisk;
    IZ_WORD  nStartDisk;
    IZ_WORD  nDirEntries;
    IZ_WORD  totalDirEntries;
    IZ_DWORD dirSize;
    IZ_DWORD dirOffset;
    IZ_WORD  cmntLen;

    IZ_WORD  padding;
};

struct CArchiveZip::ZipDirFileHeader
{
    enum
    {
        SIGNATURE   = 0x02014b50
    };

    IZ_DWORD   sig;
    IZ_WORD    verMade;
    IZ_WORD    verNeeded;
    IZ_WORD    flag;
    IZ_WORD    compression;      // COMP_xxxx
    IZ_WORD    modTime;
    IZ_WORD    modDate;
    IZ_DWORD   crc32;
    IZ_DWORD   cSize;            // Compressed size
    IZ_DWORD   ucSize;           // Uncompressed size
    IZ_WORD    fnameLen;         // Filename string follows header.
    IZ_WORD    xtraLen;          // Extra field follows filename.
    IZ_WORD    cmntLen;          // Comment field follows extra field.
    IZ_WORD    diskStart;
    IZ_WORD    intAttr;

    IZ_WORD padding;

    IZ_DWORD   extAttr;
    IZ_DWORD   hdrOffset;

    IZ_DWORD posName;
    IZ_DWORD posExtra;
    IZ_DWORD posComment;

    CStdHash<CKey, ZipDirFileHeader, HASH_SIZE>::Item hashItem;
};

const IZ_INT CArchiveZip::ZipDirHeaderSize = OFFSETOF(CArchiveZip::ZipDirHeader, padding);
const IZ_INT CArchiveZip::ZipDirFileHeaderSize = sizeof(IZ_WORD) * 11 + sizeof(IZ_DWORD) * 6;

/**
*/
IArchive* CArchiveZip::CreateArchiveZip(
    IMemoryAllocator* pAllocator,
    IInputStream* pIn)
{
    CArchiveZip* pInstance = IZ_NULL;
    IZ_BOOL result = IZ_FALSE;
    ZipDirHeader sDirHeader;

    // Read ZipDirHeader.
    IZ_UINT nDirHeaderOffset = ReadZipDirHeader(pIn, &sDirHeader);

    // Compute buffer size.
    IZ_UINT nDirFileHeaderSize = sizeof(ZipDirFileHeader) * sDirHeader.nDirEntries;
    IZ_UINT nStrSize = sDirHeader.dirSize - ZipDirFileHeaderSize * sDirHeader.nDirEntries;
    nStrSize += sDirHeader.nDirEntries * 3; // For string terminater('\0').

    size_t size = sizeof(CArchiveZip);
    size += nDirFileHeaderSize;
    size += nStrSize;

    // Allocate buffer.
    IZ_BYTE* pBuf = reinterpret_cast<IZ_BYTE*>(ALLOC_ZERO(pAllocator, size));
    VRETURN(pBuf != IZ_NULL);

    pInstance = new (pBuf) CArchiveZip;
    {
        pInstance->AddRef();
        pInstance->m_Allocator = pAllocator;

        pBuf += sizeof(CArchiveZip);

        pInstance->m_pFileHeaders = reinterpret_cast<ZipDirFileHeader*>(pBuf);
        pInstance->m_pString = reinterpret_cast<IZ_CHAR*>(pBuf + nDirFileHeaderSize);

        result = pInstance->Init(
                    sDirHeader,
                    pIn);
        VGOTO(result, __EXIT__);
    }

__EXIT__:
    if (!result) {
        if (pInstance != IZ_NULL) {
            SAFE_RELEASE(pInstance);
        }
        else {
            FREE(pAllocator, pBuf);
        }
    }

    return pInstance;
}

// Constructor
CArchiveZip::CArchiveZip()
{
    m_Allocator = IZ_NULL;

    m_pInput = IZ_NULL;

    m_pFileHeaders = IZ_NULL;
    m_pString = IZ_NULL;

    m_pCurFileHeader = IZ_NULL;
}

// Destructor
CArchiveZip::~CArchiveZip()
{
}

// Read ZipDirHeader from stream.
IZ_UINT CArchiveZip::ReadZipDirHeader(
    IInputStream* pIn,
    ZipDirHeader* pHeader)
{   
    static IZ_UINT8 buf[ZipDirHeaderSize];

    pIn->Seek(-ZipDirHeaderSize, izanagi::E_IO_STREAM_SEEK_POS_END);
    IZ_LONG nDirHeaderOffset = pIn->GetCurPos();
    IZ_INPUT_READ_ASSERT(pIn, buf, 0, sizeof(buf));

    memcpy(pHeader, buf, sizeof(buf));
    IZ_ASSERT(pHeader->sig == ZipDirHeader::SIGNATURE);

    return static_cast<IZ_UINT>(nDirHeaderOffset);
}

#define READ_STRING(s, dst, size)\
    IZ_INPUT_READ_ASSERT(s, dst, 0, size);\
    dst += size;\
    *dst = '\0';\
    ++dst

// Read ZipDirFileHeader from stream.
IZ_CHAR* CArchiveZip::ReadZipDirFileHeader(
    IInputStream* pIn,
    IZ_CHAR* pString,
    IZ_UINT* pStrPos,
    ZipDirFileHeader* pHeader)
{
    static IZ_UINT8 buf[ZipDirFileHeaderSize];

    IZ_INPUT_READ_ASSERT(pIn, buf, 0, sizeof(buf));

    {
        // Copy data before padding.
        size_t size = OFFSETOF(CArchiveZip::ZipDirFileHeader, padding);
        memcpy(pHeader, buf, size);

        // Copy data after padding.
        IZ_WORD* p = &(pHeader->padding);
        memcpy(p + 1, buf + size, sizeof(IZ_DWORD) * 2);
    }

    IZ_ASSERT(pHeader->sig == ZipDirFileHeader::SIGNATURE);

    if (pHeader->fnameLen > 0) {
        READ_STRING(pIn, pString, pHeader->fnameLen);
        pHeader->posName = *pStrPos;
        *pStrPos += pHeader->fnameLen + 1;
    }
    if (pHeader->xtraLen > 0) {
        READ_STRING(pIn, pString, pHeader->xtraLen);
        pHeader->posExtra = *pStrPos;
        *pStrPos += pHeader->xtraLen + 1;
    }
    if (pHeader->cmntLen > 0) {
        READ_STRING(pIn, pString, pHeader->cmntLen);
        pHeader->posComment = *pStrPos;
        *pStrPos += pHeader->cmntLen + 1;
    }

    return pString;
}

IZ_CHAR* CArchiveZip::GetName(IZ_UINT pos) const
{
    return m_pString + pos;
}

// Initialize.
IZ_BOOL CArchiveZip::Init(
    const ZipDirHeader& sDirHeader,
    IInputStream* pIn)
{
    IZ_CHAR* pStr = m_pString;
    IZ_UINT nStrPos = 0;

    m_nMaxFileSize = 0;

    for (IZ_UINT i = 0; i < sDirHeader.nDirEntries; ++i) {
        ZipDirFileHeader& sDirFileHeader = m_pFileHeaders[i];

        // Read ZipDirFileHeader.
        pStr = ReadZipDirFileHeader(
                pIn,
                pStr,
                &nStrPos,
                &sDirFileHeader);

        // Add to the hash list.
        IZ_CHAR* name = GetName(sDirFileHeader.posName);
        sDirFileHeader.hashItem.Init(CKey(name), &sDirFileHeader);
        m_HashList.Add(&sDirFileHeader.hashItem);
        
        // TODO
        m_nMaxFileSize = (sDirFileHeader.ucSize > m_nMaxFileSize
                            ? sDirFileHeader.ucSize
                            : m_nMaxFileSize);
    }

    m_pInput = pIn;
    m_nFileNum = sDirHeader.nDirEntries;

    return IZ_TRUE;
}

/**
* Get num of files in ARC.
*/
IZ_UINT CArchiveZip::GetFileNum() const
{
    return m_nFileNum;
}

/**
* Get max file's size in ARC.
*/
IZ_UINT CArchiveZip::GetMaxFileSize() const
{
    return m_nMaxFileSize;
}

/**
* Seek file by key.
*/
IZ_BOOL CArchiveZip::Seek(IZ_UINT nKey)
{
    IZ_ASSERT(nKey < m_nFileNum);
    IZ_ASSERT(m_pFileHeaders != IZ_NULL);

    ZipDirFileHeader* pFileHeader = &m_pFileHeaders[nKey];

    IZ_BOOL ret = SeekStream(pFileHeader->hdrOffset);

    if (ret) {
        m_pCurFileHeader = pFileHeader;
    }

    return IZ_TRUE;
}

/**
* Seek file by path.
*/
IZ_BOOL CArchiveZip::Seek(IZ_PCSTR path)
{
    // Create key for seeking.
    CKey cSeekKey(path);

    ZipDirFileHeader* pFileHeader = m_HashList.FindData(cSeekKey);
    VRETURN(pFileHeader != IZ_NULL);

    IZ_BOOL ret = SeekStream(pFileHeader->hdrOffset);

    if (ret) {
        m_pCurFileHeader = pFileHeader;
    }

    return IZ_TRUE;
}

/**
* Return current file's description.
*/
IZ_BOOL CArchiveZip::GetFileDesc(SArchiveFileDesc* pDesc)
{
    IZ_BOOL ret = (m_pCurFileHeader != IZ_NULL);

    if (ret) {
        IZ_UINT nKey = CStdUtil::GetPtrAdvanced(
                        m_pFileHeaders,
                        const_cast<ZipDirFileHeader*>(m_pCurFileHeader));
        
        pDesc->size = m_pCurFileHeader->ucSize;
        pDesc->sizeCompressed = m_pCurFileHeader->cSize;
        
        pDesc->isCompressed = (pDesc->size == pDesc->sizeCompressed);
        pDesc->isEncrypted = IZ_FALSE;

        pDesc->name = GetName(m_pCurFileHeader->posName);
        pDesc->key = nKey;
    }

    return ret;
}

IZ_BOOL CArchiveZip::SeekStream(IZ_UINT nPos)
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
