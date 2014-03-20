#include "Archive.h"

static const IZ_UINT BUF_SIZE = 4 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 4 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

IzMain(SCREEN_WIDTH, SCREEN_HEIGHT)
{
    CArchiveApp app;

    int ret = SampleMain(
        IzGetSystemDataForMainFunc(),
        &app,
        "Archive",
        IzGetScreenWidth(), IzGetScreenHeight(),
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
