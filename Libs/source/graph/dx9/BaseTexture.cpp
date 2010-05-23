#include "graph/dx9/BaseTexture.h"
#include "graph/dx9/GraphicsDevice.h"

using namespace uranus;

// コンストラクタ
CBaseTexture::CBaseTexture()
{
	m_pDevice = UN_NULL;
	m_pAllocator = UN_NULL;

	ZeroMemory(&m_TexInfo, sizeof(m_TexInfo));
}

// デストラクタ
CBaseTexture::~CBaseTexture()
{
	SAFE_RELEASE(m_pDevice);
}
