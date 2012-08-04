#if !defined(__STATE_POINT_LIGHT_H__)
#define __STATE_POINT_LIGHT_H__

#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "StateBase.h"

class CStatePointLight : public CStateBase {
public:
	CStatePointLight(
		izanagi::sample::CSampleApp* app,
		izanagi::SCameraParam& camera);
	virtual ~CStatePointLight();

public:
	// 描画.
	virtual IZ_BOOL Render(izanagi::CGraphicsDevice* device);

	// 開始
	virtual IZ_BOOL Enter(
		izanagi::IMemoryAllocator* allocator,
		void* val);

	// ステートから抜ける（終了）.
	virtual IZ_BOOL Leave();

private:
    void RenderScene(
        izanagi::CGraphicsDevice* device,
        izanagi::CDebugMesh* mesh,
        const izanagi::SVector& position);

protected:
	izanagi::CShaderBasic* m_Shader;

    izanagi::SPointLightParam m_PointLight;

	izanagi::CDebugMesh* m_Light;

    izanagi::CDebugMesh* m_Sphere;
    izanagi::CDebugMesh* m_Cube;
    izanagi::CDebugMesh* m_Plane;
};

#endif	// #if !defined(__STATE_POINT_LIGHT_H__)
