#include "izSampleKit.h"

class CGamePadApp : public izanagi::sample::CSampleApp {
public:
	CGamePadApp();
	virtual ~CGamePadApp();

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
};

CGamePadApp::CGamePadApp()
{
}

CGamePadApp::~CGamePadApp()
{
}

// 初期化.
IZ_BOOL CGamePadApp::InitInternal(
	izanagi::IMemoryAllocator* allocator,
	izanagi::graph::CGraphicsDevice* device,
	izanagi::sample::CSampleCamera& camera)
{
    return IZ_TRUE;
}

// 解放.
void CGamePadApp::ReleaseInternal()
{
}

// 更新.
void CGamePadApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::CPad* pad = GetPad();

    if (pad != IZ_NULL)
    {
        pad->Update();
    }
}

// 描画.
void CGamePadApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::CPad* pad = GetPad();
    izanagi::CDebugFont* font = GetDebugFont();

    if (device->Begin2D())
    {
        font->Begin(0, 100);
        {
            if (pad != IZ_NULL)
            {
                const izanagi::CPad::PadState& state = pad->GetCurState();
	    
                font->DBPrint("AxisPos [%d][%d][%d]\n", state.lX, state.lY, state.lZ);
		        font->DBPrint("AxisRot [%d][%d][%d]\n", state.lRx, state.lRy, state.lRz);
		        font->DBPrint("Slider  [%d][%d]\n", state.rglSlider[0], state.rglSlider[1]);
		        font->DBPrint(
			        "Pov     [%d][%d][%d][%d]\n", 
			        state.rgdwPOV[0], state.rgdwPOV[1], state.rgdwPOV[2], state.rgdwPOV[3]);

		        for (IZ_UINT i = 0; i < 32; i += 2)
                {
			        font->DBPrint(
				        "Button  %d[%d] %d[%d]\n",
				        i, state.rgbButtons[i],
				        i + 1, state.rgbButtons[i + 1]);
		        }
            }
            else
            {
                font->DBPrint("No GamePad");
            }
	    }
        font->End();

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
	CGamePadApp app;

	int ret = SampleMain(
		hInstance,
		&app,
		"GamePad",
		SCREEN_WIDTH, SCREEN_HEIGHT,
		BUF, BUF_SIZE,
		GFX_BUF, GFX_BUF_SIZE);

	return ret;
}
