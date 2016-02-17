#if !defined(__CAPTURE_MANAGER_H__)
#define __CAPTURE_MANAGER_H__

#include "izSampleKit.h"

class CaptureManager
{
public:
    CaptureManager() {}
    ~CaptureManager();

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
    void readbackDepth();
    void readbackColor();

    void blitFrameBuffer();

    void captureScreen();

private:
    GLuint m_fbo;

    izanagi::graph::CTexture* m_depth{ nullptr };

    struct ScreenData {
        GLuint buffer;
        GLuint tmpBuffer;
        GLsync fence;
    } m_SD[4];

    IZ_UINT m_head{ 0 };
    IZ_UINT m_tail{ 0 };

    izanagi::IMemoryAllocator* m_allocator{ nullptr };
    void* m_pixels{ nullptr };

    IZ_UINT m_screenBufferSize{ 0 };
    IZ_UINT m_screenWidth{ 0 };
    IZ_UINT m_screenHeight{ 0 };

    izanagi::graph::CTexture* m_tmpTex{ nullptr };
};

#endif    // #if !defined(__CAPTURE_MANAGER_H__)