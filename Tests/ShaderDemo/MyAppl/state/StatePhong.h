#if !defined(__STATE_DEBUG_PHONG_H__)
#define __STATE_DEBUG_PHONG_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izDebugUtil.h"
#include "shader/PhongShader.h"
#include "izSceneGraph.h"

class CStatePhong : public izanagi::CSceneStateBase {
public:
	CStatePhong();
	~CStatePhong();

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
	CPhongShader* m_pShader;

	izanagi::CDebugMeshGrid* m_pGrid;
	izanagi::CDebugMeshAxis* m_pAxis;

	izanagi::CDebugMeshSphere* m_pMesh;
};

#endif	// #if !defined(__STATE_DEBUG_PHONG_H__)
