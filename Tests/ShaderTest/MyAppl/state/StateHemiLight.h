#if !defined(__STATE_HEMI_LIGHT_H__)
#define __STATE_HEMI_LIGHT_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izDebugUtil.h"
#include "shader/HemiLightShader.h"
#include "izSceneGraph.h"

class CStateHemiLight : public izanagi::CSceneStateBase {
public:
	CStateHemiLight();
	~CStateHemiLight();

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
	CHemiLightShader* m_pShader;

	izanagi::graph::CTexture* m_pTex;

	izanagi::CDebugMeshGrid* m_pGrid;
	izanagi::CDebugMeshSphere* m_pSphere;
};

#endif	// #if !defined(__STATE_HEMI_LIGHT_H__)
