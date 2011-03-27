#include "effect/EffectEmitter.h"

using namespace izanagi;

void CEffectEmitter::Update(IZ_FLOAT fDelta)
{
	// TODO
	// Compute number of emitting particlies.
	IZ_UINT nNewParticleNum = 0;

	for (IZ_UINT i = 0; i < nNewParticleNum; ++i) {
		CEffectParticle* pParticle = m_pParticleMgr->GetParticleTopFromList(E_EFFECT_STATE_ACTIVE);
		if (pParticle != IZ_NULL) {
			// TODO
			// Register remain particles which are not created.
			break;
		}

		// Add an active particle to CParticleManager.
		IZ_ASSERT(m_pParticleMgr->AddParticle(
					pParticle,
					E_EFFECT_STATE_ACTIVE));

		// TODO
		// Set initial particle parameter.

		// position
		//m_pEmitterShape->GetPos();
	}

	return ;
}
