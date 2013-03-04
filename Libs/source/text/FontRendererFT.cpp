#include "text/FontRendererFT.h"
#include "text/FontHostFT.h"
#include "text/ParagraphGroup.h"
#include "izGraph.h"

namespace izanagi
{
namespace text
{
    CFontRendererFT* CFontRendererFT::CreateFontRenderer(
        IMemoryAllocator* allocator,
        IInputStream* in,
        IZ_UINT pixelSize)
    {
        void* buf = ALLOC(allocator, sizeof(CFontRendererFT));

        CFontRendererFT* instance = new(buf) CFontRendererFT;
        {
            instance->AddRef();

            instance->m_Allocator = allocator;
            instance->m_FontHost = CFontHostFT::CreateFontHostFT(
                allocator,
                in,
                pixelSize);
        }

        return instance;
    }

    CFontRendererFT::CFontRendererFT()
    {
        m_Allocator = IZ_NULL;
        m_FontHost = IZ_NULL;
    }

    CFontRendererFT::~CFontRendererFT()
    {
        SAFE_RELEASE(m_FontHost);
    }

    void CFontRendererFT::Render(
        IZ_UINT x, IZ_UINT y,
        IZ_UINT width, IZ_UINT height,
        CParagraphGroup& paragraphGroup,
        graph::CGraphicsDevice* device)
    {
        paragraphGroup.Layout(x, y);
        paragraphGroup.Prepare(m_FontHost, device);
        paragraphGroup.Render(width, height, device);
    }
}    // namespace text
}    // namespace izanagi
