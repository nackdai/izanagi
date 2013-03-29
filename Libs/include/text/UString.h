#if !defined(__IZANAGI_TEXT_USTRING_H__)
#define __IZANAGI_TEXT_USTRING_H__

#include "izDefs.h"
#include "izStd.h"
#include "FNTFormat.h"

namespace izanagi
{
    class IInputStream;

namespace text
{
    /**
     */
    class CUString
    {
    public:
        CUString();
        CUString(E_FONT_CHAR_ENCODE encode, const void* text);
        CUString(E_FONT_CHAR_ENCODE encode, const void* text, IZ_UINT bytes);

        CUString(E_FONT_CHAR_ENCODE encode)
        {
            m_Encode = encode;

            m_Text = IZ_NULL;
            m_Bytes = 0;

            m_Num = 0;
            m_Iter = IZ_NULL;
            m_ReadBytes = 0;
        }
        virtual ~CUString() {}

    public:
        void Init(const void* text);
        void Init(const void* text, IZ_UINT bytes);

        void Init(E_FONT_CHAR_ENCODE encode, const void* text);
        void Init(E_FONT_CHAR_ENCODE encode, const void* text, IZ_UINT bytes);

        IZ_UINT GetNum();

        IZ_UINT GetOne(IZ_UINT index)
        {
            BeginIter();

            IZ_UINT ret = 0;

            for (IZ_UINT i = 0; i < index; i++)
            {
                ret = GetNext();
            }

            return ret;
        }

        void BeginIter()
        {
            m_Iter = m_Text;
            m_ReadBytes = 0;
        }

        void EndIter()
        {
            m_Iter = IZ_NULL;
        }

        IZ_UINT GetCurrent();
        IZ_UINT GetCurrentAsUnicode();

        IZ_UINT GetNext();
        IZ_UINT GetNextAsUnicode();

        void* ConvertToUnicode(IMemoryAllocator* allocator = IZ_NULL);

        const IZ_UINT8* GetTextPtr() const
        {
            return m_Text;
        }

        const IZ_UINT8* GetIterPtr() const
        {
            return reinterpret_cast<IZ_UINT8*>(m_Iter);
        }

        IZ_UINT64 GetIterDistance() const
        {
            IZ_UINT64 ret = CStdUtil::GetPtrDistance(
                (const IZ_UINT8*)m_Text, 
                (const IZ_UINT8*)m_Iter);
            return ret;
        }

        E_FONT_CHAR_ENCODE GetCharCode() const { return m_Encode; }

    protected:
         void* GetNextInternal(void* data, IZ_UINT* code);

    protected:
        E_FONT_CHAR_ENCODE m_Encode;

        IZ_UINT8* m_Text;
        IZ_UINT m_Bytes;

        IZ_UINT m_Num;

        void* m_Iter;
        IZ_UINT m_ReadBytes;
    };

    /**
     */
    class CUtf8String : public CUString
    {
    public:
        CUtf8String()
            : CUString(E_FONT_CHAR_ENCODE_UTF8)
        {
        }
        CUtf8String(const void* text, IZ_UINT bytes)
            : CUString(E_FONT_CHAR_ENCODE_UTF8, text, bytes)
        {
        }
        CUtf8String(const void* text)
            : CUString(E_FONT_CHAR_ENCODE_UTF8, text)
        {
        }

        virtual ~CUtf8String() {}
    };

    /**
     */
    class CUnicodeString : public CUString
    {
    public:
        CUnicodeString()
            : CUString(E_FONT_CHAR_ENCODE_UNICODE)
        {
            m_Allocator = IZ_NULL;
        }
        CUnicodeString(const void* text, IZ_UINT bytes)
            : CUString(E_FONT_CHAR_ENCODE_UNICODE, text, bytes)
        {
            m_Allocator = IZ_NULL;
        }
        CUnicodeString(const void* text)
            : CUString(E_FONT_CHAR_ENCODE_UNICODE, text)
        {
            m_Allocator = IZ_NULL;
        }

        virtual ~CUnicodeString();

    public:
        IZ_BOOL Read(
            IMemoryAllocator* allocator,
            IInputStream* stream);
        IZ_BOOL Read(
            IMemoryAllocator* allocator,
            E_FONT_CHAR_ENCODE encode,
            const void* src, 
            IZ_UINT bytes);

    protected:
        IMemoryAllocator* m_Allocator;
    };

    /**
     */
    class CSjisString : public CUString
    {
    public:
        CSjisString()
            : CUString(E_FONT_CHAR_ENCODE_SJIS)
        {
        }
        CSjisString(const void* text, IZ_UINT bytes)
            : CUString(E_FONT_CHAR_ENCODE_SJIS, text, bytes)
        {
        }
        CSjisString(const void* text)
            : CUString(E_FONT_CHAR_ENCODE_SJIS, text)
        {
        }

        virtual ~CSjisString() {}
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_USTRING_H__)
