#if !defined(__IZANAGI_GRAPH_PIXEL_SHADER_H__)
#define __IZANAGI_GRAPH_PIXEL_SHADER_H__

#include "izStd.h"

namespace izanagi
{
namespace graph
{
    /** ピクセルシェーダ
     */
    class CPixelShader : public CObject {
    protected:
        CPixelShader() { m_Allocator = IZ_NULL; }
        virtual ~CPixelShader() {}

        NO_COPIABLE(CPixelShader);

        IZ_DEFINE_INTERNAL_RELEASE();

    protected:
        IMemoryAllocator* m_Allocator;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_PIXEL_SHADER_H__)
