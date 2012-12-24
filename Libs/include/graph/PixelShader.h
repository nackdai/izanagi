#if !defined(__IZANAGI_GRAPH_PIXEL_SHADER_H__)
#define __IZANAGI_GRAPH_PIXEL_SHADER_H__

#include "ShaderBase.h"

namespace izanagi
{
namespace graph
{
    /** ピクセルシェーダ
     */
    class CPixelShader : public IShader {
    protected:
        CPixelShader() {}
        virtual ~CPixelShader() {}
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_PIXEL_SHADER_H__)
