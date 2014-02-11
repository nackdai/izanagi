#include "text/UString.h"
#include "izSystem.h"

namespace izanagi
{
namespace text
{
    CUString::CUString()
    {
        m_Encode = E_FONT_CHAR_ENCODE_NUM;
    }

    CUString::CUString(E_FONT_CHAR_ENCODE encode, const void* text)
    {
        m_Encode = encode;
        Init(text);
    }

    CUString::CUString(E_FONT_CHAR_ENCODE encode, const void* text, IZ_UINT bytes)
    {
        m_Encode = encode;
        Init(text, bytes);
    }

    void CUString::Init(const void* text)
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

        m_Num = 0;
        m_Iter = IZ_NULL;
        m_ReadBytes = 0;
    }

    void CUString::Init(const void* text, IZ_UINT bytes)
    {
        m_Text = CONST_CAST(IZ_UINT8*, void*, text);
        m_Bytes = bytes;

        m_Num = 0;
        m_Iter = IZ_NULL;
        m_ReadBytes = 0;
    }

    void CUString::Init(E_FONT_CHAR_ENCODE encode, const void* text)
    {
        IZ_ASSERT(m_Encode == E_FONT_CHAR_ENCODE_NUM);
        m_Encode = encode;
        Init(text);
    }

    void CUString::Init(E_FONT_CHAR_ENCODE encode, const void* text, IZ_UINT bytes)
    {
        IZ_ASSERT(m_Encode == E_FONT_CHAR_ENCODE_NUM);
        m_Encode = encode;
        Init(text, bytes);
    }

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
                case E_FONT_CHAR_ENCODE_SJIS:
                    tmp = CStdUtf::GetOneCharCodeAsSJIS(tmp, &code);
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

    IZ_UINT CUString::GetCurrent()
    {
        IZ_UINT ret = 0;
        
        if (m_ReadBytes >= m_Bytes)
        {
            return ret;
        }

        if (m_Iter != IZ_NULL)
        {
            void* tmp;
            IZ_UINT bytes = 0;
        
            switch (m_Encode)
            {
            case E_FONT_CHAR_ENCODE_UTF8:
                tmp = CStdUtf::GetOneCharCodeAsUTF8(m_Iter, &ret);
                break;
            case E_FONT_CHAR_ENCODE_UNICODE:
                tmp = CStdUtf::GetOneCharCodeAsUnicode(m_Iter, &ret);
                break;
            case E_FONT_CHAR_ENCODE_SJIS:
                tmp = CStdUtf::GetOneCharCodeAsSJIS(m_Iter, &ret);
                break;
            default:
                IZ_ASSERT(IZ_FALSE);
                break;
            }
        }

        return ret;
    }

    IZ_UINT CUString::GetCurrentAsUnicode()
    {
        IZ_UINT code = GetCurrent();

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

    IZ_UINT CUString::GetNext()
    {
        IZ_UINT ret = 0;

        if (m_Iter != IZ_NULL)
        {
            m_Iter = GetNextInternal(m_Iter, &ret);
            if (ret == 0)
            {
                //m_Iter = IZ_NULL;
            }
        }

        return ret;
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

    void* Realloc(
        IMemoryAllocator* allocator, 
        void* ptr, 
        IZ_UINT size)
    {
        void* ret = REALLOC(allocator, ptr, size);
        return ret;
    }

    void* CUString::ConvertToUnicode(IMemoryAllocator* allocator/*= IZ_NULL*/)
    {
        if (m_Encode == E_FONT_CHAR_ENCODE_UTF8)
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
        else if (m_Encode == E_FONT_CHAR_ENCODE_SJIS)
        {
            IZ_ASSERT(IZ_FALSE);
            return IZ_NULL;
        }

        return m_Text;
    }

    void* CUString::GetNextInternal(void* data, IZ_UINT* code)
    {
        if (m_ReadBytes >= m_Bytes)
        {
            *code = 0;
            return m_Iter;
        }

        void* ret = IZ_NULL;

        IZ_UINT bytes = 0;
        
        switch (m_Encode)
        {
        case E_FONT_CHAR_ENCODE_UTF8:
            ret = CStdUtf::GetOneCharCodeAsUTF8(data, code, &bytes);
            break;
        case E_FONT_CHAR_ENCODE_UNICODE:
            ret = CStdUtf::GetOneCharCodeAsUnicode(data, code, &bytes);
            break;
        case E_FONT_CHAR_ENCODE_SJIS:
            ret = CStdUtf::GetOneCharCodeAsSJIS(data, code, &bytes);
            break;
        default:
            IZ_ASSERT(IZ_FALSE);
            break;
        }

        m_ReadBytes += bytes;

        return ret;
    }

    //////////////////////////////////////////

    //////////////////////////////////////////

    CUnicodeString::~CUnicodeString()
    {
        FREE(m_Allocator, m_Text);
    }

    IZ_BOOL CUnicodeString::Read(
        IMemoryAllocator* allocator,
        IInputStream* stream)
    {
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
            // BOMあり
            encode = E_FONT_CHAR_ENCODE_UTF8;
            signatureBytes = 3;
        }
        else
        {
            // TODO
            // 必ずUTF-8扱い
            encode = E_FONT_CHAR_ENCODE_UTF8;
            signatureBytes = 0;
        }

        stream->Seek(signatureBytes, E_IO_STREAM_SEEK_POS_START);

        IZ_UINT byteSize = stream->GetSize() - signatureBytes;

        IZ_CHAR* src = (IZ_CHAR*)ALLOC(allocator, byteSize + 1);
        stream->Read(src, 0, byteSize);
        src[byteSize] = 0;

        IZ_BOOL ret = Read(allocator, encode, src, byteSize);

        FREE(allocator, src);

        return ret;
    }

    IZ_BOOL CUnicodeString::Read(
        IMemoryAllocator* allocator,
        E_FONT_CHAR_ENCODE encode,
        const void* src, 
        IZ_UINT bytes)
    {
        if (m_Text != IZ_NULL)
        {
            FREE(m_Allocator, m_Text);
            m_Allocator = IZ_NULL;
            m_Bytes = 0;
        }

        // TODO
        // 4バイトアラインする？

        m_Text = (IZ_UINT8*)ALLOC(allocator, bytes + 1);
        m_Text[bytes] = 0;

        m_Bytes = bytes;

        CStdUtf::SFuncIfLowMemory func =
        {
            allocator,
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

        m_Allocator = allocator;

        return IZ_TRUE;
    }
}    // namespace text
}    // namespace izanagi
