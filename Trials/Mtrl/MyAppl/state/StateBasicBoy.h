#if !defined(__STATE_BASIC_BOY_H__)
#define __STATE_BASIC_BOY_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "izShader.h"

class CStateBasicBoy : public izanagi::CSceneStateBase {
public:
	CStateBasicBoy();
	~CStateBasicBoy();

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
	void Render2D();
	void Render3D();

protected:
	izanagi::CShaderBasic* m_pShader;

	izanagi::CDebugMeshAxis* m_pAxis;

	izanagi::graph::CTexture* m_pTex;

	izanagi::CModel* m_pMdl;
	izanagi::CMesh* m_pMsh;
	izanagi::CSkeleton* m_pSkl;
	izanagi::CMaterial* m_pMtrl[4];

	izanagi::CGeometrySorter* m_GeomSorter;
	izanagi::CSceneRenderer* m_Scene;
};

#endif	// #if !defined(__STATE_BASIC_BOY_H__)
