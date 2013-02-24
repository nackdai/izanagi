#include "text/UString.h"

namespace izanagi
{
namespace text
{
    IZ_UINT CUtf8String::GetNum()
    {
        if (m_Num == 0)
        {
            void* tmp = reinterpret_cast<void*>(m_Text);

            for (;;)
            {
                IZ_UINT code = 0;
                tmp = CStdUtf::GetOneCharCodeAsUTF8(tmp, &code);
                if (code == 0)
                {
                    break;
                }
                m_Num++;
            }
        }
        return m_Num;
    }

    IZ_UINT CUtf8String::GetNextAsUnicode()
    {
        IZ_UINT code = GetNext();

        if (m_Encode == E_FONT_CHAR_ENCODE_UTF8)
        {
            code = CStdUtf::ConvertUtf8ToUnicode(code);
        }

        return code;
    }

    void* CUtf8String::GetNextInternal(void* data, IZ_UINT* code)
    {
        void* ret = CStdUtf::GetOneCharCodeAsUTF8(data, code);
        return ret;
    }
}    // namespace text
}    // namespace izanagi
