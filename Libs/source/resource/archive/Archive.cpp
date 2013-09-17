#include "resource/archive/Archive.h"
#include "izIo.h"

namespace izanagi
{
namespace resource
{
    // インスタンス作成
    CArchive* CArchive::CreateArchive(
        IMemoryAllocator* allocator,
        IInputStream* input)
    {
        IZ_ASSERT(input != IZ_NULL);

        CArchive* instance = IZ_NULL;

        // ファイルヘッダ読み込み
        S_ARC_HEADER header;
        IZ_BOOL result = IZ_INPUT_READ(input, &header, 0, sizeof(S_ARC_HEADER));
        VGOTO(result, __EXIT__);

        // マジックナンバーチェック
        result = IS_ARC_FORMAT(header.magic);
        VGOTO(result, __EXIT__);

        instance = CreateArchive(
            header,
            allocator,
            input);

    __EXIT__:
        if (!result) {
            if (instance != IZ_NULL) {
                SAFE_RELEASE(instance);
            }
        }

        return instance;
    }

    // インスタンス作成
    CArchive* CArchive::CreateArchive(
        const S_ARC_HEADER& header,
        IMemoryAllocator* allocator,
        IInputStream* input)
    {
        IZ_ASSERT(allocator != IZ_NULL);
        IZ_ASSERT(input != IZ_NULL);

        IZ_BOOL result = IZ_FALSE;
        IZ_UINT8* buf = IZ_NULL;
        IZ_UINT8* bufTop = IZ_NULL;
        CArchive* instance = IZ_NULL;

        IZ_UINT fileNum = header.numFiles;

        // 確保メモリサイズ
        size_t size = sizeof(CArchive);
        size += header.sizeName;
        size += header.sizeFileHeader;
        size += fileNum * sizeof(CArcHashItem);

        // メモリ確保
        buf = (IZ_UINT8*)ALLOC_ZERO(allocator, size);
        result = (buf != IZ_NULL);
        VGOTO(result, __EXIT__);

        // 先頭位置保持
        bufTop = buf;

        // インスタンス作成
        instance = new(buf) CArchive;
        {
            buf += sizeof(CArchive);

            instance->AddRef();

            instance->m_Allocator = allocator;
            instance->m_pInput = input;

            // ヘッダ
            memcpy(&instance->m_sHeader, &header, sizeof(header));

            // ファイル情報
            if (fileNum > 0) {
                instance->m_pFileHeaders = reinterpret_cast<S_ARC_FILE_HEADER*>(buf);

                size_t readSize = fileNum * sizeof(S_ARC_FILE_HEADER);
                result = IZ_INPUT_READ(
                            input, 
                            instance->m_pFileHeaders, 
                            0, readSize);
                VGOTO(result, __EXIT__);

                buf += readSize;
            }

            // 名前バッファ
            if (header.sizeName > 0) {
                instance->m_pNameBuffer = buf;

                result = IZ_INPUT_READ(
                            input, 
                            instance->m_pNameBuffer, 
                            0, 
                            header.sizeName);
                VGOTO(result, __EXIT__);

                buf += header.sizeName;
            }

            // ハッシュ要素
            if (instance->m_pFileHeaders != IZ_NULL) {
                instance->m_pHashItems = reinterpret_cast<CArcHashItem*>(buf);

                for (IZ_UINT i = 0; i < fileNum; ++i) {
                    const S_ARC_FILE_HEADER& fileHeader = *(instance->GetFileHeaderByIdx(i));

                    IZ_PCSTR name = instance->GetName(fileHeader.posName);
                    IZ_ASSERT(name != IZ_NULL);

                    // Set data to hash item,
                    instance->m_pHashItems[i].Init(
                        CKey(name), 
                        const_cast<S_ARC_FILE_HEADER*>(&fileHeader));

                    // Add hash item to hash list.
                    instance->m_HashList.Add(&instance->m_pHashItems[i]);
                }

                // Remember initial file header.
                instance->m_CurrentFielHeader = &instance->m_pFileHeaders[0];
            }
        }

        IZ_ASSERT(CStdUtil::GetPtrDistance(buf, bufTop) == size);

    __EXIT__:
        if (!result) {
            if (bufTop != IZ_NULL) {
                FREE(allocator, bufTop);
            }
            else if (instance != IZ_NULL) {
                SAFE_RELEASE(instance);
            }
        }

        return instance;
    }

