#if !defined(__STATE_HDR_H__)
#define __STATE_HDR_H__

#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "StatePostEffect.h"

class CStateHDR : public CStatePostEffect {
public:
    CStateHDR(
        izanagi::sample::CSampleApp* app,
        izanagi::SCameraParam& camera);
    virtual ~CStateHDR();

public:
    // 描画.
    virtual IZ_BOOL Render(izanagi::graph::CGraphicsDevice* device);

    // 開始
    virtual IZ_BOOL Enter(
        izanagi::IMemoryAllocator* allocator,
        void* val);
};

#endif    // #if !defined(__STATE_HDR_H__)
