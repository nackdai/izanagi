#if !defined(__IZANAGI_TEXT_TEXT_LINE_H__)
#define __IZANAGI_TEXT_TEXT_LINE_H__

#include "izDefs.h"
#include "izStd.h"
#include "FontHost.h"

namespace izanagi
{
    namespace graph
    {
        class CGraphicsDevice;
        class CTexture;
    }

namespace text
{
    /**
     */
    class CLine : public CObject
    {
        friend class CParagraph;

    protected:
        CLine();
        virtual ~CLine();

        NO_COPIABLE(CLine);
        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        PURE_VIRTUAL(IZ_UINT GetLineWidth());

    protected:
        void Prepare(
            IZ_UINT height,
            IZ_UINT ascent,
            IFontHost* host,
            graph::CGraphicsDevice* device);

        PURE_VIRTUAL(
            void Prepare(
                IZ_UINT8* dst,
                IZ_UINT pitch,
                IZ_UINT ascent,
                IFontHost* host));

        void Clear();

        void Render(
            IZ_UINT x,
            IZ_UINT y,
            graph::CGraphicsDevice* device);

    private:
        CStdList<CLine>::Item* GetListItem() { return &m_ListItem; }

    protected:
        IMemoryAllocator* m_Allocator;

        graph::CTexture* m_Texture;

        CStdList<CLine>::Item m_ListItem;
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_TEXT_LINE_H__)