    // コンストラクタ
    CArchive::CArchive()
    {
        m_pInput = IZ_NULL;

        FILL_ZERO(&m_sHeader, sizeof(m_sHeader));

        m_pNameBuffer = IZ_NULL;
        m_pFileHeaders = IZ_NULL;

        m_CurrentFielHeader = IZ_NULL;
    }

    // デストラクタ
    CArchive::~CArchive()
    {
        if (m_pInput != IZ_NULL) {
            m_pInput->Finalize();
            m_pInput = IZ_NULL;
        }
    }

    // Seek file by key.
    IZ_BOOL CArchive::SeekByKey(
        IZ_UINT key,
        SArchiveFileDesc* desc)
    {
        IZ_ASSERT(m_pFileHeaders != IZ_NULL);

        IZ_UINT idx = key;
        IZ_ASSERT(idx < m_sHeader.numFiles);

        // Get file's header.
        const S_ARC_FILE_HEADER* fileHeader = GetFileHeaderByIdx(idx);
        VRETURN(fileHeader != IZ_NULL);

        m_CurrentFielHeader = fileHeader;

        // Seek in stream.
        IZ_BOOL ret = SeekStream(fileHeader->pos);

        if (ret) {
            if (desc != IZ_NULL) {

                SetFileDesc(desc, *m_CurrentFielHeader);
            }
        }

        return ret;
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
    IZ_BOOL CArchive::SeekByPath(
        IZ_PCSTR path,
        SArchiveFileDesc* desc)
    {
        IZ_ASSERT(m_pFileHeaders != IZ_NULL);

        // Replace '\\' to '/'
        _ReplacePathSeparetor(const_cast<IZ_CHAR*>(path));

        // Create key for seeking.
        CKey key(path);

        // Find file's header.
        S_ARC_FILE_HEADER* fileHeader = m_HashList.FindData(key);
        VRETURN(fileHeader != IZ_NULL);

        m_CurrentFielHeader = fileHeader;

        // Seek in stream.
        IZ_BOOL ret = SeekStream(fileHeader->pos);

        if (ret) {
            if (desc != IZ_NULL) {
                SetFileDesc(desc, *m_CurrentFielHeader);
            }
        }

        return ret;
    }

    // Read data.
    IZ_BOOL CArchive::Read(
        void* dst,
        const SArchiveFileDesc& desc)
    {
        VRETURN(SeekByKey(desc.key, IZ_NULL));

        if (IZ_INPUT_READ(
            m_pInput,
            dst,
            0,
            desc.size))
        {
            return IZ_TRUE;
        }

        IZ_ASSERT(IZ_FALSE);
        return IZ_FALSE;
    }

    // Seek input stream.
    IZ_BOOL CArchive::SeekStream(IZ_UINT pos)
    {
        IZ_ASSERT(m_pInput != IZ_NULL);
        IZ_ASSERT(pos > 0);

        IZ_BOOL ret = IZ_FALSE;
    
        // Get current position
        const IZ_UINT curPos = static_cast<const IZ_UINT>(m_pInput->GetCurPos());

        if (curPos == pos) {
            // Nothing is done.
            ret = IZ_TRUE;
        }
        else if (curPos == 0) {
            ret = m_pInput->Seek(pos, E_IO_STREAM_SEEK_POS_START);
        }
        else {
            IZ_INT offset = pos - curPos;
            ret = m_pInput->Seek(offset, E_IO_STREAM_SEEK_POS_CUR);
        }

        IZ_ASSERT(ret);

        return ret;
    }

    // Set file description.
    void CArchive::SetFileDesc(
        SArchiveFileDesc* dst,
        const S_ARC_FILE_HEADER& src)
    {
        dst->size = src.size;
        dst->sizeCompressed = 0;

        dst->key = src.key;

        dst->isCompressed = src.isCompressed;
        dst->isEncrypted = src.isEncrypted;

        dst->name = GetName(src.posName);
        dst->filePos = src.pos;
    }

    // Get file's name.
    IZ_PCSTR CArchive::GetName(IZ_UINT pos) const
    {
        IZ_ASSERT(m_pNameBuffer != IZ_NULL);
        IZ_ASSERT(pos < m_sHeader.sizeName);

        return reinterpret_cast<IZ_PCSTR>(m_pNameBuffer + pos);
    }

    // Get file header.
    const S_ARC_FILE_HEADER* CArchive::GetFileHeaderByIdx(IZ_UINT idx) const
    {
        VRETURN_NULL(idx < m_sHeader.numFiles);

        IZ_ASSERT(m_pFileHeaders != IZ_NULL);
        return &m_pFileHeaders[idx];
    }
}   // namespace resource
}   // namespace izanagi
