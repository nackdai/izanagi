#include "graph/dx9/BaseTexture.h"
#include "graph/dx9/GraphicsDevice.h"

using namespace izanagi;

// コンストラクタ
CBaseTexture::CBaseTexture()
{
	m_pDevice = IZ_NULL;
	m_pAllocator = IZ_NULL;

	ZeroMemory(&m_TexInfo, sizeof(m_TexInfo));
}

// デストラクタ
CBaseTexture::~CBaseTexture()
{
	SAFE_RELEASE(m_pDevice);
}
