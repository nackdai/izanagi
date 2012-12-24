#if !defined(__IZANAGI_GRAPH_PIXEL_SHADER_DX9_H__)
#define __IZANAGI_GRAPH_PIXEL_SHADER_DX9_H__

#include "graph/PixelShader.h"
#include "graph/dx9/ShaderProxy.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceDX9;

    /** ピクセルシェーダ
     */
    class CPixelShaderDX9 : public CShaderProxy<CPixelShader>
    {
        friend class CGraphicsDeviceDX9;

    private:
        // インスタンス作成
        static CPixelShader* CreatePixelShader(
            CGraphicsDeviceDX9* device,
            IMemoryAllocator* allocator,
            const void* program);

    protected:
        inline CPixelShaderDX9();
        virtual inline ~CPixelShaderDX9();

    public:
        D3D_PS* GetRawInterface() { return m_PS; }

    protected:
        // 本体
        D3D_PS* m_PS;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_PIXEL_SHADER_DX9_H__)
