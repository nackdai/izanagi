#if !defined(__STATE_FONT_H__)
#define __STATE_FONT_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izFont.h"

class CStateFont : public izanagi::CGameState {
public:
	CStateFont();
	~CStateFont();

public:
	IZ_BOOL Create();
	IZ_BOOL Render();
	IZ_BOOL Update();
	IZ_BOOL Destroy();
	IZ_BOOL Enter();
	IZ_BOOL Leave();

	IZ_BOOL OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags);
	IZ_BOOL OnMouseMove(IZ_UINT nFlags, IZ_INT x, IZ_INT y);
	IZ_BOOL OnMouseWheel(IZ_UINT nFlags, IZ_SHORT zDelta, IZ_INT x, IZ_INT y); 

protected:
	izanagi::CTexture* m_pTex;
	izanagi::CFontRenderer* m_pFontRenderer;
};

#endif	// #if !defined(__STATE_FONT_H__)
