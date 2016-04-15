#if !defined(__SSAO_APP_H__)
#define __SSAO_APP_H__

#include "izSampleKit.h"
#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "GBuffer.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class SSAOApp : public izanagi::sample::CSampleApp {
public:
    SSAOApp();
    virtual ~SSAOApp();

public:
    // 初期化.
    virtual IZ_BOOL InitInternal(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::sample::CSampleCamera& camera) override;

    // 解放.
    virtual void ReleaseInternal() override;

    // 更新.
    virtual void UpdateInternal(izanagi::graph::CGraphicsDevice* device) override;

    // 描画.
    virtual void RenderInternal(izanagi::graph::CGraphicsDevice* device) override;

    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key) override;

private:
    void RenderScene(
        izanagi::graph::CGraphicsDevice* device,
        izanagi::CDebugMesh* mesh,
        const izanagi::math::SVector4& position);

protected:
    izanagi::shader::CShaderBasic* m_Shader;

    izanagi::CDebugMesh* m_Sphere;
    izanagi::CDebugMesh* m_Cube;
    izanagi::CDebugMesh* m_Plane;

    izanagi::math::SMatrix44 m_L2W;
    izanagi::SParallelLightParam m_ParallelLight;

    izanagi::graph::CVertexBuffer* m_VB;
    izanagi::graph::CVertexDeclaration* m_VD;

    izanagi::graph::CRenderTarget* m_RT[4];

    enum RenderMode {
        SSAO,
        Textures,
        Ambient,

        RenderModeNum,
    };
    RenderMode m_Mode;
};

#endif    // #if !defined(__SSAO_APP_H__)
