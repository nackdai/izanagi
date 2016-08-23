#if !defined(__MOTION_BLEND_APP_H__)
#define __MOTION_BLEND_APP_H__

#include "izSampleKit.h"
#include "izAnimation.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class CMotionBlendApp : public izanagi::sample::CSampleApp
{
public:
    CMotionBlendApp();
    virtual ~CMotionBlendApp();

protected:
    // 初期化.
    virtual IZ_BOOL InitInternal(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::sample::CSampleCamera& camera);

    // 解放.
    virtual void ReleaseInternal();

    // 更新.
    virtual void UpdateInternal(izanagi::graph::CGraphicsDevice* device);

    // 描画.
    virtual void RenderInternal(izanagi::graph::CGraphicsDevice* device);

    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key) override;

private:
    izanagi::CImage* m_Img;
    izanagi::CModel* m_Mdl;
    izanagi::CMesh* m_Msh;
    izanagi::CSkeleton* m_Skl;
    izanagi::shader::CShaderBasic* m_Shd;
    izanagi::CMaterial* m_Mtrl[17];

    izanagi::CRenderGraph* m_RenderGraph;
    izanagi::CSceneRenderer* m_Renderer;

    izanagi::animation::CTimeline m_Timeline;

    izanagi::CAnimation* m_Anm[2];
    izanagi::CAnmLinearBlender* m_AnmBlender;

    IZ_FLOAT m_weight{ 0.0f };
};

#endif    // #if !defined(__MOTION_BLEND_APP_H__)
