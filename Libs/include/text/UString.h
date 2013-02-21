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
            m_IsInIter = IZ_FALSE;
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
            m_IsInIter = IZ_TRUE;
            m_IterTmp = m_Text;
        }

        void EndIter()
        {
            // Nothing...
        }

        IZ_UINT GetNext()
        {
            IZ_UINT ret = 0;

            if (m_IsInIter)
            {
                m_IterTmp = GetNextInternal(m_IterTmp, &ret);
                if (ret == 0)
                {
                    m_IsInIter = IZ_FALSE;
                }
            }

            return ret;
        }

    protected:
         PURE_VIRTUAL(void* GetNextInternal(void* data, IZ_UINT* code));

    protected:
        E_FONT_CHAR_ENCODE m_Encode;

        IZ_UINT8* m_Text;
        IZ_UINT m_Bytes;

        IZ_UINT m_Num;

        IZ_BOOL m_IsInIter;
        void* m_IterTmp;
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

    protected:
        virtual void* GetNextInternal(void* data, IZ_UINT* code);
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_USTRING_H__)
