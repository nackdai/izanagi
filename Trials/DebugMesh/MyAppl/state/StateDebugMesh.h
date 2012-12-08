#if !defined(__STATE_DEBUG_MESH_H__)
#define __STATE_DEBUG_MESH_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izDebugUtil.h"
#include "shader/BasicShader.h"
#include "izSceneGraph.h"

class CStateDebugMesh : public izanagi::CSceneStateBase {
public:
	CStateDebugMesh();
	~CStateDebugMesh();

public:
	IZ_BOOL Init();
	IZ_BOOL Render(izanagi::graph::CGraphicsDevice* device);
	IZ_BOOL Update();
	IZ_BOOL Destroy();
	IZ_BOOL Enter(izanagi::IMemoryAllocator* allocator, void* val);
	IZ_BOOL Leave();

	IZ_BOOL OnKeyDown(IZ_UINT nChar);
	IZ_BOOL OnMouseMove(IZ_INT x, IZ_INT y);
	IZ_BOOL OnMouseWheel(IZ_SHORT zDelta); 

protected:
	IZ_BOOL Render2D();
	IZ_BOOL Render3D();

protected:
	CBasicShader* m_pShader;

	izanagi::graph::CTexture* m_pTex;

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
