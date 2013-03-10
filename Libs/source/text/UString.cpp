#include "text/UString.h"
#include "izIo.h"

namespace izanagi
{
namespace text
{
    IZ_UINT CUString::GetNum()
    {
        if (m_Num == 0)
        {
            void* tmp = reinterpret_cast<void*>(m_Text);

            for (;;)
            {
                IZ_UINT code = 0;

                switch (m_Encode)
                {
                case E_FONT_CHAR_ENCODE_UTF8:
                    tmp = CStdUtf::GetOneCharCodeAsUTF8(tmp, &code);
                    break;
                case E_FONT_CHAR_ENCODE_UNICODE:
                    tmp = CStdUtf::GetOneCharCodeAsUnicode(tmp, &code);
                    break;
                default:
                    IZ_ASSERT(IZ_FALSE);
                    break;
                }

                if (code == 0)
                {
                    break;
                }
                m_Num++;
            }
        }
        return m_Num;
    }

    IZ_UINT CUString::GetNextAsUnicode()
    {
        IZ_UINT code = GetNext();

        switch (m_Encode)
        {
        case E_FONT_CHAR_ENCODE_UTF8:
            code = CStdUtf::ConvertUtf8ToUnicode(code);
            break;
        case E_FONT_CHAR_ENCODE_UNICODE:
            // Nothing
            break;
        default:
            IZ_ASSERT(IZ_FALSE);
            break;
        }

        return code;
    }

    void* CUString::GetNextInternal(void* data, IZ_UINT* code)
    {
        void* ret = IZ_NULL;
        
        switch (m_Encode)
        {
        case E_FONT_CHAR_ENCODE_UTF8:
            ret = CStdUtf::GetOneCharCodeAsUTF8(data, code);
            break;
        case E_FONT_CHAR_ENCODE_UNICODE:
            ret = CStdUtf::GetOneCharCodeAsUnicode(data, code);
            break;
        default:
            IZ_ASSERT(IZ_FALSE);
            break;
        }

        return ret;
    }

    //////////////////////////////////////////

    CUtf8String::CUtf8String(const void* text)
        : CUString(E_FONT_CHAR_ENCODE_UTF8)
    {
        m_Text = CONST_CAST(IZ_UINT8*, void*, text);

        // TODO
        m_Bytes = 0;
        IZ_UINT8 ch = 0;
        IZ_UINT i = 0;
        while  ((ch = m_Text[i++]) != 0)
        {
            m_Bytes++;
        }
    }

    void* Realloc(
        IMemoryAllocator* allocator, 
        void* ptr, 
        IZ_UINT size)
    {
        void* ret = REALLOC(allocator, ptr, size);
        return ret;
    }

    void* CUtf8String::ConvertToUnicode(IMemoryAllocator* allocator/*= IZ_NULL*/)
    {
        void* dst = ALLOC(allocator, m_Bytes + 1);

        CStdUtf::SFuncIfLowMemory func =
        {
            allocator,
            Realloc,
        };

        CStdUtf::ConvertUtf8ToUnicode(
            &dst,
            m_Bytes,
            m_Text,
            func);

        return dst;
    }

    //////////////////////////////////////////

    CUnicodeString::CUnicodeString(IMemoryAllocator* allocator)
        : CUString(E_FONT_CHAR_ENCODE_UNICODE)
    {
        m_Allocator = allocator;
    }

    CUnicodeString::~CUnicodeString()
    {
        FREE(m_Allocator, m_Text);
    }

    IZ_BOOL CUnicodeString::Read(IInputStream* stream)
    {
        if (m_Text == IZ_NULL)
        {
            return IZ_FALSE;
        }

        IZ_CHAR signature[4];
        stream->Read(signature, 0, 4);

        IZ_UINT signatureBytes = 0;

        E_FONT_CHAR_ENCODE encode = E_FONT_CHAR_ENCODE_UTF8;

        if (signature[0] == '\xFF' && signature[1] == '\xFE')
        {
            encode = E_FONT_CHAR_ENCODE_UTF16;
            signatureBytes = 2;
        }
        else if (signature[0] == '\xEF' && signature[1] == '\xBB' && signature[2] == '\xBF')
        {
            // BOM‚ ‚è
            encode = E_FONT_CHAR_ENCODE_UTF8;
            signatureBytes = 3;
        }
        else
        {
            // TODO
            // •K‚¸UTF-8ˆµ‚¢
            encode = E_FONT_CHAR_ENCODE_UTF8;
            signatureBytes = 0;
        }

        stream->Seek(signatureBytes, E_IO_STREAM_SEEK_POS_START);

        IZ_UINT byteSize = stream->GetSize() - signatureBytes;

        IZ_CHAR* src = (IZ_CHAR*)ALLOC(m_Allocator, byteSize + 1);
        stream->Read(src, 0, byteSize);
        src[byteSize] = 0;

        IZ_BOOL ret = Read(encode, src, byteSize);

        FREE(m_Allocator, src);

        return ret;
    }

    IZ_BOOL CUnicodeString::Read(
        E_FONT_CHAR_ENCODE encode,
        const void* src, 
        IZ_UINT bytes)
    {
        if (m_Text != IZ_NULL)
        {
            return IZ_FALSE;
        }

        m_Text = (IZ_UINT8*)ALLOC(m_Allocator, bytes + 1);

        CStdUtf::SFuncIfLowMemory func =
        {
            m_Allocator,
            Realloc,
        };

        if (encode == E_FONT_CHAR_ENCODE_UTF8)
        {
            CStdUtf::ConvertUtf8ToUnicode(
                (void**)&m_Text,
                bytes,
                src,
                func);
        }
        else if (encode == E_FONT_CHAR_ENCODE_UTF16)
        {
            CStdUtf::ConvertUtf16ToUnicode(
                (void**)&m_Text,
                bytes,
                src,
                func);
        }

        return IZ_TRUE;
    }
}    // namespace text
}    // namespace izanagi
