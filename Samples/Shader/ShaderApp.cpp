#include "ShaderApp.h"

CShaderApp::CShaderApp()
{
}

CShaderApp::~CShaderApp()
{
}

// 初期化.
IZ_BOOL CShaderApp::InitInternal(
	izanagi::IMemoryAllocator* allocator,
	izanagi::CGraphicsDevice* device,
	izanagi::sample::CSampleCamera& camera)
{
	// TODO
	return IZ_TRUE;
}

// 解放.
void CShaderApp::ReleaseInternal()
{
}

// 更新.
void CShaderApp::UpdateInternal(izanagi::CCamera& camera)
{
	// Nothing is done...
}

// 描画.
void CShaderApp::RenderInternal(izanagi::CGraphicsDevice* device)
{
	static const IZ_COLOR bgColor = IZ_COLOR_RGBA(0, 128, 255, 255);

	device->BeginRender(
		izanagi::E_GRAPH_CLEAR_FLAG_ALL,
		bgColor, 1.0f, 0);
	{
	}
	device->EndRender();
}
