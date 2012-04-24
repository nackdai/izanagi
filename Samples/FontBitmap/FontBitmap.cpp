#include "izSampleKit.h"
#include "izFont.h"

#include "FontUTF8.h"
#include "FontSJIS.h"

class CFontBitmapApp : public izanagi::sample::CSampleApp {
public:
	CFontBitmapApp();
	virtual ~CFontBitmapApp();

protected:
	// 初期化.
	virtual IZ_BOOL InitInternal(
		izanagi::IMemoryAllocator* allocator,
		izanagi::CGraphicsDevice* device,
		izanagi::sample::CSampleCamera& camera);

	// 解放.
	virtual void ReleaseInternal();

	// 更新.
	virtual void UpdateInternal(
		izanagi::CCamera& camera,
		izanagi::CGraphicsDevice* device);

	// 描画.
	virtual void RenderInternal(izanagi::CGraphicsDevice* device);

private:
	izanagi::CFontRenderer* m_FontUtf8;
	izanagi::CFontRenderer* m_FontSjis;
};

CFontBitmapApp::CFontBitmapApp()
{
	m_FontUtf8 = IZ_NULL;
	m_FontSjis = IZ_NULL;
}

CFontBitmapApp::~CFontBitmapApp()
{
}

// 初期化.
IZ_BOOL CFontBitmapApp::InitInternal(
	izanagi::IMemoryAllocator* allocator,
	izanagi::CGraphicsDevice* device,
	izanagi::sample::CSampleCamera& camera)
{
	// UTF8
	{
		izanagi::CFileInputStream in;
		VRETURN(in.Open("./data/FontSample_Utf8.fnt"));

		// FNT読み込み
		m_FontUtf8 = izanagi::CFontRenderer::CreateFontRendererBmp(
					allocator,
					device,
					40,
					&in);

		VRETURN(m_FontUtf8 != IZ_NULL);
	}

	// UTF8
	{
		izanagi::CFileInputStream in;
		VRETURN(in.Open("./data/FontSample_Sjis.fnt"));

		// FNT読み込み
		m_FontSjis = izanagi::CFontRenderer::CreateFontRendererBmp(
					allocator,
					device,
					40,
					&in);
		VRETURN(m_FontSjis != IZ_NULL);
	}

	return IZ_TRUE;
}

// 解放.
void CFontBitmapApp::ReleaseInternal()
{
	SAFE_RELEASE(m_FontUtf8);
	SAFE_RELEASE(m_FontSjis);
}

// 更新.
void CFontBitmapApp::UpdateInternal(
	izanagi::CCamera& camera,
	izanagi::CGraphicsDevice* device)
{
	// Nothing is done...
}

// 描画.
void CFontBitmapApp::RenderInternal(izanagi::CGraphicsDevice* device)
{
	if (device->Begin2D()) {
		// フォント描画
		m_FontUtf8->Register(fontUTF8);
		m_FontUtf8->Render(
			fontUTF8,
			0, 200,
			izanagi::CColor::GREEN);

#if 1
		m_FontSjis->Register(fontSJIS);
		m_FontSjis->Render(
			fontSJIS,
			0, 300,
			izanagi::CColor::GREEN);
#endif

		device->End2D();
	}
}

static const IZ_UINT BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CFontBitmapApp app;

	int ret = SampleMain(
		hInstance,
		&app,
		"FontBitmap",
		SCREEN_WIDTH, SCREEN_HEIGHT,
		BUF, BUF_SIZE,
		GFX_BUF, GFX_BUF_SIZE);

	return ret;
}
