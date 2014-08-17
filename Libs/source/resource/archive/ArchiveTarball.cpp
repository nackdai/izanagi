#include "izSystem.h"
#include "resource/archive/ArchiveTarball.h"

namespace izanagi
{
namespace resource
{
    struct TarHeader {
        IZ_CHAR name[100];
        IZ_CHAR mode[8];
	    IZ_CHAR uid[8];
	    IZ_CHAR gid[8];
	    IZ_CHAR size[12];
	    IZ_CHAR mtime[12];
	    IZ_CHAR chksum[8];
	    IZ_CHAR typeflag;
	    IZ_CHAR linkname[100];
	    IZ_CHAR magic[6];
	    IZ_CHAR version[2];
	    IZ_CHAR uname[32];
	    IZ_CHAR gname[32];
	    IZ_CHAR major[8];
	    IZ_CHAR minor[8];
        IZ_CHAR padding[167];
    };

    class CArchiveTar::CTarHeader : public TarHeader {
    public:
        static CTarHeader* Create(
            IMemoryAllocator* allocator,
            IInputStream* in);

    private:
        CTarHeader() {}
        ~CTarHeader() {}

        IZ_DECL_PLACEMENT_NEW();

    public:
        CArchiveTar::TarHeaderHash::Item* GetHashItem() { return &m_HashItem; }

        IZ_UINT GetBytes() { return m_Bytes; }
        IZ_UINT GetPos() { return m_Pos; }

    private:
        CArchiveTar::TarHeaderHash::Item m_HashItem;

        IZ_UINT m_Pos;
        IZ_UINT m_Bytes;
    };

    inline IZ_UINT _OctalStringToInt(const char* str, IZ_INT num)
    {
        // 8i” -> 10i”

        IZ_UINT ret = 0;

        while (*str != '\0') {
            ret = ret * 8 + (*str - '0');
            str++;
        }

        return ret;
    }

    CArchiveTar::CTarHeader* CArchiveTar::CTarHeader::Create(
        IMemoryAllocator* allocator,
        IInputStream* in)
    {
        void* p = ALLOC(allocator, sizeof(CTarHeader));
        VRETURN_NULL(p);

        CTarHeader* instance = new(p) CTarHeader;
        {
            IZ_INPUT_READ(in, instance, 0, sizeof(TarHeader));

            CKey key(instance->name);
            instance->m_HashItem.Init(key, instance);

            instance->m_Bytes = _OctalStringToInt(instance->size, COUNTOF(instance->size));

            instance->m_Pos = (IZ_UINT)in->GetCurPos();
        }

        return instance;
    }

    /////////////////////////////////////////////////

    IArchive* CArchiveTar::CreateArchive(
        IMemoryAllocator* allocator,
        IInputStream* input)
    {
        void* p = ALLOC(allocator, sizeof(CArchiveTar));
        VRETURN_NULL(p);

        CArchiveTar* instance = new(p) CArchiveTar;
        {
            instance->AddRef();

            instance->m_Allocator = allocator;
            instance->m_Input = input;

            while (input->GetCurPos() < input->GetSize()) {
                CTarHeader* tarHeader = CTarHeader::Create(allocator, input);

                IZ_UINT offset = tarHeader->GetBytes();
                
                if (tarHeader == IZ_NULL) {
                    break;
                }
                else if (tarHeader->typeflag != '0') {
                    FREE(allocator, tarHeader);
                }
                else {
                    instance->m_Headers.Add(tarHeader->GetHashItem());
                }

                IZ_UINT odd = (offset & 511);

                if (odd > 0) {
                    offset += 512 - odd;
                }
                
                input->Seek(offset, E_IO_STREAM_SEEK_POS_CUR);
            }
        }

        return instance;
    }

    CArchiveTar::CArchiveTar()
    {
        m_Allocator = IZ_NULL;
        m_Input = IZ_NULL;
        m_MaxBytes = 0;
    }

    CArchiveTar::~CArchiveTar()
    {
        CStdList<TarHeaderHash::Item>::Item* item = m_Headers.GetOrderTop();
        while (item != IZ_NULL) {
            CStdList<TarHeaderHash::Item>::Item* next = item->GetNext();

            CTarHeader* header = item->GetData()->GetData();

            item = next;

            FREE(m_Allocator, header);
        }

        m_Headers.Clear();
    }

    IZ_UINT CArchiveTar::GetFileNum() const
    {
        IZ_UINT ret = m_Headers.GetDataNum();
        return ret;
    }

    IZ_UINT CArchiveTar::GetMaxFileSize() const
    {
        return m_MaxBytes;
    }

    IInputStream* CArchiveTar::GetInputStream()
    {
        return m_Input;
    }

     IZ_BOOL CArchiveTar::SeekByKey(
        IZ_UINT key,
        SArchiveFileDesc* desc)
     {
         TarHeaderHash::Item* item = m_Headers.Find(key);
         if (item == IZ_NULL) {
             return IZ_FALSE;
         }

         if (desc != IZ_NULL) {
             CTarHeader* header = item->GetData();

             desc->name = header->name;
             desc->size = header->GetBytes();
             desc->sizeCompressed = 0;
             desc->isCompressed = IZ_FALSE;
             desc->isEncrypted = IZ_FALSE;
             desc->filePos = header->GetPos();
             desc->key = key;
         }

         return IZ_TRUE;
     }

     IZ_BOOL CArchiveTar::SeekByPath(
        IZ_PCSTR path,
        SArchiveFileDesc* desc)
     {
         CKey key(path);
         return SeekByKey(key, desc);
     }

     IZ_BOOL CArchiveTar::Read(
        void* dst,
        const SArchiveFileDesc& desc)
     {
         IZ_ASSERT(m_Input != IZ_NULL);

         VRETURN(m_Input->Seek(desc.filePos, E_IO_STREAM_SEEK_POS_START));

         IZ_INPUT_READ(m_Input, dst, 0, desc.size);

         return IZ_TRUE;
     }
}   // namespace resource
}   // namespace izanagi
