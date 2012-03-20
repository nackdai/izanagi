#include "izSampleKit.h"
#include "CubeMapApp.h"

static const IZ_UINT BUF_SIZE = 3 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CCubeMapApp app;

	izanagi::CStandardMemoryAllocator allocator(BUF_SIZE, BUF);
	izanagi::CStandardMemoryAllocator allocatorForGraph(GFX_BUF_SIZE, GFX_BUF);

	izanagi::sample::SSampleParam sampleParam = {
		&app,
		&allocator,
		&allocatorForGraph,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		IZ_TRUE,
		"CubeMapApp",
		hInstance,
	};

	IZ_BOOL result = SampleMainLoop(sampleParam);

	int ret = (result ? 0 : 1);

	return ret;
}
