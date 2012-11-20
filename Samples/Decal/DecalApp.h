#if !defined(__DECAL_APP_H__)
#define __DECAL_APP_H__

#include "izSampleKit.h"
#include "Sphere.h"
#include "Cube.h"
#include "Decal.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class CDecalApp : public izanagi::sample::CSampleApp {
public:
	CDecalApp();
	virtual ~CDecalApp();

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

	virtual IZ_BOOL OnKeyDown(IZ_UINT nChar);
	virtual void OnKeyUp(IZ_UINT nChar);
	virtual IZ_BOOL OnMouseLBtnDown(const izanagi::CIntPoint& point);

private:
	CSphere* m_Sphere;
	//CCube* m_Sphere;
	CDecal* m_Decal;

	izanagi::CImage* m_Img;

	izanagi::CShaderBasic* m_Shader;
};

#endif	// #if !defined(__DECAL_APP_H__)