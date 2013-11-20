#include "text/TextLineImpl.h"
#include "text/FontHost.h"
#include "text/ParagraphImpl.h"
#include "text/UString.h"
#include "izGraph.h"

namespace izanagi
{
namespace text
{
    CDefaultLine::CDefaultLine()
    {
        m_Text = IZ_NULL;
        m_Bytes = 0;
    }

    IZ_UINT CDefaultLine::GetLineWidth()
    {
        if (m_Text == IZ_NULL)
        {
            return 0;
        }

        CUString string;
        string.Init(m_Encode, m_Text, m_Bytes);

        IZ_UINT width = 0;
        IZ_UINT code = 0;

        string.BeginIter();

        for (;;)
        {
            if (m_FontHost->GetEncodeType() == E_FONT_CHAR_ENCODE_UNICODE)
            {
                code = string.GetNextAsUnicode();
            }
            else
            {
                code = string.GetNext();
            }

            if (code == 0)
            {
                break;
            }

            SGlyphMetrics metrics;

            if (m_FontHost->GetGlyphMetricsByCode(code, metrics))
            {
                width += metrics.advance;
            }
            else
            {
                // TODO
            }
        }

        string.EndIter();

        return width;
    }

    IZ_BOOL CDefaultLine::Init(void* userData)
    {
        CDefaultParagraph::SLineParam* param = reinterpret_cast<CDefaultParagraph::SLineParam*>(userData);
        m_Encode = param->encode;
        m_Text = param->text;
        m_Bytes = param->bytes;
        return IZ_TRUE;
    }

    void CDefaultLine::Prepare(
        IZ_UINT8* dst,
        IZ_UINT pitch,
        IZ_UINT ascent,
        IFontHost* host)
    {
        IZ_UINT code = 0;

        CUString string;
        string.Init(m_Encode, m_Text, m_Bytes);

        string.BeginIter();

        IZ_UINT x = 0;

        for (;;)
        {
            if (m_FontHost->GetEncodeType() == E_FONT_CHAR_ENCODE_UNICODE)
            {
                code = string.GetNextAsUnicode();
            }
            else
            {
                code = string.GetNext();
            }

            if (code == 0)
            {
                break;
            }

            IZ_UINT id = m_FontHost->GetGlyphID(code);

            SGlyphMetrics metrics;

            if (m_FontHost->GetGlyphMetricsByID(id, metrics))
            {
                x = this->CopyImage(
                    id,
                    dst,
                    x,
                    pitch,
                    ascent);
            }
            else
            {
                // TODO
            }
        }

        string.EndIter();
    }
}   // namespace text
}   // namespace izanagi
