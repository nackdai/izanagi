#if !defined(__STATE_FBX_H__)
#define __STATE_FBX_H__

#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "StateBase.h"
#include "izAnimation.h"

class CStateFbx : public CStateBase {
public:
    CStateFbx(izanagi::sample::CSampleApp* app);
    virtual ~CStateFbx();

public:
    // 開始
    virtual IZ_BOOL Enter(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::CValue& arg);

    // ステートから抜ける（終了）.
    virtual IZ_BOOL Leave();

    virtual IZ_UINT GetTexIdx() const { return 0; }
    virtual IZ_UINT GetShaderPassIdx() const { return 0; }

    virtual IZ_BOOL Render(izanagi::graph::CGraphicsDevice* device) override;

private:
    virtual void OnUpdate(
        IZ_FLOAT time,
        izanagi::graph::CGraphicsDevice* device) override;

    virtual IZ_BOOL CreateMaterial(izanagi::IMemoryAllocator* allocator) override;

    // キー押下
    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key) override;

private:
    izanagi::IMemoryAllocator* m_Allocator{ nullptr };

    izanagi::CAnimation* m_Anm{ nullptr };

    izanagi::animation::CTimeline m_Timeline;
};

#endif    // #if !defined(__STATE_FBX_H__)
