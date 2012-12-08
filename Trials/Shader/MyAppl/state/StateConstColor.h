#if !defined(__STATE_CONST_COLOR_H__)
#define __STATE_CONST_COLOR_H__

#include "izDefs.h"
#include "izStd.h"
#include "izShader.h"
#include "izDebugUtil.h"
#include "izSceneGraph.h"

class CStateConstColor : public izanagi::CSceneStateBase {
public:
	CStateConstColor() {}
	~CStateConstColor() {}

public:
	IZ_BOOL Init();
	IZ_BOOL Render(izanagi::graph::CGraphicsDevice* device);
	IZ_BOOL Update();
	IZ_BOOL Destroy();
	IZ_BOOL Enter(izanagi::IMemoryAllocator* allocator, void* val);
	IZ_BOOL Leave();

	//IZ_BOOL OnKeyDown(IZ_UINT nChar);
	//IZ_BOOL OnMouseMove(IZ_INT x, IZ_INT y);
	//IZ_BOOL OnMouseWheel(IZ_SHORT zDelta); 

protected:
	void Render2D();
	void Render3D();

protected:
	izanagi::CDebugMesh* m_pMesh;
	izanagi::CShaderBasic* m_pShader;

	izanagi::IZ_SHADER_HANDLE m_hL2W;
	izanagi::IZ_SHADER_HANDLE m_hW2C;
};

#endif	// #if !defined(__STATE_CONST_COLOR_H__)
