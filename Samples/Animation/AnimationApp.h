#if !defined(__ANIMATION_APP_H__)
#define __ANIMATION_APP_H__

#include "izSampleKit.h"
#include "izAnimation.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class CAnimationApp : public izanagi::sample::CSampleApp {
public:
	CAnimationApp();
	virtual ~CAnimationApp();

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
	izanagi::CImage* m_Img;
	izanagi::CModel* m_Mdl;
	izanagi::CMesh* m_Msh;
	izanagi::CSkeleton* m_Skl;
	izanagi::CShaderBasic* m_Shd;
	izanagi::CAnimation* m_Anm;

    izanagi::animation::CTimeline m_Timeline;
};

#endif	// #if !defined(__ANIMATION_APP_H__)
