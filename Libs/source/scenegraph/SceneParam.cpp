#include "scenegraph/SceneParam.h"

using namespace izanagi;

#define SET_BUF(p, buf, type, num) \
	p = reinterpret_cast<type *>(buf); \
	buf += sizeof(type) * num

/**
* インスタンス作成
*/
CSceneParam* CSceneParam::CreateSceneParam(
	IMemoryAllocator* pAllocator,
	IZ_UINT nParallelLightNum,
	IZ_UINT nPointLightNum,
	IZ_UINT nSpotLightNum)
{
	IZ_ASSERT(pAllocator != IZ_NULL);

	CSceneParam* pInstance = IZ_NULL;

	// メモリサイズ
	size_t nSize = sizeof(CSceneParam);
	nSize += sizeof(SParallelLightParam) * nParallelLightNum;
	nSize += sizeof(SPointLightParam) * nPointLightNum;
	nSize += sizeof(SSpotLightParam) * nSpotLightNum;

	// メモリ確保
	IZ_BYTE* pBuf = reinterpret_cast<IZ_BYTE*>(ALLOC_ZERO(pAllocator, nSize));
	IZ_BOOL result = (pBuf != IZ_NULL);
	VGOTO(result, __EXIT__);

	// インスタンス作成
	pInstance = new (pBuf) CSceneParam;
	{
		pBuf += sizeof(CSceneParam);

		pInstance->AddRef();

		pInstance->m_pAllocator = pAllocator;

		SET_BUF(pInstance->m_pParallel, pBuf, SParallelLightParam, nParallelLightNum);
		SET_BUF(pInstance->m_pPoint, pBuf, SPointLightParam, nPointLightNum);
		SET_BUF(pInstance->m_pSpot, pBuf, SSpotLightParam, nSpotLightNum);

		FILL_ZERO(&pInstance->m_sAmbient, sizeof(pInstance->m_sAmbient));
		FILL_ZERO(&pInstance->m_sHemisphere, sizeof(pInstance->m_sHemisphere));

		pInstance->m_nParallelLightNum = nParallelLightNum;
		pInstance->m_nPointLightNum = nPointLightNum;
		pInstance->m_nSpotLightNum = nSpotLightNum;
	}

__EXIT__:
	if (!result) {
		if (pInstance != IZ_NULL) {
			SAFE_RELEASE(pInstance);
		}
		else if (pBuf != IZ_NULL) {
			FREE(pAllocator, pBuf);
		}
	}

	return pInstance;
}

// コンストラクタ
CSceneParam::CSceneParam()
{
	m_pAllocator = IZ_NULL;

	m_pParallel = IZ_NULL;
	m_pPoint = IZ_NULL;
	m_pSpot = IZ_NULL;

	m_nParallelLightNum = 0;
	m_nPointLightNum = 0;
	m_nSpotLightNum = 0;
}
