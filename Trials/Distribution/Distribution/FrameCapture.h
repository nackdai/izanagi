#if !defined(__FRAME_CAPTURE_H__)
#define __FRAME_CAPTURE_H__

#include "izSampleKit.h"

class FrameCapture
{
public:
    FrameCapture() {}
    ~FrameCapture();

public:
    void initScreenCapture(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        IZ_UINT width, IZ_UINT height);
    void procScreenCapture();
    void captureScreen();

    void terminate();

    void drawDebug(izanagi::graph::CGraphicsDevice* device);

private:
    IZ_UINT m_screenBufferSize{ 0 };
    IZ_UINT m_screenWidth{ 0 };
    IZ_UINT m_screenHeight{ 0 };

#ifdef __IZ_OGL__
    struct ScreeData {
        GLuint buffer;
        GLuint tmpBuffer;
        GLsync fence;
    } m_SD[4];

    IZ_UINT m_RBHead{ 0 };
    IZ_UINT m_RBTail{ 0 };
#endif  // #ifdef __IZ_OGL__

    izanagi::graph::CTexture* m_tmpTex{ nullptr };
};

#endif    // #if !defined(__FRAME_CAPTURE_H__)