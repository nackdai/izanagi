#include "SampleApp.h"
#include "SampleWindowProc.h"

using namespace izanagi;
using namespace sample;

CSampleApp::CSampleApp()
{
	m_Device = IZ_NULL;
	m_Allocator = IZ_NULL;

	m_DebugFont = IZ_NULL;
	m_Pad = IZ_NULL;
	m_Keyboard = IZ_NULL;

	m_ScreenWidth = 1;
	m_ScreenHeight = 1;
}

CSampleApp::~CSampleApp()
{
	// 解放されているかチェック
	IZ_ASSERT(m_Device == IZ_NULL);
}

// 初期化
IZ_BOOL CSampleApp::Init(const SSampleAppParams& params)
{
	IZ_BOOL ret = IZ_TRUE;

	m_ScreenWidth = params.screenWidth;
	m_ScreenHeight= params.screenHeight;

	m_Allocator = params.allocator;
	VGOTO(ret = (m_Allocator != IZ_NULL), __EXIT__);

	// グラフィックスデバイス作成
	{
		IZ_ASSERT(params.allocatorForGraph != IZ_NULL);

		m_Device = izanagi::CGraphicsDevice::CreateGrapicsDevice(
						params.allocatorForGraph);
		VGOTO(ret = (m_Device != IZ_NULL), __EXIT__);
	}

	// グラフィックスデバイス設定
	izanagi::SGraphicsDeviceInitParams gfxDevParams;
	{
		gfxDevParams.hFocusWindow = (HWND)params.focusWindow;
		gfxDevParams.hDeviceWindow = (HWND)params.deviceWindow;
		
		gfxDevParams.Windowed = IZ_TRUE;						// 画面モード(ウインドウモード)

		gfxDevParams.BackBufferWidth = params.screenWidth;			// バックバッファの幅
		gfxDevParams.BackBufferHeight = params.screenHeight;		// バックバッファの高さ

		gfxDevParams.MultiSampleType = D3DMULTISAMPLE_NONE;	// マルチ・サンプリングの種類

		gfxDevParams.Adapter = D3DADAPTER_DEFAULT;
		gfxDevParams.DeviceType = D3DDEVTYPE_HAL;
		gfxDevParams.BehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;

		gfxDevParams.DepthStencilFormat = D3DFMT_D24S8;

		//gfxDevParams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		gfxDevParams.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	}

	// デバイスリセット
	ret = m_Device->Reset(gfxDevParams);
	VGOTO(ret, __EXIT__);

	// デバッグフォント初期化
	{
		m_DebugFont = izanagi::CDebugFont::CreateDebugFont(
						m_Allocator,
						m_Device);
		VGOTO(ret = (m_DebugFont != IZ_NULL), __EXIT__);
	}

#if 0
	// 入力初期化
	{
#if 0
		HRESULT hr = CoInitialize(NULL);
		IZ_ASSERT(SUCCEEDED(hr));

		D_INPUT pInput = NULL;
		hr = CoCreateInstance(
				CLSID_DirectInput8,
				NULL, 
				CLSCTX_INPROC,
				IID_IDirectInput8,
				(void**)&pInput);
		IZ_ASSERT(SUCCEEDED(hr));

		hr = pInput->Initialize(hInst, DIRECTINPUT_VERSION);
#else
		D_INPUT* input = NULL;
		HRESULT hr = DirectInput8Create(
						params.instanceHandle,
						DIRECTINPUT_VERSION,
						IID_IDirectInput8,
						(void**)&input,
						NULL);
		IZ_ASSERT(SUCCEEDED(hr));
#endif
		
		// パッド作成
		{
			m_Pad = izanagi::CPad::CreatePad(&m_Allocator);
			VGOTO(ret, __EXIT__);

			izanagi::SInputDeviceInitParam padInitParam(
				input,
				params.deviceWindow);
			m_Pad->Init(&padInitParam);
		}

		// キーボード作成
		{
			m_Keyboard = izanagi::CKeyboard::CreateKeyboard(&m_Allocator);
			VGOTO(ret, __EXIT__);
			
			// TODO
		}

		input->Release();
	}
#endif

	// TODO
	// リセット用コールバックセット

    ret = InitInternal(
			m_Allocator,
			m_Device,
			m_Camera);
	VGOTO(ret, __EXIT__);

	// メッセージハンドラにアプリを設定
	VGOTO(ret = (params.wndProc != IZ_NULL), __EXIT__);
	params.wndProc->Init(this);

__EXIT__:
	if (!ret) {
		Release();
	}

	return ret;
}

// 開放
void CSampleApp::Release()
{
	ReleaseInternal();

	izanagi::IMemoryAllocator* allocator = IZ_NULL;
	if (m_Device) {
		allocator = m_Device->GetDeviceAllocator();
	}
    
	SAFE_RELEASE(m_DebugFont);
	SAFE_RELEASE(m_Pad);
	SAFE_RELEASE(m_Keyboard);

	SAFE_RELEASE(m_Device);

	if (allocator) {
		allocator->Dump();
	}
}

// 更新.
void CSampleApp::Update()
{
	UpdateInternal(m_Camera.GetRawInterface());

	m_Camera.Update();
}

// 描画.
void CSampleApp::Render()
{
	IZ_ASSERT(m_Device != IZ_NULL);

	IZ_COLOR bgColor = GetBgColor();

	m_Device->BeginRender(
		izanagi::E_GRAPH_CLEAR_FLAG_ALL,
		bgColor, 1.0f, 0);
	{
		RenderInternal(m_Device);

		IZ_ASSERT(m_DebugFont != IZ_NULL);

		// 時間表示
		if (m_Device->Begin2D()) {
			m_DebugFont->Begin();

			{
				IZ_FLOAT time = GetTimer(0).GetTime();
				IZ_FLOAT fps = 1000.0f / time;

				m_DebugFont->DBPrint(
					"%.2f[ms] %.2f[fps]\n",
					time, fps);
			}
			{
				IZ_FLOAT time = GetTimer(1).GetTime();
				IZ_FLOAT fps = 1000.0f / time;

				m_DebugFont->DBPrint(
					"%.2f[ms] %.2f[fps]\n",
					time, fps);
			}

			m_DebugFont->End();

			m_Device->End2D();
		}
	}
	m_Device->EndRender();
}

// V同期.
void CSampleApp::Present()
{
	IZ_ASSERT(m_Device != IZ_NULL);
	m_Device->Present();
}

// スクリーン幅取得
IZ_UINT CSampleApp::GetScreenWidth() const
{
	IZ_ASSERT(m_Device != IZ_NULL);
	return m_ScreenWidth;
}

// スクリーン高さ取得
IZ_UINT CSampleApp::GetScreenHeight() const
{
	IZ_ASSERT(m_Device != IZ_NULL);
	return m_ScreenHeight;
}

// タイマ取得.
izanagi::CTimer& CSampleApp::GetTimer(IZ_UINT idx)
{
	IZ_ASSERT(idx < SAMPLE_TIMER_NUM);
	return m_Timer[idx];
}

// カメラ取得.
CSampleCamera& CSampleApp::GetCamera()
{
	return m_Camera;
}

// 背景色取得.
IZ_COLOR CSampleApp::GetBgColor() const
{
	static const IZ_COLOR bgColor = IZ_COLOR_RGBA(0, 128, 255, 255);
	return bgColor;
}
