#if !defined(__STATE_CONST_COLOR_H__)
#define __STATE_CONST_COLOR_H__

#include "izDefs.h"
#include "izStd.h"
#include "izShader.h"
#include "izDebugUtil.h"

class CStateConstColor : public izanagi::CGameState {
public:
	CStateConstColor() {}
	~CStateConstColor() {}

public:
	IZ_BOOL Create();
	IZ_BOOL Render();
	IZ_BOOL Update();
	IZ_BOOL Destroy();
	IZ_BOOL Enter();
	IZ_BOOL Leave();

	//IZ_BOOL OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags);
	//IZ_BOOL OnMouseMove(IZ_UINT nFlags, IZ_INT x, IZ_INT y);
	//IZ_BOOL OnMouseWheel(IZ_UINT nFlags, IZ_SHORT zDelta, IZ_INT x, IZ_INT y); 

protected:
	void Render2D();
	void Render3D();

protected:
	izanagi::CDebugMesh* m_pMesh;
	izanagi::CShaderBasic* m_pShader;

	izanagi::IZ_SHADER_HANDLE m_hL2W;
	izanagi::IZ_SHADER_HANDLE m_hW2C;
};

#endif	// #if !defined(__STATE_CONST_COLOR_H__)
