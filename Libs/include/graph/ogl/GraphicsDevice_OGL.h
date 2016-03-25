#if !defined(__IZANAGI_GRPAH_GRAPHICS_DEVICE_OGL_H__)
#define __IZANAGI_GRPAH_GRAPHICS_DEVICE_OGL_H__

#include "graph/gles2/GraphicsDevice_GLES2.h"
#include "graph/ogl/VertexDeclaration_OGL.h"

namespace izanagi {
namespace graph {
    class CGraphicsDeviceOGL : public CGraphicsDeviceGLES2 {
    public:
        CGraphicsDeviceOGL();
        virtual ~CGraphicsDeviceOGL();

    public:
        // 頂点バッファ作成
        virtual CVertexBuffer* CreateVertexBuffer(
            IZ_UINT nStride,
            IZ_UINT nVtxNum,
            E_GRAPH_RSC_USAGE nCreateType) override;

        // 頂点宣言作成
        virtual CVertexDeclaration* CreateVertexDeclaration(const SVertexElement* pElem, IZ_UINT nNum) override;

    public:
        virtual IZ_BOOL Reset(const void* initialParam) override;

        virtual IZ_BOOL BeginScene(
            CRenderTarget** pRT,
            IZ_UINT nCount,
            CRenderTarget* pDepth,
            IZ_DWORD nClearFlags,
            IZ_COLOR nClearColor = 0,
            IZ_FLOAT fClearZ = 1.0f,
            IZ_DWORD nClearStencil = 0) override;

        // シーン描画終了
        virtual void EndScene(IZ_UINT flag = 0xffffffff) override;

        // 頂点バッファセット
        virtual IZ_BOOL SetVertexBuffer(
            IZ_UINT nStreamIdx,
            IZ_UINT nOffsetByte,
            IZ_UINT nStride,
            CVertexBuffer* pVB) override;

        virtual IZ_BOOL SetVertexBufferInstanced(
            IZ_UINT streamIdx,
            IZ_UINT offsetByte,
            IZ_UINT stride,
            E_GRAPH_VB_USAGE usage,
            IZ_UINT divisor,
            CVertexBuffer* vb) override;

        virtual IZ_BOOL DrawIndexedInstancedPrimitive(
            E_GRAPH_PRIM_TYPE prim_type,
            IZ_UINT vtxOffset,
            IZ_UINT vtxNum,
            IZ_UINT idxOffset,
            IZ_UINT nPrimCnt) override;

        virtual IZ_BOOL DrawInstancedPrimitive(
            E_GRAPH_PRIM_TYPE prim_type,
            IZ_UINT idxOffset,
            IZ_UINT nPrimCnt) override;

        virtual IZ_BOOL CheckRenderTargetCount(IZ_UINT cnt) override
        {
            VRETURN(0 < cnt);
            return IZ_TRUE;
        }

    protected:
        virtual void OnTerminate() override;

    private:
        InstancingParam m_instancingParams[MAX_STREAM_NUM];
        IZ_UINT m_numInstancingPrim{ 0 };

        CRenderTarget* m_rtInternalDepth{ nullptr };
        IZ_BOOL m_isUseInternalDepth{ IZ_FALSE };
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRPAH_GRAPHICS_DEVICE_OGL_H__)
