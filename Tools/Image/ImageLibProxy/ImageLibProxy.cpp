// これは メイン DLL ファイルです。

#include "stdafx.h"

#include "ImageLibProxy.h"
#include "ImageLibWrapper.h"

namespace izanagi {
namespace tool {

    ImageLibProxy::ImageLibProxy()
    {
    }

    ImageLibProxy::~ImageLibProxy()
    {
    }

    ImageLibProxy::!ImageLibProxy()
    {
    }

    bool ImageLibProxy::InitImageLib(IntPtr hWnd)
    {
        return (izInitImageLib((void*)hWnd) == IZ_TRUE);
    }

    void ImageLibProxy::ReleaseImageLib()
    {
        izReleaseImageLib();
    }

    IntPtr ImageLibProxy::ReadImageMaster(char* path)
    {
        return (IntPtr)izReadImageMaster(path);
    }

    void ImageLibProxy::ReleaseImageMaster(IntPtr p)
    {
        izReleaseImageMaster((void*)p);
    }

    unsigned int ImageLibProxy::GetTexNumInMaster(IntPtr p)
    {
        return izGetTexNumInMaster((void*)p);
    }

    IntPtr ImageLibProxy::GetTextureInMaster(IntPtr p, unsigned int texIdx)
    {
        return (IntPtr)izGetTextureInMaster((void*)p, texIdx);
    }

    unsigned int ImageLibProxy::GetImageNumInTexture(IntPtr p)
    {
        return izGetImageNumInTexture((void*)p);
    }

    unsigned int ImageLibProxy::GetMipMapNumInTexture(IntPtr p)
    {
        return izGetMipMapNumInTexture((void*)p);
    }

    IntPtr ImageLibProxy::GetImageInTexture(IntPtr p, unsigned int imgIdx, unsigned int level)
    {
        return (IntPtr)izGetImageInTexture((void*)p, imgIdx, level);
    }

    unsigned int ImageLibProxy::GetTextureType(IntPtr p)
    {
        return izGetTextureType((void*)p);
    }

    unsigned int ImageLibProxy::GetImageWidth(IntPtr p)
    {
        return izGetImageWidth((void*)p);
    }

    unsigned int ImageLibProxy::GetImageHeight(IntPtr p)
    {
        return izGetImageHeight((void*)p);
    }

    IntPtr ImageLibProxy::GetPixelDataAsBGRA8(IntPtr p)
    {
        return (IntPtr)izGetPixelDataAsBGRA8((void*)p);
    }

    void ImageLibProxy::ReleasePixelData(IntPtr p)
    {
        izReleasePixelData((void*)p);
    }

}   // namespace tool
}   // namespace izanagi
