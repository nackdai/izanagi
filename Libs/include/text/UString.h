#if !defined(__IZANAGI_TEXT_USTRING_H__)
#define __IZANAGI_TEXT_USTRING_H__

#include "izDefs.h"
#include "FNTFormat.h"

namespace izanagi
{
namespace text
{
    /**
     */
    class CUString
    {
    protected:
        CUString(E_FONT_CHAR_ENCODE encode)
        {
            m_Encode = encode;
            m_Num = 0;
            m_Iter = IZ_NULL;
        }
        virtual ~CUString() {}

    public:
        PURE_VIRTUAL(IZ_UINT GetNum());

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

        PURE_VIRTUAL(IZ_UINT GetNextAsUnicode());

    protected:
         PURE_VIRTUAL(void* GetNextInternal(void* data, IZ_UINT* code));

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
        CUtf8String(const char* text, IZ_UINT bytes)
            : CUString(E_FONT_CHAR_ENCODE_UTF8)
        {
            m_Text = CONST_CAST(IZ_UINT8*, char*, text);
            m_Bytes = bytes;
        }
        virtual ~CUtf8String() {}

    public:
        virtual IZ_UINT GetNum();

        virtual IZ_UINT GetNextAsUnicode();

    protected:
        virtual void* GetNextInternal(void* data, IZ_UINT* code);
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_USTRING_H__)
