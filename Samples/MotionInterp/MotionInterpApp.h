#if !defined(__MOTION_INTERP_APP_H__)
#define __MOTION_INTERP_APP_H__

#include "izSampleKit.h"
#include "izAnimation.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class CMotionInterpApp : public izanagi::sample::CSampleApp
{
    class CTimeOverHandler : public izanagi::animation::CTimeline::CTimeOverHandler
    {
    public:
        CTimeOverHandler(CMotionInterpApp* app) { m_App = app; }
        virtual ~CTimeOverHandler() {}

    public:
        void Handle(const izanagi::animation::CTimeline& timeline);

    private:
        CMotionInterpApp* m_App;
    };

public:
    CMotionInterpApp();
    virtual ~CMotionInterpApp();

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

    virtual IZ_BOOL OnKeyDown(IZ_UINT nChar);

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
    izanagi::CAnimationInterp* m_AnmInterp;

    CTimeOverHandler m_TimeOverHandler;

    izanagi::IAnimation* m_CurAnm;
    IZ_UINT m_TargetAnmIdx;
};

#endif    // #if !defined(__MOTION_INTERP_APP_H__)
