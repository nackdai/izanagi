#if !defined(__STATE_FONT_H__)
#define __STATE_FONT_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izFont.h"
#include "izSceneGraph.h"

class CStateFont : public izanagi::CSceneStateBase {
public:
	CStateFont();
	~CStateFont();

public:
	IZ_BOOL Init();
	IZ_BOOL Render(izanagi::CGraphicsDevice* device);
	IZ_BOOL Update();
	IZ_BOOL Destroy();
	IZ_BOOL Enter(izanagi::IMemoryAllocator* allocator, void* val);
	IZ_BOOL Leave();

	IZ_BOOL OnKeyDown(IZ_UINT nChar);
	IZ_BOOL OnMouseMove(IZ_INT x, IZ_INT y);
	IZ_BOOL OnMouseWheel(IZ_SHORT zDelta); 

protected:
	izanagi::CTexture* m_pTex;
	izanagi::CFontRenderer* m_pFontRenderer;
};

#endif	// #if !defined(__STATE_FONT_H__)
