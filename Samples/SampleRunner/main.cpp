#include "izSampleKit.h"
#include <string>

class CSampleRunner : public izanagi::sample::CSampleApp {
public:
    CSampleRunner();
    virtual ~CSampleRunner();

protected:
    // 初期化.
    virtual IZ_BOOL InitInternal(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::sample::CSampleCamera& camera);

    // 解放.
    virtual void ReleaseInternal();

    // 更新.
    virtual void UpdateInternal(izanagi::graph::CGraphicsDevice* device);

    // 描画.
    virtual void RenderInternal(izanagi::graph::CGraphicsDevice* device);

private:
};

CSampleRunner::CSampleRunner()
{
}

CSampleRunner::~CSampleRunner()
{
}

// 初期化.
IZ_BOOL CSampleRunner::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    m_Allocator = allocator;

    HANDLE hFind;
	WIN32_FIND_DATA win32fd;

	hFind = FindFirstFile(".\\*", &win32fd);

	if (hFind == INVALID_HANDLE_VALUE) {
		return IZ_FALSE;
	}

    do {
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            std::string filename(win32fd.cFileName);

            if (filename.compare(".") == 0 || filename.compare("..") == 0) {
                continue;
            }

            filename.append("\\*");

            WIN32_FIND_DATA sub;
            HANDLE h = FindFirstFile(filename.c_str(), &sub);
			//printf("%s\n", win32fd.cFileName);

            do {
                printf("%s\n", sub.cFileName);
            } while (FindNextFile(h, &win32fd));
		}
        else {
        }
	} while (FindNextFile(hFind, &win32fd));

    return IZ_TRUE;
}

// 解放.
void CSampleRunner::ReleaseInternal()
{
}

// 更新.
void CSampleRunner::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
}

// 描画.
void CSampleRunner::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
}

static const IZ_UINT BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

CSampleRunner app;

IzMain(SCREEN_WIDTH, SCREEN_HEIGHT)
{
    int ret = SampleMain(
        IzGetSystemDataForMainFunc(),
        &app,
        "SampleRunner",
        IzGetScreenWidth(), IzGetScreenHeight(),
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
