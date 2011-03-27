#include "effect/EffectTexAnmManager.h"
#include "izGraph.h"
#include "izIo.h"

using namespace izanagi;

/**
*/
CEffectTexAnmManager* CEffectTexAnmManager::CreateEffectTexAnmManager(
	IMemoryAllocator* pAllocator,
	IInputStream* pIn,
	IZ_UINT nParamNum)
{
	// TODO
	return IZ_NULL;
}

/**
*/
CEffectTexAnmManager* CEffectTexAnmManager::CreateEffectTexAnmManager(
	IMemoryAllocator* pAllocator)
{
	CEffectTexAnmManager* ret = CreateEffectTexAnmManager(
									pAllocator,
									IZ_NULL,
									0);
	return ret;
}

CEffectTexAnmManager::CEffectTexAnmManager()
{
	m_pAllocator = IZ_NULL;

	m_pTex = IZ_NULL;
	m_pParamBuffer = IZ_NULL;
}

CEffectTexAnmManager::~CEffectTexAnmManager()
{
	SAFE_RELEASE(m_pTex);
}
