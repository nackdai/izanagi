#include "izSampleKit.h"

//#define TRY_INSIDE_TAA
#define TRY_NEW_TAA

#ifdef TRY_INSIDE_TAA
#include "TAAApp2.h"
#elif defined(TRY_NEW_TAA)
#include "TAAApp3.h"
#else
#include "TAAApp.h"
#endif

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

static const IZ_UINT BUF_SIZE = 8 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

#ifdef TRY_INSIDE_TAA
TAAApp2 app;
#elif defined(TRY_NEW_TAA)
TAAApp3 app;
#else
TAAApp app;
#endif

IzMain(SCREEN_WIDTH, SCREEN_HEIGHT)
{
    int ret = SampleMain(
        IzGetSystemDataForMainFunc(),
        &app,
#ifdef TRY_INSIDE_TAA
        "TAA INSIDE",
#elif defined(TRY_NEW_TAA)
		"TAA New",
#else
		"TAA",
#endif
        IzGetScreenWidth(), IzGetScreenHeight(),
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
