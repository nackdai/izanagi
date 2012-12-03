#if !defined(__TEST_MOUSE_HIT_H__)
#define __TEST_MOUSE_HIT_H__

#include "izSampleKit.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class CScissoring : public izanagi::sample::CSampleApp {
public:
	CScissoring();
	virtual ~CScissoring();

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

	virtual IZ_BOOL OnKeyDown(IZ_UINT nChar);
	virtual void OnKeyUp(IZ_UINT nChar);
	virtual IZ_BOOL OnMouseLBtnDown(const izanagi::CIntPoint& point);

private:
	struct SRectangle {
		izanagi::math::CRectangle rc;
		izanagi::CDebugMeshRectangle* mesh;
		izanagi::math::SMatrix mtx;
	} m_Rectangle;

	struct STriangle {
		izanagi::math::CTriangle tri;
		izanagi::math::SMatrix mtx;
		izanagi::graph::CVertexBuffer* defaultVB;
		izanagi::graph::CVertexBuffer* vb[2];
		izanagi::graph::CVertexDeclaration* vd;
	} m_Triangles;

	IZ_UINT m_TriNum;
	izanagi::math::CTriangle m_NewTri[2];

	izanagi::CShaderBasic* m_Shader;

	IZ_BOOL m_EnableScissoring;
	IZ_BOOL m_CopiedVtx;
};

#endif	// #if !defined(__TEST_MOUSE_HIT_H__)