﻿#if !defined(__STATE_DEBUG_NORMAL_MAP_H__)
#define __STATE_DEBUG_NORMAL_MAP_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izDebugUtil.h"
#include "shader/NormalMapShader.h"

class CStateNormalMap : public izanagi::CGameState {
public:
	CStateNormalMap();
	~CStateNormalMap();

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
	CNormalMapShader* m_pShader;

	izanagi::CTexture* m_pTex;
	izanagi::CTexture* m_pNormalMap;

	izanagi::CDebugMeshGrid* m_pGrid;
	izanagi::CDebugMeshAxis* m_pAxis;
	//izanagi::CDebugMeshSphere* m_pMesh;
	izanagi::CDebugMeshRectangle* m_pMesh;
};

#endif	// #if !defined(__STATE_DEBUG_NORMAL_MAP_H__)
