#if !defined(__TAA_APP_H__)
#define __TAA_APP_H__

#include "izSampleKit.h"
#include "izSceneGraph.h"
#include "izDebugUtil.h"

#include "GBuffer.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class TAAApp : public izanagi::sample::CSampleApp {
public:
	TAAApp();
	virtual ~TAAApp();

public:
	// 初期化.
	virtual IZ_BOOL InitInternal(
		izanagi::IMemoryAllocator* allocator,
		izanagi::graph::CGraphicsDevice* device,
		izanagi::sample::CSampleCamera& camera) override;

	// 解放.
	virtual void ReleaseInternal() override;

	// 更新.
	virtual void UpdateInternal(izanagi::graph::CGraphicsDevice* device) override;

	// 描画.
	virtual void RenderInternal(izanagi::graph::CGraphicsDevice* device) override;

private:
	void renderGeometryPass(izanagi::graph::CGraphicsDevice* device);
	void renderColorPass(izanagi::graph::CGraphicsDevice* device);
	void renderTAAPass(izanagi::graph::CGraphicsDevice* device);

protected:
	izanagi::sample::ObjModel* m_obj{ nullptr };

	izanagi::sample::Shader m_shdGeometryPass;
	izanagi::sample::Shader m_shdColorPass;
	izanagi::sample::Shader m_shdTAAPass;

	GBuffer m_gbuffer;

	int m_curRt{ 0 };
	izanagi::graph::CRenderTarget* m_rt[2];

	izanagi::CDebugMesh* m_screenFillPlane{ nullptr };

	izanagi::math::CMatrix44 m_mtxL2W;
	izanagi::math::CMatrix44 m_mtxOffset;

	IZ_UINT m_frame{ 0 };

	izanagi::math::CMatrix44 m_mtxPrevL2W;
	izanagi::math::CMatrix44 m_mtxPrevW2C;
};

#endif    // #if !defined(__TAA_APP_H__)
