#include "unicode/utypes.h"
#include "unicode/uchar.h"
#include "unicode/ubidi.h"
#include "unicode/ustring.h"

#include "ParagraphCache.h"

namespace izanagi
{
    // 改行コード
	// Mac OS X を含む Unix（LF）
	// クラシック Mac OS（CR）
	// Windows（CR + LF）
	// ユニコード行／段落区切り（LSEP／PSEP）
    static const LEUnicode separators[] = {
        0x000A, // LF
        0x000D, // CR
        0x2028, // LSEP
        0x2029, // PSEP
        0x0000
    };


    IZ_BOOL CParagraphGroup::Init(
        izanagi::text::IFontHost* host,
        izanagi::text::CUString& str, 
        void* userData)
    {
        SAFE_REPLACE(m_Host, host);

        FontRuns fontRuns(0);
        fontRuns.add((const LEFontInstance*)userData, str.GetNum());
        InitInternal(str, &fontRuns);

        return IZ_TRUE;
    }

    #define CH_LF 0x000A
    #define CH_CR 0x000D
    #define CH_LSEP 0x2028
    #define CH_PSEP 0x2029

    static LEUnicode *skipLineEnd(LEUnicode *ptr)
    {
        if (ptr[0] == CH_CR && ptr[1] == CH_LF) {
            ptr += 1;
        }

        return ptr + 1;
    }


    le_int32 findRun(const RunArray *runArray, le_int32 offset)
    {
        le_int32 runCount = runArray->getCount();

        for (le_int32 run = 0; run < runCount; run += 1) {
            if (runArray->getLimit(run) > offset) {
                return run;
            }
        }

        return -1;
    }

    void subsetFontRuns(const FontRuns *fontRuns, le_int32 start, le_int32 limit, FontRuns *sub)
    {
        le_int32 startRun = findRun(fontRuns, start);
        le_int32 endRun   = findRun(fontRuns, limit - 1);

        sub->reset();

        for (le_int32 run = startRun; run <= endRun; run += 1) {
            const LEFontInstance *runFont = fontRuns->getFont(run);
            le_int32 runLimit = fontRuns->getLimit(run) - start;

            if (run == endRun) {
                runLimit = limit - start;
            }

            sub->add(runFont, runLimit);
        }
    }

    void CParagraphGroup::InitInternal(
        text::CUString& text, 
        const FontRuns* fontRuns)
    {
        FontRuns subFontRuns(0);

        LEUnicode* data = (LEUnicode*)text.ConvertToUnicode(m_Allocator);
        IZ_UINT num = text.GetNum();

        LEErrorCode status;
        LocaleRuns *locales = NULL;

        m_Level = UBIDI_DEFAULT_LTR;

        LEUnicode* start = data;

        IZ_UINT ascent = 0;
        IZ_UINT descent = 0;
        IZ_UINT leading = 0;

        while (start != IZ_NULL)
        {
            LEUnicode* end = u_strpbrk(start, separators);

            if (end == IZ_NULL)
            {
                end = &start[num];
            }

            if (end != start)
            {
                subsetFontRuns(fontRuns, start - data, end - data, &subFontRuns);

                ParagraphLayout* layout = new ParagraphLayout(
                    start,
                    end - start,
                    &subFontRuns,
                    NULL,
                    NULL,
                    locales,
                    m_Level,
                    FALSE,
                    status);

                if (m_Level == UBIDI_DEFAULT_LTR) {
                    m_Level = layout->getParagraphLevel();
                }

                CParagraph* paragraph = CreateParagraph(
                    m_Allocator,
                    m_Host,
                    text.GetCharCode(),
                    start,
                    (end - start) * sizeof(LEUnicode),
                    layout);

                ascent = (ascent > layout->getAscent() ? ascent : layout->getAscent());
                descent = (descent > layout->getDescent() ? descent : layout->getDescent());
                leading = (leading > layout->getLeading() ? leading : layout->getLeading());

                AddParagraph(paragraph);
            }

            if (*end == 0) {
                break;
            }

            start = skipLineEnd(end);
        }

        m_LineHeight = ascent + descent + leading;
        m_Ascent = ascent;
    }

    CParagraph::CParagraph()
    {
        m_IsReflowed = IZ_FALSE;
    }

    CParagraph::~CParagraph()
    {
        delete m_Layout;
    }

    CLine* CParagraph::CreateLine(IZ_UINT width)
    {
        if (!m_IsReflowed)
        {
            m_Layout->reflow();
            m_IsReflowed = IZ_TRUE;
        }

        CLine* ret = IZ_NULL;

        ParagraphLayout::Line* line = m_Layout->nextLine(width);
        if (line != IZ_NULL)
        {
            ret = CLine::CreateLine(
                m_Allocator,
                m_Host,
                line);
        }

        return ret;

    }

    CParagraph* CParagraphGroup::CreateParagraph(
        IMemoryAllocator* allocator,
        izanagi::text::IFontHost* host, 
        izanagi::text::E_FONT_CHAR_ENCODE encode,
        void* text,
        IZ_UINT bytes,
        void* data)
    {
        CParagraph* ret = (CParagraph*)izanagi::text::CParagraph::CreateParagraph<CParagraph>(
            allocator,
            host,
            encode,
            text,
            bytes,
            data);
        return ret;
    }

    IZ_BOOL CParagraph::Init(
        izanagi::text::IFontHost* host,
        izanagi::text::E_FONT_CHAR_ENCODE encode,
        void* text,
        IZ_UINT bytes,
        void* userData)
    {
        SAFE_REPLACE(m_Host, host);
        ParagraphLayout* layout = (ParagraphLayout*)userData;
        m_Layout = layout;
        return IZ_TRUE;
    }

    CLine::CLine()
    {
    }

    CLine::~CLine()
    {
        delete m_Line;
    }

    CLine* CLine::CreateLine(
        IMemoryAllocator* allocator,
        izanagi::text::IFontHost* host,
        ParagraphLayout::Line* line)
    {
        CLine* ret = (CLine*)izanagi::text::CLine::CreateLine<izanagi::CLine>(
            allocator,
            host,
            (void*)line);
        return ret;
    }

    IZ_UINT CLine::GetLineWidth()
    {
        return m_Line->getWidth();
    }

    IZ_BOOL Init(void* userData)
    {
        return IZ_TRUE;
    }

    void CLine::Prepare(
        IZ_UINT8* dst,
        IZ_UINT pitch,
        IZ_UINT ascent,
        text::IFontHost* host)
    {
        IZ_UINT x = 1;
        IZ_UINT y = ascent;

        le_int32 runCount = m_Line->countRuns();
        le_int32 run;

        le_int32 out = 0;

        for (run = 0; run < runCount; run += 1)
        {
            const ParagraphLayout::VisualRun *visualRun = m_Line->getVisualRun(run);
            le_int32 glyphCount = visualRun->getGlyphCount();
            const LEFontInstance *font = visualRun->getFont();
            const LEGlyphID *glyphs = visualRun->getGlyphs();
            const float *positions = visualRun->getPositions();

            IZ_UINT posX = 0;
            IZ_UINT posY = ascent;

            for (le_int32 g = 0; g < glyphCount; g += 1)
            {
                TTGlyphID ttGlyph = (TTGlyphID) LE_GET_GLYPH(glyphs[g]);

                if (ttGlyph < 0xFFFE)
                {
                    posX = CopyImage(
                        ttGlyph,
                        dst,
                        posX,
                        pitch,
                        ascent);
                }
            }
        }
    }
}