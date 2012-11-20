#include "MyAppl.h"
#include "MySystem.h"
#include "MyCamera.h"
#include "state/StateManager.h"
#include "TextureManager.h"

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

	// ステート初期化
	ret = CStateManager::GetInstance().Init();
	VRETURN(ret);

	// カメラ初期化
	{
		izanagi::CVector vecPos(0.0f, 0.0f, 20.0f, 1.0f);
		izanagi::CVector vecRef(0.0f, 0.0f, 0.0f, 1.0f);
		izanagi::CVector vecUp(0.0f, 1.0f, 0.0f, 1.0f);

		CMyCamera::GetInstance().Init(
			vecPos, vecRef, vecUp,
			1.0f, 1000.0f,
			izanagi::CMath::Deg2Rad(90.0f),
			(IZ_FLOAT)SCREEN_WIDTH / SCREEN_HEIGHT);
	}

#if 1
	// パッド初期化
	{
		D_INPUT* pInput = NULL;
		HRESULT hr = DirectInput8Create(
						hInst,
						DIRECTINPUT_VERSION,
						IID_IDirectInput8,
						(void**)&pInput,
						NULL);
		IZ_ASSERT(SUCCEEDED(hr));
		
		ret = CMySystem::GetInstance().InitKeyboard();
		if (ret) {
			izanagi::SInputDeviceInitParam sInitParam(pInput, hDeviceWindow);
			CMySystem::GetInstance().GetKeyboard()->Init(&sInitParam);
		}

		pInput->Release();

		//VRETURN(ret);
	}
#endif

	// テクスチャマネージャ初期化
	izanagi::CTextureManager::GetInstance().Init(
		CMySystem::GetInstance().GetMemoryAllocator(),
		CMySystem::GetInstance().GetGraphicsDevice());
	izanagi::CTextureManager::GetInstance().AllRead();

	return IZ_TRUE;
}

CMyAppl::CMyAppl()
{
	m_pRT = IZ_NULL;
	m_IsOnShader = IZ_TRUE;
}

// 開放
void CMyAppl::Release()
{
	izanagi::CTextureManager::GetInstance().Clear();

	SAFE_RELEASE(m_pRT);
	CStateManager::GetInstance().Destroy();
	CMySystem::GetInstance().Release();
}

// 更新
IZ_BOOL CMyAppl::Update()
{
	IZ_BOOL ret = IZ_FALSE;

	CMySystem::GetInstance().GetKeyboard()->Update();

	if (CMySystem::GetInstance().GetKeyboard()->IsPushOneShotKey(izanagi::E_KEYBOARD_BUTTON_SPACE)) {
		m_IsOnShader = !m_IsOnShader;
	}

	if (!CStateManager::GetInstance().OnKeyDown(0)) {
		ret = CStateManager::GetInstance().Update(CMySystem::GetInstance().GetMemoryAllocator());
	}

	CMyCamera::GetInstance().Update();

	return ret;
}

// 描画
IZ_BOOL CMyAppl::Render()
{
	static const D3DCOLOR color = D3DCOLOR_ARGB(255, 0, 128, 255);

	izanagi::graph::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();

	pDevice->BeginRender(
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		color, 1.0f, 0);

	CMyCamera::GetInstance().Update();

	IZ_BOOL ret = CStateManager::GetInstance().Render(pDevice);

	pDevice->EndRender();

	return ret;
}