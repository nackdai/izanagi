#if !defined(__SD_UNITY_CHAN_APP_H__)
#define __SD_UNITY_CHAN_APP_H__

#include "izSampleKit.h"
#include "izAnimation.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class SDUnityChanApp : public izanagi::sample::CSampleApp {
public:
    SDUnityChanApp();
    virtual ~SDUnityChanApp();

protected:
    // 初期化.
    virtual IZ_BOOL InitInternal(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::sample::CSampleCamera& camera) override;

    IZ_BOOL initModel(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device);

    IZ_BOOL CreateMaterial(izanagi::IMemoryAllocator* allocator);

    // 解放.
    virtual void ReleaseInternal() override;

    // 更新.
    virtual void UpdateInternal(izanagi::graph::CGraphicsDevice* device) override;

    // 描画.
    virtual void RenderInternal(izanagi::graph::CGraphicsDevice* device) override;

    // キー押下
    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key) override;

private:
    izanagi::CImage* m_Img{ nullptr };
    izanagi::CModel* m_Mdl{ nullptr };
    izanagi::CMesh* m_Msh{ nullptr };
    izanagi::CSkeleton* m_Skl{ nullptr };
    izanagi::shader::CShaderBasic* m_Shd{ nullptr };

    izanagi::CMaterial* m_Mtrl[3];

    izanagi::CRenderGraph* m_RenderGraph{ nullptr };
    izanagi::CSceneRenderer* m_Renderer{ nullptr };

    izanagi::math::SMatrix44 m_L2W;

    izanagi::sample::CSampleMdlRenderHandler* m_MdlRenderHandler{ nullptr };

    izanagi::CAnimation* m_Anm{ nullptr };

    izanagi::animation::CTimeline m_Timeline;

    IZ_BOOL m_enableStepAnm{ IZ_FALSE };
    IZ_BOOL m_step{ IZ_FALSE };
};

#endif    // #if !defined(__SD_UNITY_CHAN_APP_H__)
