#if !defined(__IZANAGI_GRAPH_RENDER_TARGET_GLES2_H__)
#define __IZANAGI_GRAPH_RENDER_TARGET_GLES2_H__

#include "graph/GraphDefs.h"
#include "graph/RenderTarget.h"
#include "graph/gles2/TextureProxy.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceGLES2;

    class CRenderTargetGLES2 : public CTextureProxy<CRenderTarget>
    {
        friend class CGraphicsDeviceGLES2;
		friend class CFrameBufferObject;

    private:
        // レンダーターゲット作成
        static CRenderTargetGLES2* CreateRenderTarget(
            CGraphicsDeviceGLES2* device,
            IMemoryAllocator* allocator,
            IZ_UINT width,
            IZ_UINT height,
            E_GRAPH_PIXEL_FMT fmt);

        // レンダーターゲット作成
        static CRenderTargetGLES2* CreateDepthStencilRenderTarget(
            CGraphicsDeviceGLES2* device,
            IMemoryAllocator* allocator,
            IZ_UINT width, 
            IZ_UINT height);

        static CRenderTargetGLES2* CreateDummyRenderTarget(
            IMemoryAllocator* allocator,
            IZ_UINT width,
            IZ_UINT height);

    private:
        inline CRenderTargetGLES2();
        virtual inline ~CRenderTargetGLES2();

    private:
        // 本体作成（レンダーターゲット）
        IZ_BOOL CreateBody_RenderTarget(
            IZ_UINT width,
            IZ_UINT height,
            E_GRAPH_PIXEL_FMT fmt);

        IZ_BOOL CreateBody_DepthStencilRenderTarget(
            IZ_UINT width,
            IZ_UINT height);

        // テクスチャ情報取得
         void SetTextureInfo(
            IZ_UINT width,
            IZ_UINT height,
            E_GRAPH_PIXEL_FMT fmt);

		 bool isSpecifiedClearClr() const
		 {
			 return m_isSpecifedClearClr;
		 }

		 const float* getClearColor() const
		 {
			 return m_clearClr;
		 }

    public:
        virtual IZ_BOOL IsPrepared() const;

        virtual IZ_BOOL Disable();

        virtual IZ_BOOL Restore();

		// ロック
		virtual IZ_UINT Lock(
			IZ_UINT level,
			void** data,
			IZ_BOOL isReadOnly,
			IZ_BOOL isDiscard = IZ_FALSE) override;

		// アンロック
		virtual IZ_BOOL Unlock(IZ_UINT level) override;

		virtual void setClearColor(float r, float g, float b, float a) override final
		{
			m_isSpecifedClearClr = true;
			m_clearClr[0] = r;
			m_clearClr[1] = g;
			m_clearClr[2] = b;
			m_clearClr[3] = a;
		}

    private:
        virtual TEX_HANDLE GetTexHandle()
        {
            return m_Texture;
        }

    private:
        CGraphicsDeviceGLES2* m_Device;

        // 本体
        GLuint m_Texture;

		void* m_TemporaryData{ nullptr };
		IZ_UINT m_allocatedSize{ 0 };

		bool m_isSpecifedClearClr{ false };
		float m_clearClr[4];
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_RENDER_TARGET_GLES2_H__)
