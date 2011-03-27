#include <stdafx.h>
#include "MyAppl.h"
#include "MySystem.h"
#include "MyCamera.h"
#include "state/StateManager.h"

#include "izMath.h"

namespace {
	void _OnLostDevice()
	{
	}

	IZ_BOOL _OnReset()
	{
		return IZ_TRUE;
	}
}	// namespace

CMyAppl CMyAppl::s_cInstance;

void CMyAppl::Reset()
{
	_OnLostDevice();
}

// 初期化
IZ_BOOL CMyAppl::Init(
	HWND hDeviceWindow,
	HWND hFocusWindow)
{
	static const IZ_UINT MEM_SIZE = 16 * 1024 * 1024;	// 16MB
	static IZ_UINT8 MEM_BUF[MEM_SIZE];

	// 乱数初期化
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	izanagi::CMathRand::Init(systime.wSecond);

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

	// デバッグフォント初期化
	ret = CMySystem::GetInstance().InitDebugFont();
	VRETURN(ret);

	// ステート初期化
	ret = CStateManager::GetInstance().Create();
	VRETURN(ret);

	izanagi::IMemoryAllocator* pAllocator = CMySystem::GetInstance().GetMemoryAllocator();
	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();

	// コールバックセット
	CMySystem::GetInstance().GetGraphicsDevice()->SetResetCallBack(_OnReset);
	CMySystem::GetInstance().GetGraphicsDevice()->SetLostDeviceCallBack(_OnLostDevice);

	return IZ_TRUE;
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
	IZ_BOOL ret = CStateManager::GetInstance().Update();
	return ret;
}

// 描画
IZ_BOOL CMyAppl::Render()
{
	static const D3DCOLOR color = D3DCOLOR_ARGB(255, 0, 128, 255);

	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();

	const izanagi::SViewport& vp = pDevice->GetViewport();

	if (pDevice->BeginRender(
			izanagi::E_GRAPH_CLEAR_FLAG_ALL,
			color, 1.0f, 0))
	{
		CMyCamera::GetInstance().UpdateCamera();

		CStateManager::GetInstance().Render();

		if (CMySystem::GetInstance().GetGraphicsDevice()->Begin2D()) {
			CMySystem::GetInstance().GetDebugFont()->Begin();

#if 0
			{
				IZ_FLOAT time = CMySystem::GetInstance().GetTimer(0).GetTime();
				IZ_FLOAT fps = 1000.0f / time;

				CMySystem::GetInstance().GetDebugFont()->DBPrint(
					0, vp.height - 32,
					"%.2f[ms] %.2f[fps]\n",
					time, fps);
			}

			{
				IZ_FLOAT time = CMySystem::GetInstance().GetTimer(1).GetTime();
				IZ_FLOAT fps = 1000.0f / time;

				CMySystem::GetInstance().GetDebugFont()->DBPrint(
					0, vp.height - 16,
					"%.2f[ms] %.2f[fps]\n",
					time, fps);
			}
#endif

#if 0
			izanagi::CPostEffect* pPostEffect = CPostEffectSample::GetInstance().GetPostEffect();
			if (pPostEffect != IZ_NULL) {
				IZ_UINT y = 0;

				for (IZ_UINT i = 3; i < 7; i++) {
					izanagi::CTexture* tex = pPostEffect->GetTexture(i);

					//if (tex != IZ_NULL) {
					{
						pDevice->SetTexture(
							0, 
							tex);

						pDevice->Set2DRenderOp(izanagi::E_GRAPH_2D_RENDER_OP_NO_TEX_ALPHA);
						pDevice->Draw2DSprite(
							izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
							izanagi::CIntRect(0, y, 256, y + 144));

						y += 144;
					}
				}
			}
#endif

			CMySystem::GetInstance().GetDebugFont()->End();
			CMySystem::GetInstance().GetGraphicsDevice()->End2D();
		}

		pDevice->EndRender();
	}

	return IZ_TRUE;
}
