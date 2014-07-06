#include "izSampleKit.h"
#include "PhotoGalleryApp.h"
#include "Configure.h"

static const IZ_UINT BUF_SIZE = 16 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

PhotoGalleryApp app;

IzMain(Configure::SCREEN_WIDTH, Configure::SCREEN_HEIGHT)
{
    int ret = SampleMain(
        IzGetSystemDataForMainFunc(),
        &app,
        "PhotoGalleryApp",
        IzGetScreenWidth(), IzGetScreenHeight(),
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}