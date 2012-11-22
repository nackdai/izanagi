#if !defined(__POST_EFFECT_APP_H__)
#define __POST_EFFECT_APP_H__

#include "izSampleKit.h"
#include "StateManager.h"
#include "SceneRenderer.h"
#include "PostEffectSample.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class CPostEffectApp : public izanagi::sample::CSampleApp {
public:
	CPostEffectApp();
	virtual ~CPostEffectApp();

public:
	// キーボード押下
	virtual IZ_BOOL OnKeyDown(IZ_UINT nChar);

protected:
	// 初期化.
	virtual IZ_BOOL InitInternal(
		izanagi::IMemoryAllocator* allocator,
		izanagi::graph::CGraphicsDevice* device,
		izanagi::sample::CSampleCamera& camera);

	// 解放.
	virtual void ReleaseInternal();

	// 更新.
	virtual void UpdateInternal(izanagi::graph::CGraphicsDevice* device);

	// 描画.
	virtual void RenderInternal(izanagi::graph::CGraphicsDevice* device);

private:
	izanagi::IMemoryAllocator* m_Allocator;
	izanagi::graph::CGraphicsDevice* m_Device;

    IZ_INT m_CurIdx;
    IZ_BOOL m_IsChangeState;
};

#endif
