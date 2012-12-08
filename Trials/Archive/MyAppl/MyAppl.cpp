#include "MyAppl.h"
#include "MySystem.h"
#include "state/StateManager.h"

CMyAppl CMyAppl::s_cInstance;

// 初期化
IZ_BOOL CMyAppl::Init(
	HINSTANCE hInst,
	HWND hDeviceWindow,
	HWND hFocusWindow)
{
	static const IZ_UINT MEM_SIZE = 16 * 1024 * 1024;	// 16MB
	static IZ_UINT8 MEM_BUF[MEM_SIZE];

	// システム初期化
	IZ_BOOL ret = CMySystem::GetInstance().Init(MEM_SIZE, MEM_BUF);
	VRETURN(ret);

	// グラフィックスデバイス設定
	izanagi::SGraphicsDeviceInitParams sParams;
	{
		sParams.hFocusWindow = hFocusWindow;
		sParams.hDeviceWindow = hDeviceWindow;
		
		sParams.Windowed = IZ_TRUE;						// 画面モード(ウインドウモード)

		sParams.BackBufferWidth = SCREEN_WIDTH;			// バックバッファの幅
		sParams.BackBufferHeight = SCREEN_HEIGHT;		// バックバッファの高さ

		sParams.MultiSampleType = D3DMULTISAMPLE_NONE;	// マルチ・サンプリングの種類

		sParams.Adapter = D3DADAPTER_DEFAULT;
		sParams.DeviceType = D3DDEVTYPE_HAL;
		sParams.BehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;

		sParams.DepthStencilFormat = D3DFMT_D24S8;

		sParams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	}

	// デバイスリセット
	ret = CMySystem::GetInstance().GetGraphicsDevice()->Reset(sParams);
	VRETURN(ret);

	// デバッグフォント初期化
	ret = CMySystem::GetInstance().InitDebugFont();
	VRETURN(ret);

	// リセット用コールバックセット
	//CMySystem::GetInstance().GetGraphicsDevice()->SetResetCallBack(_ResetResource);

#if 0
	// パッド初期化
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
		D_INPUT* pInput = NULL;
		HRESULT hr = DirectInput8Create(
						hInst,
						DIRECTINPUT_VERSION,
						IID_IDirectInput8,
						(void**)&pInput,
						NULL);
		IZ_ASSERT(SUCCEEDED(hr));
#endif
		
		ret = CMySystem::GetInstance().InitPad();
		if (ret) {
			izanagi::SInputDeviceInitParam sInitParam(pInput, hDeviceWindow);
			CMySystem::GetInstance().GetPad()->Init(&sInitParam);
		}

		pInput->Release();

		//VRETURN(ret);
	}
#endif

	// ステート初期化
	ret = CStateManager::GetInstance().Init();
	VRETURN(ret);

	CStateManager::GetInstance().ChangeState(STATE_TEST1);

	return IZ_TRUE;
}

CMyAppl::CMyAppl()
{
}

// 開放
void CMyAppl::Release()
{
	CStateManager::GetInstance().Destroy();
	CMySystem::GetInstance().Release();
}

// 更新
IZ_BOOL CMyAppl::Update()
{
	if (CMySystem::GetInstance().GetPad() != NULL) {
		CMySystem::GetInstance().GetPad()->Update();
	}

	IZ_BOOL ret = CStateManager::GetInstance().Update(CMySystem::GetInstance().GetMemoryAllocator());
	return ret;
}

// 描画
IZ_BOOL CMyAppl::Render()
{
	static const D3DCOLOR color = D3DCOLOR_ARGB(255, 0, 128, 255);

	izanagi::graph::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();

	pDevice->BeginRender(
		izanagi::graph::E_GRAPH_CLEAR_FLAG_ALL,
		color, 1.0f, 0);

	IZ_BOOL ret = CStateManager::GetInstance().Render(pDevice);

	pDevice->EndRender();

	return ret;
}