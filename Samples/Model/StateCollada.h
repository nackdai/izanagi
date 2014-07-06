#if !defined(__STATE_COLLADA_H__)
#define __STATE_COLLADA_H__

#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "StateBase.h"

class CStateCollada : public CStateBase {
public:
    CStateCollada(izanagi::sample::CSampleApp* app);
    virtual ~CStateCollada();

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
};

#endif    // #if !defined(__STATE_COLLADA_H__)
