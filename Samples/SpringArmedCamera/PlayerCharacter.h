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

    IZ_FLOAT getSpeed() const
    {
        return m_speed;
    }

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
    izanagi::engine::AnimationStateMachine* m_anm{ nullptr };

    IZ_FLOAT m_speed{ 0.0f };

    izanagi::engine::Mesh* m_mesh{ nullptr };
    izanagi::engine::CharacterController* m_ctrl{ nullptr };

    izanagi::CRenderGraph* m_RenderGraph{ nullptr };
    izanagi::CSceneRenderer* m_Renderer{ nullptr };
};

#endif    // #if !defined(__PLAYER_CHARACTER_H__)
