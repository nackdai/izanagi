#if !defined(__SHADOW_H__)
#define __SHADOW_H__

#include "izSampleKit.h"

class CShadow : public izanagi::CObject {
public:
	static CShadow* Create(
		izanagi::IMemoryAllocator* allocator,
		izanagi::CGraphicsDevice* device,
		IZ_UINT width,
		IZ_UINT height);

public:
	CShadow();
	~CShadow();

	IZ_DEFINE_INTERNAL_RELEASE();

public:
	IZ_BOOL BeginShadowRender(
		izanagi::CGraphicsDevice* device,
		const izanagi::SVector& lightPos,
		const izanagi::SVector& lightDir);

	void EndShadowRender(izanagi::CGraphicsDevice* device);

	izanagi::CTexture* GetRT() { return m_RT; }
	izanagi::CSurface* GetDepthRT() { return m_DepthRT; }

	const izanagi::SMatrix& GetLightViewProjMtx() const;
	const izanagi::SMatrix& GetShadowTexMtx() const;

private:
	izanagi::IMemoryAllocator* m_Allocator;

	izanagi::CTexture* m_RT;
	izanagi::CSurface* m_DepthRT;

	izanagi::CCamera m_Light;

	static const izanagi::E_GRAPH_PIXEL_FMT fmtRT = izanagi::E_GRAPH_PIXEL_FMT_RGBA8;
	static const izanagi::E_GRAPH_PIXEL_FMT fmtDepth = izanagi::E_GRAPH_PIXEL_FMT_D24S8;
};

#endif	// #if !defined(__SHADOW_H__)