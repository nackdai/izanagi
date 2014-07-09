// ImageLibProxy.h

#pragma once

using namespace System;

namespace izanagi {
namespace tool {

    public ref class ImageLibProxy
    {
    private:
        ImageLibProxy();
        ~ImageLibProxy();
        !ImageLibProxy();

    public:
        static bool InitImageLib(IntPtr hWnd);

        static void ReleaseImageLib();

        static IntPtr ReadImageMaster(char* path);

        static void ReleaseImageMaster(IntPtr p);

        static unsigned int GetTexNumInMaster(IntPtr p);

        static IntPtr GetTextureInMaster(IntPtr p, unsigned int texIdx);

        static unsigned int GetImageNumInTexture(IntPtr p);

        static unsigned int GetMipMapNumInTexture(IntPtr p);

        static IntPtr GetImageInTexture(IntPtr p, unsigned int imgIdx, unsigned int level);

        static unsigned int GetTextureType(IntPtr p);

        static unsigned int GetImageWidth(IntPtr p);

        static unsigned int GetImageHeight(IntPtr p);

        static IntPtr GetPixelDataAsBGRA8(IntPtr p);

        static void ReleasePixelData(IntPtr p);

        static void* GetBGRA8ByPosition(IntPtr ptr, int x, int y, int stride);
    };

}   // namespace tool
}   // namespace izanagi
