#if !defined(__FBO_MANAGER_H__)
#define __FBO_MANAGER_H__

#include "izSampleKit.h"

class FBOManager
{
public:
    FBOManager() {}
    ~FBOManager();

public:
    void init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        IZ_UINT width, IZ_UINT height);

    void begin(
        izanagi::graph::CGraphicsDevice* device,
        IZ_COLOR clrClear);

    void end(izanagi::graph::CGraphicsDevice* device);

    void terminate();

    void drawDebug(izanagi::graph::CGraphicsDevice* device);

private:
    izanagi::graph::CRenderTarget* m_RT[2];
    izanagi::graph::CRenderTarget* m_depthRT{ nullptr };
};

#endif    // #if !defined(__FBO_MANAGER_H__)