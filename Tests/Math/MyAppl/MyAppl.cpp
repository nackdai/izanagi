#include <stdafx.h>
#include "MyAppl.h"
#include "MySystem.h"
#include "MyCamera.h"

CMyAppl CMyAppl::s_cInstance;

// 初期化
IZ_BOOL CMyAppl::Init(
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

	return IZ_TRUE;
}

CMyAppl::CMyAppl()
{
}

// 開放
void CMyAppl::Release()
{
	SAFE_RELEASE(m_pRT);
	CMySystem::GetInstance().Release();
}

namespace {
	void test0()
	{
		izanagi::SQuat quat;
		izanagi::SQuat::SetQuatFromRadAxis(quat, IZ_DEG2RAD(45.0f), 0.0f, 0.0f, 1.0f);

		izanagi::SMatrix mtx;
		izanagi::SQuat::MatrixFromQuat(mtx, quat);

		izanagi::SQuat quat1;
		izanagi::SQuat::QuatFromMatrix(quat1, mtx);
	}

	void test1()
	{
		static izanagi::SVector tbl[] = {
			{0.0f, 0.0f, 1.0f, -34.3486f},
			{0.0f, 1.0f, 0.0f, 0.0f},
			{1.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 44.145f},
			{1.0f, 0.0f, 0.0f, -180.0f},
		};

		izanagi::SMatrix mtx;
		izanagi::SMatrix::SetUnit(mtx);

		for (size_t i = 0; i < COUNTOF(tbl); i++) {
			izanagi::SQuat quat;
			izanagi::SQuat::SetQuatFromRadAxis(
				quat,
				IZ_DEG2RAD(tbl[i].w), tbl[i].x, tbl[i].y, tbl[i].z);

			izanagi::SMatrix tmp;
			izanagi::SQuat::MatrixFromQuat(tmp, quat);

			IZ_PRINTF("-----------\n");
			for (IZ_UINT n = 0; n < 16; n++) {
				IZ_PRINTF("%f", tmp.a[n]);
				if ((n % 4) == 3) {
					IZ_PRINTF("\n");
				}
				else {
					IZ_PRINTF(", ");
				}
			}

			izanagi::SMatrix::Mul(mtx, mtx, tmp);
		}

		IZ_PRINTF("============\n");
		for (IZ_UINT n = 0; n < 16; n++) {
			IZ_PRINTF("%f", mtx.a[n]);
			if ((n % 4) == 3) {
				IZ_PRINTF("\n");
			}
			else {
				IZ_PRINTF(", ");
			}
		}

		izanagi::SQuat quatEnd;
		izanagi::SQuat::QuatFromMatrix(quatEnd, mtx);

		IZ_PRINTF("quat: %f, %f, %f, %f\n", quatEnd.x, quatEnd.y, quatEnd.z, quatEnd.w); 
	}
}	// namespace

// 更新
IZ_BOOL CMyAppl::Update()
{
	test0();
	test1();
	return IZ_TRUE;
}

// 描画
IZ_BOOL CMyAppl::Render()
{
	static const D3DCOLOR color = D3DCOLOR_ARGB(255, 0, 128, 255);

	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();

	pDevice->BeginRender(
		izanagi::E_GRAPH_CLEAR_FLAG_ALL,
		color, 1.0f, 0);

	// Nothing is done.

	pDevice->EndRender();

	return IZ_TRUE;
}