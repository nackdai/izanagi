#if !defined(__STATE_BASIC_2_H__)
#define __STATE_BASIC_2_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "shader/BasicShader.h"
#include "shader/PhongShader.h"
#include "shader/SkinShader.h"

class CStateBasicEx2 : public izanagi::CSceneStateBase {
public:
	CStateBasicEx2();
	~CStateBasicEx2();

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
	void Render2D();
	void Render3D();

protected:
	//CBasicShader* m_pShader;
	//CPhongShader* m_pShader;
	CSkinShader* m_pShader;

	izanagi::CDebugMeshAxis* m_pAxis;

	izanagi::CTexture* m_pTex;

	izanagi::CModel* m_pMdl;
	izanagi::CMesh* m_pMsh;
	izanagi::CSkeleton* m_pSkl;

	izanagi::CAnimation* m_pAnm;
	izanagi::CStdTimeline m_Timeline;

	IZ_FLOAT m_fTime;
};

#endif	// #if !defined(__STATE_BASIC_2_H__)
