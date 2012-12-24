#if !defined(__STATE_SEPIA_H__)
#define __STATE_SEPIA_H__

#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "StatePostEffect.h"

class CStateSepia : public CStatePostEffect {
public:
    CStateSepia(
        izanagi::sample::CSampleApp* app,
        izanagi::SCameraParam& camera);
    virtual ~CStateSepia();

public:
    // 描画.
    virtual IZ_BOOL Render(izanagi::graph::CGraphicsDevice* device);

    // 開始
    virtual IZ_BOOL Enter(
        izanagi::IMemoryAllocator* allocator,
        void* val);
};

#endif    // #if !defined(__STATE_SEPIA_H__)
