#if !defined(__STATE_BLOOM_STAR_H__)
#define __STATE_BLOOM_STAR_H__

#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "StatePostEffect.h"

class CStateBloomStar : public CStatePostEffect {
public:
    CStateBloomStar(
        izanagi::sample::CSampleApp* app,
        izanagi::SCameraParam& camera);
    virtual ~CStateBloomStar();

public:
    // 描画.
    virtual IZ_BOOL Render(izanagi::graph::CGraphicsDevice* device);

    // 開始
    virtual IZ_BOOL Enter(
        izanagi::IMemoryAllocator* allocator,
        void* val);
};

#endif    // #if !defined(__STATE_BLOOM_STAR_H__)
