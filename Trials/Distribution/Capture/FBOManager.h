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

    void readback(izanagi::graph::CGraphicsDevice* device);

    void begin(
        izanagi::graph::CGraphicsDevice* device,
        IZ_COLOR clrClear);

    void end(izanagi::graph::CGraphicsDevice* device);

    void terminate();

    void drawDebug(izanagi::graph::CGraphicsDevice* device);

private:
    struct {
        izanagi::graph::CRenderTarget* rt[2];
    } m_frames[4];

    IZ_UINT m_RBHead{ 0 };
    IZ_UINT m_RBTail{ 0 };

    izanagi::graph::CRenderTarget* m_depthRT{ nullptr };

    izanagi::IMemoryAllocator* m_allocator{ nullptr };
    void* m_pixels{ nullptr };

    IZ_UINT m_screenWidth;
    IZ_UINT m_screenHeight;

    izanagi::graph::CTexture* m_clr{ nullptr };
    izanagi::graph::CTexture* m_depth{ nullptr };
};

#endif    // #if !defined(__FBO_MANAGER_H__)