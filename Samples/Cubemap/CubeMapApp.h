#if !defined(__CUBE_MAP_APP_H__)
#define __CUBE_MAP_APP_H__

#include "izSampleKit.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class CCubeMapApp: public izanagi::sample::CSampleApp {
public:
	CCubeMapApp();
	virtual ~CCubeMapApp();

protected:
	// 初期化.
	virtual IZ_BOOL InitInternal(
		izanagi::IMemoryAllocator* allocator,
		izanagi::CGraphicsDevice* device,
		izanagi::sample::CSampleCamera& camera);

	// 解放.
	virtual void ReleaseInternal();

	// 更新.
	virtual void UpdateInternal(
		izanagi::CCamera& camera,
		izanagi::CGraphicsDevice* device);

	// 描画.
	virtual void RenderInternal(izanagi::CGraphicsDevice* device);

private:
	izanagi::sample::CSampleEnvBox* m_Cube;

	izanagi::CImage* m_Img;
	izanagi::CShaderBasic* m_Shader;

	izanagi::SMatrix m_L2W;
};

#endif	// #if !defined(__CUBE_MAP_APP_H__)