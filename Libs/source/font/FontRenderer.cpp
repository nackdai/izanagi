#include "font/FontRenderer.h"
#include "font/FontRendererBmp.h"
#include "izIo.h"
#include "izGraph.h"

using namespace izanagi;

/**
*/
CFontRenderer* CFontRenderer::CreateFontRendererBmp(
	IMemoryAllocator* pAllocator,
	graph::CGraphicsDevice* pDevice,
	IZ_UINT	nMaxRegisterNum,
	IInputStream* in)
{
	CFontRenderer* ret = CFontRendererBmp::CreateFontRendererBmp(
							pAllocator,
							pDevice,
							nMaxRegisterNum,
							in);
	return ret;
}

/**
*/
CFontRenderer* CreateFontRendererFT(
	IMemoryAllocator* pAllocator,
	graph::CGraphicsDevice* pDevice,
	IZ_UINT	nMaxRegisterNum)
{
	// TODO
	IZ_ASSERT(IZ_FALSE);
	return IZ_NULL;
}
