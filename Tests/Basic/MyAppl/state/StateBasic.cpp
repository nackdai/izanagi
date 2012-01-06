#include "StateBasic.h"
#include "MySystem.h"

CStateBasic::CStateBasic()
{
	m_pTex = IZ_NULL;
}

CStateBasic::~CStateBasic()
{
}

IZ_BOOL CStateBasic::Init()
{
	return IZ_TRUE;
}

namespace {
	#define _Print CMySystem::GetInstance().GetDebugFont()->DBPrint
	void _PrintPadInfo()
	{
		if (CMySystem::GetInstance().GetPad() != IZ_NULL) {
			const izanagi::CPad::PadState& state = CMySystem::GetInstance().GetPad()->GetCurState();

			_Print("AxisPos [%d][%d][%d]\n", state.lX, state.lY, state.lZ);
			_Print("AxisRot [%d][%d][%d]\n", state.lRx, state.lRy, state.lRz);
			_Print("Slider  [%d][%d]\n", state.rglSlider[0], state.rglSlider[1]);
			_Print(
				"Pov     [%d][%d][%d][%d]\n", 
				state.rgdwPOV[0], state.rgdwPOV[1], state.rgdwPOV[2], state.rgdwPOV[3]);
			for (IZ_UINT i = 0; i < 32; i += 2) {
				_Print(
					"Button  %d[%d] %d[%d]\n",
					i, state.rgbButtons[i],
					i + 1, state.rgbButtons[i + 1]);
			}
		}
	}
}	// namespace

IZ_BOOL CStateBasic::Render(izanagi::CGraphicsDevice* device)
{
	if (CMySystem::GetInstance().GetGraphicsDevice()->Begin2D()) {
		CMySystem::GetInstance().GetDebugFont()->Begin();

#if 1
		{
			IZ_FLOAT time = CMySystem::GetInstance().GetTimer(0).GetTime();
			IZ_FLOAT fps = 1000.0f / time;

			CMySystem::GetInstance().GetDebugFont()->DBPrint(
				"%.2f[ms] %.2f[fps]\n",
				time, fps);
		}
#endif

#if 1
		{
			IZ_FLOAT time = CMySystem::GetInstance().GetTimer(1).GetTime();
			IZ_FLOAT fps = 1000.0f / time;

			CMySystem::GetInstance().GetDebugFont()->DBPrint(
				"%.2f[ms] %.2f[fps]\n",
				time, fps);
		}
#endif

		_PrintPadInfo();

		CMySystem::GetInstance().GetDebugFont()->DBPrint(
			0xffffffff,
			"test\n");

		CMySystem::GetInstance().GetDebugFont()->End();

		// DBPrint内でセットされているテクスチャをスプライト表示してみる
#if 1
#if 1
		if (m_pTex != IZ_NULL) {
			CMySystem::GetInstance().GetGraphicsDevice()->SetTexture(0, m_pTex);
			CMySystem::GetInstance().GetGraphicsDevice()->Set2DRenderOp(izanagi::E_GRAPH_2D_RENDER_OP_MODULATE);
			CMySystem::GetInstance().GetGraphicsDevice()->Draw2DSprite(
				izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
				izanagi::CIntRect(300, 100, 556, 228));
		}

		CMySystem::GetInstance().GetGraphicsDevice()->Draw2DRect(
			izanagi::CIntRect(100, 100, 200, 200),
			IZ_COLOR_RGBA(0, 0xff, 0, 0xff));

		CMySystem::GetInstance().GetGraphicsDevice()->Draw2DLine(
			izanagi::CIntPoint(100, 100),
			izanagi::CIntPoint(200, 200),
			IZ_COLOR_RGBA(0xff, 0, 0, 0xff));
#else
		CMySystem::GetInstance().GetGraphicsDevice()->Draw2DSpriteEx(
			CIntRect(0, 0, 256, 128),
			CIntRect(0, 100, 256, 228));
#endif
#endif

		CMySystem::GetInstance().GetGraphicsDevice()->End2D();
	}

	return IZ_TRUE;
}

IZ_BOOL CStateBasic::Update()
{
	return IZ_TRUE;
}

IZ_BOOL CStateBasic::Destroy()
{
	SAFE_RELEASE(m_pTex);
	return IZ_TRUE;
}

IZ_BOOL CStateBasic::Enter(izanagi::IMemoryAllocator* allocator, void* val)
{
#if 1
	m_pTex = CMySystem::GetInstance().GetGraphicsDevice()->CreateTextureFromFile(
				"../data/test.bmp",
				izanagi::E_GRAPH_PIXEL_FMT_RGBA8);
#else
	izanagi::CFileInputStream cInput;
	cInput.Open("../data/test.bmp");

	izanagi::IMemoryAllocator* pAllocator = CMySystem::GetInstance().GetMemoryAllocator();

	IZ_UINT nSize = cInput.GetFileSize();
	void* pBuf = ALLOC_ZERO(pAllocator, nSize);

	cInput.Read(pBuf, 0, nSize);

	cInput.Close();

	m_pTex = CMySystem::GetInstance().GetGraphicsDevice()->CreateTextureFromMemory(
				pBuf, nSize,
				D3DFMT_A8R8G8B8);

	pAllocator->Free(pBuf);
#endif
	return IZ_TRUE;
}

IZ_BOOL CStateBasic::Leave()
{
	SAFE_RELEASE(m_pTex);
	return IZ_TRUE;
}

IZ_BOOL CStateBasic::OnKeyDown(IZ_UINT nChar)
{
	return IZ_TRUE;
}

IZ_BOOL CStateBasic::OnMouseMove(IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}

IZ_BOOL CStateBasic::OnMouseWheel(IZ_SHORT zDelta)
{
	return IZ_TRUE;
}