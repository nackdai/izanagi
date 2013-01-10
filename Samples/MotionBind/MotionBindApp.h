#if !defined(__MOTION_BIND_APP_H__)
#define __MOTION_BIND_APP_H__

#include "izSampleKit.h"
#include "izAnimation.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class CMotionBindApp : public izanagi::sample::CSampleApp
{
public:
    CMotionBindApp();
    virtual ~CMotionBindApp();

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

private:
    izanagi::CImage* m_Img;
    izanagi::CModel* m_Mdl;
    izanagi::CMesh* m_Msh;
    izanagi::CSkeleton* m_Skl;
    izanagi::CShaderBasic* m_Shd;
    izanagi::CMaterial* m_Mtrl[17];

    izanagi::CRenderGraph* m_RenderGraph;
    izanagi::CSceneRenderer* m_Renderer;

    izanagi::animation::CTimeline m_Timeline;

    izanagi::CAnimation* m_Anm[2];
    izanagi::CAnimationBinder* m_AnmBinder;
};

#endif    // #if !defined(__MOTION_BIND_APP_H__)
