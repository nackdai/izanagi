#if !defined(__SHADER_APP_H__)
#define __SHADER_APP_H__

#include "izSampleKit.h"

class CShaderApp : public izanagi::sample::CSampleApp {
public:
	CShaderApp();
	virtual ~CShaderApp();

protected:
	// 初期化.
	virtual IZ_BOOL InitInternal(
		izanagi::IMemoryAllocator* allocator,
		izanagi::CGraphicsDevice* device,
		izanagi::sample::CSampleCamera& camera);

	// 解放.
	virtual void ReleaseInternal();

	// 更新.
	virtual void UpdateInternal(izanagi::CCamera& camera);

	// 描画.
	virtual void RenderInternal(izanagi::CGraphicsDevice* device);
};

#endif	// #if !defined(__SHADER_APP_H__)