#if !defined(__STATE_DEBUG_MESH_H__)
#define __STATE_DEBUG_MESH_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izDebugUtil.h"
#include "shader/BasicShader.h"

class CStateDebugMesh : public izanagi::CGameState {
public:
	CStateDebugMesh();
	~CStateDebugMesh();

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
	izanagi::CDebugMeshCylinder* m_pCylinder;
	izanagi::CDebugMeshBox* m_pBox;
	izanagi::CDebugMeshTorus* m_pTorus;
	izanagi::CDebugMeshRectangle* m_pRect;
	izanagi::CDebugMeshAxis* m_pAxis;
	izanagi::CDebugMeshFrustum* m_pFrustum;

	izanagi::CDebugMesh* m_MeshList[6];
	IZ_INT m_CurMesh;

	union {
		struct {
			IZ_UINT32 isDrawDebugAxis	: 1;
		};
		IZ_UINT32 val;
	} m_Flags;
};

#endif	// #if !defined(__STATE_DEBUG_MESH_H__)
