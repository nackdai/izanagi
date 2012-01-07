#if !defined(__STATE_PHONG_SHADER_H__)
#define __STATE_PHONG_SHADER_H__

#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "StateBase.h"

class CStatePhongShader : public CStateBase {
public:
	CStatePhongShader(
		izanagi::sample::CSampleApp* app,
		izanagi::SCameraParam& camera);
	virtual ~CStatePhongShader();

public:
	// 初期化.
	virtual IZ_BOOL Init();

	// 更新.
	virtual IZ_BOOL Update();

	// 描画.
	virtual IZ_BOOL Render(izanagi::CGraphicsDevice* device);

	// 開始
	virtual IZ_BOOL Enter(
		izanagi::IMemoryAllocator* allocator,
		void* val);

	// 終了.
	virtual IZ_BOOL Destroy();

	// ステートから抜ける（終了）.
	virtual IZ_BOOL Leave();

	// For Windows

	// キー押下
	virtual IZ_BOOL OnKeyDown(IZ_UINT nChar);

protected:
	izanagi::CShaderBasic* m_Shader;
	izanagi::CDebugMeshSphere* m_Sphere;

	izanagi::SMatrix m_L2W;
	izanagi::SParallelLightParam m_ParallelLight;
};

#endif	// #if !defined(__STATE_PHONG_SHADER_H__)
