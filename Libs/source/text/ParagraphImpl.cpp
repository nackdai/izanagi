#include "text/ParagraphImpl.h"
#include "text/FontHost.h"
#include "text/TextLineImpl.h"
#include "izGraph.h"

namespace izanagi
{
namespace text
{
    CDefaultParagraph::CDefaultParagraph()
    {
        m_FontHost = IZ_NULL;
    }

    CDefaultParagraph::~CDefaultParagraph()
    {
        SAFE_RELEASE(m_FontHost);
    }

    IZ_BOOL CDefaultParagraph::Init(
        IFontHost* host,
        E_FONT_CHAR_ENCODE encode,
        const void* text,
        IZ_UINT bytes,
        void* userData)
    {
        SAFE_REPLACE(m_FontHost, host);
        m_String.Init(encode, text, bytes);
        return IZ_TRUE;
    }

    void CDefaultParagraph::BeginCreateLine()
    {
        m_LastPos = 0;
        m_String.BeginIter();
    }

    void CDefaultParagraph::EndCreateLine()
    {
        m_String.EndIter();
    }

    CLine* CDefaultParagraph::CreateLine(IZ_UINT width)
    {
        E_FONT_CHAR_ENCODE encode = m_FontHost->GetEncodeType();

        CLine* ret = IZ_NULL;

        IZ_UINT code = 0;
        IZ_UINT advanced = 0;

        const void* ptr = m_String.GetIterPtr();

        for (IZ_UINT count = 0; ; count++)
        {
            if (encode == E_FONT_CHAR_ENCODE_UNICODE)
            {
                code = m_String.GetCurrentAsUnicode();
            }
            else
            {
                code = m_String.GetCurrent();
            }

            IZ_BOOL isFinishLine = IZ_FALSE;

            if (code == 0)
            {
                isFinishLine = IZ_TRUE;
            }
            else
            {
                IZ_UINT id = m_FontHost->GetGlyphID(code);

                SGlyphMetrics metrics;

                if (m_FontHost->GetGlyphMetricsByID(id, metrics))
                {
                    if (advanced + metrics.advance >= width)
                    {
                        isFinishLine = IZ_TRUE;;
                    }
                    else
                    {
                        advanced += metrics.advance;
                        m_String.GetNext();
                    }
                }
                else
                {
                    // TODO
                }
            }

            if (isFinishLine)
            {
                if (count > 0)
                {
                    IZ_UINT pos = static_cast<IZ_UINT>(m_String.GetIterDistance());
                    IZ_UINT bytes = pos - m_LastPos;
                    m_LastPos = pos;

                    SLineParam param;
                    {
                        param.encode = m_String.GetCharCode();
                        param.text = ptr;
                        param.bytes = bytes;
                    }

                    ret = CDefaultLine::CreateLine(
                        m_Allocator,
                        m_FontHost,
                        &param);
                }

                break;
            }
        }

        return ret;
    }
}   // namespace text
}   // namespace izanagi
