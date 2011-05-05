#if !defined(__STATE_BASIC_H__)
#define __STATE_BASIC_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "izShader.h"

//////////////////////////////////////////////////////

class CShaderTest : public izanagi::CShaderBasic {
	friend class izanagi::CShader;

protected:
	CShaderTest() {}
	~CShaderTest() {}

public:
	izanagi::IZ_SHADER_HANDLE GetParameterByName(IZ_PCSTR pszName);
	izanagi::IZ_SHADER_HANDLE GetParameterBySemantic(IZ_PCSTR pszSemantic);
};

//////////////////////////////////////////////////////

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
	void Render2D();
	void Render3D();

protected:
	izanagi::CShaderBasic* m_pShader;

	izanagi::CDebugMeshAxis* m_pAxis;

	izanagi::CTexture* m_pTex[2];

	izanagi::CModel* m_pMdl;
	izanagi::CMesh* m_pMsh;
	izanagi::CSkeleton* m_pSkl;
	izanagi::CMaterial* m_pMtrl[2];

	izanagi::CGeometrySorter* m_GeomSorter;
	izanagi::CScene* m_Scene;
};

#endif	// #if !defined(__STATE_BASIC_H__)
