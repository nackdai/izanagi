#include "izSampleKit.h"
#include "SSAOApp.h"

static const IZ_UINT BUF_SIZE = 8 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

SSAOApp app;

IzMain(SCREEN_WIDTH, SCREEN_HEIGHT)
{
    int ret = SampleMain(
        IzGetSystemDataForMainFunc(),
        &app,
        "SSAO",
        IzGetScreenWidth(), IzGetScreenHeight(),
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
