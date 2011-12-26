#if !defined(__STATE_DEBUG_BASIC_H__)
#define __STATE_DEBUG_BASIC_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izDebugUtil.h"
#include "shader/BasicShader.h"

class CStateBasic : public izanagi::CGameState {
public:
	CStateBasic();
	~CStateBasic();

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
	IZ_BOOL Render2D();
	IZ_BOOL Render3D();

protected:
	CBasicShader* m_pShader;

	izanagi::CTexture* m_pTex;

	izanagi::CDebugMeshGrid* m_pGrid;
	izanagi::CDebugMeshSphere* m_pSphere;
};

#endif	// #if !defined(__STATE_DEBUG_BASIC_H__)
