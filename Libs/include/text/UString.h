#if !defined(__IZANAGI_TEXT_USTRING_H__)
#define __IZANAGI_TEXT_USTRING_H__

#include "izDefs.h"
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
    protected:
        CUString::CUString() {}

        CUString(E_FONT_CHAR_ENCODE encode)
        {
            m_Encode = encode;
            m_Num = 0;
            m_Text = IZ_NULL;
            m_Bytes = 0;
            m_Iter = IZ_NULL;
        }
        virtual ~CUString() {}

    public:
        virtual IZ_UINT GetNum();

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
        }

        void EndIter()
        {
            m_Iter = IZ_NULL;
        }

        IZ_UINT GetNext()
        {
            IZ_UINT ret = 0;

            if (m_Iter != IZ_NULL)
            {
                m_Iter = GetNextInternal(m_Iter, &ret);
                if (ret == 0)
                {
                    m_Iter = IZ_NULL;
                }
            }

            return ret;
        }

        virtual IZ_UINT GetNextAsUnicode();

        virtual void* ConvertToUnicode(IMemoryAllocator* allocator = IZ_NULL) { return m_Text; }

    protected:
         virtual void* GetNextInternal(void* data, IZ_UINT* code);

    protected:
        E_FONT_CHAR_ENCODE m_Encode;

        IZ_UINT8* m_Text;
        IZ_UINT m_Bytes;

        IZ_UINT m_Num;

        void* m_Iter;
    };

    /**
     */
    class CUtf8String : public CUString
    {
    public:
        CUtf8String(const void* text, IZ_UINT bytes)
            : CUString(E_FONT_CHAR_ENCODE_UTF8)
        {
            m_Text = CONST_CAST(IZ_UINT8*, void*, text);
            m_Bytes = bytes;
        }
        CUtf8String(const void* text);
        virtual ~CUtf8String() {}

    public:
        virtual void* ConvertToUnicode(IMemoryAllocator* allocator = IZ_NULL);
    };

    /**
     */
    class CUnicodeString : public CUString
    {
    public:
        CUnicodeString(IMemoryAllocator* allocator);
        ~CUnicodeString();

    public:
        IZ_BOOL Read(IInputStream* stream);
        IZ_BOOL Read(
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
        CSjisString(const void* text, IZ_UINT bytes)
            : CUString(E_FONT_CHAR_ENCODE_SJIS)
        {
            m_Text = CONST_CAST(IZ_UINT8*, void*, text);
            m_Bytes = bytes;
        }
        CSjisString(const void* text);
        virtual ~CSjisString() {}

    public:
        virtual void* ConvertToUnicode(IMemoryAllocator* allocator = IZ_NULL);
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_USTRING_H__)
