#if !defined(__STATE_MONOTONE_H__)
#define __STATE_MONOTONE_H__

#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "StatePostEffect.h"

class CStateMonotone : public CStatePostEffect {
public:
    CStateMonotone(
        izanagi::sample::CSampleApp* app,
        izanagi::SCameraParam& camera);
    virtual ~CStateMonotone();

public:
    // 描画.
    virtual IZ_BOOL Render(izanagi::graph::CGraphicsDevice* device);

    // 開始
    virtual IZ_BOOL Enter(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::CValue& arg);
};

#endif    // #if !defined(__STATE_PHONG_SHADER_H__)
