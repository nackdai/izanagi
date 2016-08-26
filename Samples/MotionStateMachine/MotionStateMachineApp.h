#if !defined(__MOTION_STATE_MACHINE_APP_H__)
#define __MOTION_STATE_MACHINE_APP_H__

#include "izSampleKit.h"
#include "izEngine.h"

#include "izAnimation.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class MotionStateMachineApp : public izanagi::sample::CSampleApp
{
    class CTimeOverHandler : public izanagi::animation::CTimeline::CTimeOverHandler
    {
    public:
        CTimeOverHandler(MotionStateMachineApp* app) { m_App = app; }
        virtual ~CTimeOverHandler() {}

    public:
        void Handle(const izanagi::animation::CTimeline& timeline);

    private:
        MotionStateMachineApp* m_App;
    };

public:
    MotionStateMachineApp();
    virtual ~MotionStateMachineApp();

protected:
    // 初期化.
    virtual IZ_BOOL InitInternal(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::sample::CSampleCamera& camera);

    IZ_BOOL initModel(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device);

    IZ_BOOL CreateMaterial(izanagi::IMemoryAllocator* allocator);

    void initAnimation(izanagi::IMemoryAllocator* allocator);

    void initStateMachine(izanagi::IMemoryAllocator* allocator);

    void initLayer(izanagi::IMemoryAllocator* allocator);

    // 解放.
    virtual void ReleaseInternal();

    // 更新.
    virtual void UpdateInternal(izanagi::graph::CGraphicsDevice* device);

    // 描画.
    virtual void RenderInternal(izanagi::graph::CGraphicsDevice* device);

    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key);

private:
    izanagi::engine::AnimationStateMachine* m_stateMachine{ nullptr };
    izanagi::engine::AnimationStateMachine* m_layer{ nullptr };

    izanagi::engine::AnimationStateMachineConditionValue* m_willUmatobi{ nullptr };

    IZ_FLOAT m_speed{ 0.0f };
    IZ_BOOL m_isLose{ IZ_FALSE };

    enum State {
        Idle,
        Walk,
        Lose,
        Run,
        Umatobi,
        Num,
    };

    izanagi::IAnimation* m_Anm[State::Num];

    izanagi::CImage* m_Img{ nullptr };
    izanagi::CModel* m_Mdl{ nullptr };
    izanagi::CMesh* m_Msh{ nullptr };
    izanagi::CSkeleton* m_Skl{ nullptr };
    izanagi::shader::CShaderBasic* m_Shd{ nullptr };

    izanagi::CMaterial* m_Mtrl[9];

    izanagi::CRenderGraph* m_RenderGraph{ nullptr };
    izanagi::CSceneRenderer* m_Renderer{ nullptr };

    izanagi::sample::CSampleMdlRenderHandler* m_MdlRenderHandler{ nullptr };

    izanagi::animation::CTimeline m_Timeline;
};

#endif    // #if !defined(__MOTION_STATE_MACHINE_APP_H__)
