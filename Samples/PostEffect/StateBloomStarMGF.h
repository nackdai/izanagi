#if !defined(__STATE_BLOOM_STAR_MGF_H__)
#define __STATE_BLOOM_STAR_MGF_H__

#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "StatePostEffect.h"

class CStateBloomStarMGF : public CStatePostEffect {
public:
    CStateBloomStarMGF(
        izanagi::sample::CSampleApp* app,
        izanagi::SCameraParam& camera);
    virtual ~CStateBloomStarMGF();

public:
    // 描画.
    virtual IZ_BOOL Render(izanagi::graph::CGraphicsDevice* device);

    // 開始
    virtual IZ_BOOL Enter(
        izanagi::IMemoryAllocator* allocator,
        izanagi::CValue& arg);
};

#endif    // #if !defined(__STATE_BLOOM_STAR_MGF_H__)
