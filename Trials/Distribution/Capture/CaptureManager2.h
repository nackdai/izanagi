#if !defined(__CAPTURE_MANAGER_2_H__)
#define __CAPTURE_MANAGER_2_H__

#include "izSampleKit.h"

class CaptureManager2
{
public:
    CaptureManager2() {}
    ~CaptureManager2();

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

    izanagi::graph::CTexture* getColor()
    {
        return m_color;
    }

private:
    void blitFrameBuffer();

private:
    GLuint m_fbo;

    izanagi::graph::CTexture* m_color{ nullptr };
    izanagi::graph::CTexture* m_depth{ nullptr };

    izanagi::IMemoryAllocator* m_allocator{ nullptr };
    void* m_pixels{ nullptr };

    IZ_UINT m_screenBufferSize{ 0 };
    IZ_UINT m_screenWidth{ 0 };
    IZ_UINT m_screenHeight{ 0 };
};

#endif    // #if !defined(__CAPTURE_MANAGER_2_H__)