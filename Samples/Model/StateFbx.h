#if !defined(__STATE_FBX_H__)
#define __STATE_FBX_H__

#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "StateBase.h"

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
};

#endif    // #if !defined(__STATE_FBX_H__)
