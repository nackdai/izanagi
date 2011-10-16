#include "DebugInfoRenderer.h"
#include "MySystem.h"

void CDebugInfoRenderer::Render(IZ_PCSTR pszStr)
{
	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();
	izanagi::CDebugFont* pFont = CMySystem::GetInstance().GetDebugFont();

	if (pDevice->Begin2D()) {
		pFont->Begin();

#if 1
		{
			IZ_FLOAT time = CMySystem::GetInstance().GetTimer(0).GetTime();
			IZ_FLOAT fps = 1000.0f / time;

			pFont->DBPrint(
				"%.2f[ms] %.2f[fps]\n",
				time, fps);
		}
#endif

#if 1
		{
			IZ_FLOAT time = CMySystem::GetInstance().GetTimer(1).GetTime();
			IZ_FLOAT fps = 1000.0f / time;

			pFont->DBPrint(
				"%.2f[ms] %.2f[fps]\n",
				time, fps);
		}
#endif

		if (pszStr != IZ_NULL) {
			pFont->DBPrint(
				0xffffffff,
				"%s\n",
				pszStr);
		}

		pFont->End();

		pDevice->End2D();
	}
}
