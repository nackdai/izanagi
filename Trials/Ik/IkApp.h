#if !defined(__TEST_IK_APP_H__)
#define __TEST_IK_APP_H__

#include "izSampleKit.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class CIkApp : public izanagi::sample::CSampleApp {
public:
	CIkApp();
	virtual ~CIkApp();

protected:
	// 初期化.
	virtual IZ_BOOL InitInternal(
		izanagi::IMemoryAllocator* allocator,
		izanagi::graph::CGraphicsDevice* device,
		izanagi::sample::CSampleCamera& camera);

	// 解放.
	virtual void ReleaseInternal();

	// 更新.
	virtual void UpdateInternal(
        izanagi::CCamera& camera,
        izanagi::graph::CGraphicsDevice* device);

	// 描画.
	virtual void RenderInternal(izanagi::graph::CGraphicsDevice* device);

	virtual IZ_BOOL OnMouseLBtnDown(const izanagi::CIntPoint& point);
	virtual IZ_BOOL OnKeyDown(IZ_UINT nChar);
};

#endif	// #if !defined(__TEST_IK_APP_H__)