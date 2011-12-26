#include "effect/EffectParticleManager.h"
#include "izGraph.h"

using namespace izanagi;

// 更新
void CEffectParticleManager::Update(IZ_FLOAT fDelta)
{
	CStdList<CEffectParticle>& cParticlieList = m_ParticleList[E_EFFECT_STATE_ACTIVE];

	while (CStdList<CEffectParticle>::Item* pItem = cParticlieList.GetTop()) {
		CEffectParticle* pParticle = pItem->GetData();
		IZ_ASSERT(pParticle != IZ_NULL);

		pParticle->Update(fDelta);
	}
}

// 描画
void CEffectParticleManager::Render(CGraphicsDevice* pDevice)
{
	IZ_ASSERT(pDevice != IZ_NULL);

	CStdList<CEffectParticle>& cParticlieList = m_ParticleList[E_EFFECT_STATE_ACTIVE];

	while (CStdList<CEffectParticle>::Item* pItem = cParticlieList.GetTop()) {
		CEffectParticle* pParticle = pItem->GetData();
		IZ_ASSERT(pParticle != IZ_NULL);

		pParticle->Render(pDevice);
	}
}
