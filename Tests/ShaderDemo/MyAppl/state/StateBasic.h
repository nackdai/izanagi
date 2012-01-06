#if !defined(__STATE_DEBUG_BASIC_H__)
#define __STATE_DEBUG_BASIC_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izDebugUtil.h"
#include "shader/BasicShader.h"
#include "izSceneGraph.h"

class CStateBasic : public izanagi::CSceneStateBase {
public:
	CStateBasic();
	~CStateBasic();

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
	IZ_BOOL Render2D();
	IZ_BOOL Render3D();

protected:
	CBasicShader* m_pShader;

	izanagi::CDebugMeshGrid* m_pGrid;
	izanagi::CDebugMeshSphere* m_pSphere;
};

#endif	// #if !defined(__STATE_DEBUG_BASIC_H__)
