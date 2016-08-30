#if !defined(__PLAYER_CHARACTER_H__)
#define __PLAYER_CHARACTER_H__

#include "izSampleKit.h"
#include "izEngine.h"
#include "izAnimation.h"

class PlayerCharacter
{
public:
    PlayerCharacter();
    ~PlayerCharacter();

public:
    // 初期化.
    IZ_BOOL init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::sample::CSampleCamera& camera);

    // 解放.
    void release();

    // 更新.
    void update(
        izanagi::graph::CGraphicsDevice* device,
        const izanagi::CCamera& camera,
        IZ_FLOAT elapsed);

    // 描画.
    void render(
        izanagi::graph::CGraphicsDevice* device,
        const izanagi::CCamera& camera);

    IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key);
    void OnKeyUp(izanagi::sys::E_KEYBOARD_BUTTON key);

    IZ_FLOAT getSpeed() const
    {
        return m_speed;
    }

private:
    IZ_FLOAT GetDirection(
        IZ_FLOAT forward, IZ_FLOAT right,
        izanagi::math::CVector3& dir);

    void MoveForward(
        izanagi::math::CVector3& dir,
        IZ_FLOAT value);

protected:
    IZ_BOOL initModel(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device);

    IZ_BOOL createMaterial(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::CImage* img,
        izanagi::shader::CShaderBasic* shd);

    void initAnimation(izanagi::IMemoryAllocator* allocator);

private:
    izanagi::math::CQuat QuatToRotateRight;

    izanagi::CCamera* m_camera{ nullptr };

    izanagi::engine::AnimationStateMachine* m_anm{ nullptr };

    IZ_FLOAT m_speed{ 0.0f };

    izanagi::engine::Mesh* m_mesh{ nullptr };
    izanagi::engine::CharacterController* m_ctrl{ nullptr };

    izanagi::CRenderGraph* m_RenderGraph{ nullptr };
    izanagi::CSceneRenderer* m_Renderer{ nullptr };

    enum State {
        Idle,
        Move,
        Rotate,
    };

    State m_state{ State::Idle };
    IZ_FLOAT m_slerp{ 0.0f };

    // Character direction.
    izanagi::math::CVector3 m_dir;

    izanagi::math::CQuat m_toQuat;
    izanagi::math::CQuat m_fromQuat;

    IZ_FLOAT m_forward{ 0.0f };
    IZ_FLOAT m_right{ 0.0f };
};

#endif    // #if !defined(__PLAYER_CHARACTER_H__)
