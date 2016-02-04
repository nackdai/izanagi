#if !defined(__FBO_MANAGER_2_H__)
#define __FBO_MANAGER_2_H__

#include "izSampleKit.h"

class FBOManager2
{
public:
    FBOManager2() {}
    ~FBOManager2();

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
    GLuint m_fbo;

    izanagi::graph::CTexture* m_clr{ nullptr };
    izanagi::graph::CTexture* m_depth{ nullptr };

    izanagi::IMemoryAllocator* m_allocator{ nullptr };
    void* m_pixels{ nullptr };

    IZ_UINT m_screenWidth;
    IZ_UINT m_screenHeight;
};

#endif    // #if !defined(__FBO_MANAGER_2_H__)