#include "StateFont.h"
#include "MySystem.h"

CStateFont::CStateFont()
{
	m_pTex = IZ_NULL;
	m_pFontRenderer = IZ_NULL;
}

CStateFont::~CStateFont()
{
}

IZ_BOOL CStateFont::Init()
{
	return IZ_TRUE;
}

IZ_BOOL CStateFont::Render(izanagi::graph::CGraphicsDevice* device)
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

#if 0
		CMySystem::GetInstance().GetDebugFont()->DBPrint(
			0xffffffff,
			"test\n");
#endif

		CMySystem::GetInstance().GetDebugFont()->End();

		// フォント描画
		IZ_ASSERT(m_pFontRenderer != NULL);
		m_pFontRenderer->Register("あいうえおかきくけこabcdefghijklmnopqrstuvwxyz");
		m_pFontRenderer->Render(
			"あいうえおかきくけこabcdefghijklmnopqrstuvwxyz",
			0, 200,
			izanagi::CColor::GREEN);

		m_pFontRenderer->Register("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		m_pFontRenderer->Render(
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
			0, 250,
			izanagi::CColor::GREEN);

		CMySystem::GetInstance().GetGraphicsDevice()->End2D();
	}

	return IZ_TRUE;
}

IZ_BOOL CStateFont::Update()
{
	return IZ_TRUE;
}

IZ_BOOL CStateFont::Destroy()
{
	SAFE_RELEASE(m_pTex);
	return IZ_TRUE;
}

IZ_BOOL CStateFont::Enter(izanagi::IMemoryAllocator* allocator, void* val)
{
	izanagi::graph::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();
	izanagi::IMemoryAllocator* pAllocator = CMySystem::GetInstance().GetMemoryAllocator();

	izanagi::CFileInputStream cIn;
	VRETURN(cIn.Open("./data/test.fnt"));

	// FNT読み込み
	m_pFontRenderer = izanagi::CFontRenderer::CreateFontRendererBmp(
						pAllocator,
						pDevice,
						40,
						&cIn);

	cIn.Close();

	return IZ_TRUE;
}

IZ_BOOL CStateFont::Leave()
{
	SAFE_RELEASE(m_pFontRenderer);
	return IZ_TRUE;
}

IZ_BOOL CStateFont::OnKeyDown(IZ_UINT nChar)
{
	return IZ_TRUE;
}

IZ_BOOL CStateFont::OnMouseMove(IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}

IZ_BOOL CStateFont::OnMouseWheel(IZ_SHORT zDelta)
{
	return IZ_TRUE;
}