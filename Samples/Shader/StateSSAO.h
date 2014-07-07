#if !defined(__STATE_SSAO_H__)
#define __STATE_SSAO_H__

#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "StateBase.h"

class CStateSSAO : public CStateBase {
public:
    CStateSSAO(
        izanagi::sample::CSampleApp* app,
        izanagi::SCameraParam& camera);
    virtual ~CStateSSAO();

public:
    // 描画.
    virtual IZ_BOOL Render(izanagi::graph::CGraphicsDevice* device);

    // 開始
    virtual IZ_BOOL Enter(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::CValue& arg);

    // ステートから抜ける（終了）.
    virtual IZ_BOOL Leave();

private:
    void RenderScene(
        izanagi::graph::CGraphicsDevice* device,
        izanagi::CDebugMesh* mesh,
        const izanagi::math::SVector& position);

protected:
    izanagi::shader::CShaderBasic* m_Shader;

    izanagi::CDebugMesh* m_Sphere;
    izanagi::CDebugMesh* m_Cube;
    izanagi::CDebugMesh* m_Plane;

    izanagi::math::SMatrix m_L2W;
    izanagi::SParallelLightParam m_ParallelLight;
};

#endif    // #if !defined(__STATE_SSAO_H__)
