#include "text/ParagraphGroupImpl.h"
#include "text/ParagraphImpl.h"
#include "text/FontHost.h"
#include "text/UString.h"
#include "izGraph.h"

namespace izanagi
{
namespace text
{
    CDefaultParagraphGroup::CDefaultParagraphGroup()
    {
        m_FontHost = IZ_NULL;
    }

    CDefaultParagraphGroup::~CDefaultParagraphGroup()
    {
        SAFE_RELEASE(m_FontHost);
    }

    #define CH_LF   (0x000A)
    #define CH_CR   (0x000D)
    #define CH_LSEP (0x2028)
    #define CH_PSEP (0x2029)

    // 改行コード
	// Mac OS X を含む Unix（LF）
	// クラシック Mac OS（CR）
	// Windows（CR + LF）
	// ユニコード行／段落区切り（LSEP／PSEP）
    static const IZ_UINT separators[] = {
        CH_LF,      // LF
        CH_CR,      // CR
        CH_LSEP,    // LSEP
        CH_PSEP,    // PSEP
    };

    namespace
    {
        inline IZ_BOOL _IsSeparator(IZ_UINT code)
        {
            for (size_t i = 0; i < COUNTOF(separators); i++)
            {
                if (separators[i] == code)
                {
                    return IZ_TRUE;
                }
            }

            return IZ_FALSE;
        }
    }

    IZ_BOOL CDefaultParagraphGroup::Init(
        IFontHost* host,
        CUString& str, 
        void* userData)
    {
        SAFE_REPLACE(m_FontHost, host);

        m_LineHeight = m_FontHost->GetPixelSize();
        m_Ascent = m_FontHost->GetAscender();

        IZ_UINT prev = 0;
        IZ_UINT code = 0;

        IZ_BOOL isSeparated = IZ_FALSE;

        const IZ_UINT8* start = str.GetTextPtr();
        IZ_UINT lastPos = 1;

        str.BeginIter();

        for (;;)
        {
            if (isSeparated)
            {
                start = str.GetIterPtr();

                isSeparated = IZ_FALSE;
            }

            code = str.GetNext();

            if (prev == CH_CR && code == CH_LF)
            {
                // skip
                code = str.GetNext();
            }

            prev = code;

            if (_IsSeparator(code) || code == 0)
            {
                IZ_UINT pos = static_cast<IZ_UINT>(str.GetIterDistance());
                IZ_UINT bytes = pos - lastPos;
                lastPos = pos;

                CParagraph* paragraph = CDefaultParagraph::CreateParagraph(
                    m_Allocator,
                    host,
                    str.GetCharCode(),
                    start,
                    bytes,
                    userData);

                AddParagraph(paragraph);

                if (code == 0)
                {
                    break;
                }
                else
                {
                    isSeparated = IZ_TRUE;
                }
            }
        }

        str.EndIter();

        return IZ_TRUE;
    }
}   // namespace text
}   // namespace izanagi
