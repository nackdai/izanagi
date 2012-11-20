#if !defined(__TEST_MOUSE_HIT_H__)
#define __TEST_MOUSE_HIT_H__

#include "izSampleKit.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class CMouseHit : public izanagi::sample::CSampleApp {
public:
	CMouseHit();
	virtual ~CMouseHit();

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
	struct {
		izanagi::CRectangle rc;
		izanagi::CDebugMeshRectangle* mesh;
		izanagi::SMatrix mtx;
	} m_Rectangles[4];

	izanagi::CShaderBasic* m_Shader;

	IZ_BOOL m_IsPressKey;
	izanagi::CIntPoint m_MousePoint;
	IZ_INT m_CrossRectIdx;
};

#endif	// #if !defined(__TEST_MOUSE_HIT_H__)