#include "font/FontRenderer.h"
#include "font/FontRendererBmp.h"
#include "unIo.h"
#include "unGraph.h"

using namespace uranus;

/**
*/
CFontRenderer* CFontRenderer::CreateFontRendererBmp(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice,
	UN_UINT	nMaxRegisterNum,
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
	CGraphicsDevice* pDevice,
	UN_UINT	nMaxRegisterNum)
{
	// TODO
	UN_ASSERT(UN_FALSE);
	return UN_NULL;
}
