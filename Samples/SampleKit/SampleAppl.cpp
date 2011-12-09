#include "SampleAppl.h"
#include "SampleSystem.h"
#include "SampleCamera.h"

CSampleAppl CSampleAppl::s_cInstance;

// 初期化
IZ_BOOL CSampleAppl::Init(const SSampleApplParams& params)
{
	// システム初期化
	IZ_BOOL ret = CMySystem::GetInstance().Init(
                    params.allocatorBufSize,
                    params.allocatorBuf);
	VRETURN(ret);

	// グラフィックスデバイス設定
	izanagi::SGraphicsDeviceInitParams gfxDevParams;
	{
		gfxDevParams.hFocusWindow = hFocusWindow;
		gfxDevParams.hDeviceWindow = hDeviceWindow;
		
		gfxDevParams.Windowed = IZ_TRUE;						// 画面モード(ウインドウモード)

		gfxDevParams.BackBufferWidth = params.screenWidth;			// バックバッファの幅
		gfxDevParams.BackBufferHeight = params.screenHeight;		// バックバッファの高さ

		gfxDevParams.MultiSampleType = D3DMULTISAMPLE_NONE;	// マルチ・サンプリングの種類

		gfxDevParams.Adapter = D3DADAPTER_DEFAULT;
		gfxDevParams.DeviceType = D3DDEVTYPE_HAL;
		gfxDevParams.BehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;

		gfxDevParams.DepthStencilFormat = D3DFMT_D24S8;

		gfxDevParams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	}

	// デバイスリセット
	ret = CMySystem::GetInstance().GetGraphicsDevice()->Reset(gfxDevParams);
	VRETURN(ret);

	// デバッグフォント初期化
	ret = CMySystem::GetInstance().InitDebugFont();
	VRETURN(ret);

	// リセット用コールバックセット
	//CMySystem::GetInstance().GetGraphicsDevice()->SetResetCallBack(_ResetResource);

    ret = InitInternal();
    VRETURN(ret);

	return IZ_TRUE;
}

CSampleAppl::~CSampleAppl()
{
}

// 開放
void CSampleAppl::Release()
{
	ReleaseInternal();
    CMySystem::GetInstance().Release();
}

// 描画.
void CSampleAppl::Render()
{
	CGraphicsDevice* device = CMySystem::GetInstance().GetGraphicsDevice();
    RenderInternal(device);
}
