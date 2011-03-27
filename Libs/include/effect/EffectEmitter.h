#if !defined(__IZANAGI_EFFECT_EFFECT_EMITTER_H__)
#define __IZANAGI_EFFECT_EFFECT_EMITTER_H__

#include "izMath.h"
#include "EffectParticle.h"
#include "EffectParticleManager.h"
#include "EffectShape.h"

namespace izanagi {
	/**
	*/
	struct SEffectEmitterParam {
		IZ_FLOAT	fEmmitRatio;
		IZ_FLOAT	fEmmitInterval;

		IZ_UINT32	fEmmitTime;
		IZ_UINT32	nEmmitCount;

		SVector vScale;
		SVector vRotate;
		SVector vTranslate;
	};

	// エミッタ
	class CEffectEmitter {
	protected:
		CEffectEmitter();
		~CEffectEmitter();

		NO_COPIABLE(CEffectEmitter);

	public:
		void Update(IZ_FLOAT fDelta);

		void SetParam(const SEffectEmitterParam& sParam) { memcpy(&m_Param, &sParam, sizeof(SEffectEmitterParam)); }
		const SEffectEmitterParam& GetParam() const { return m_Param; }

	protected:
		void SetParticleManager(CEffectParticleManager* pMgr) { SAFE_REPLACE(m_pParticleMgr, pMgr); }

		CStdList<CEffectEmitter>::Item* GetListItem() { return &m_ListItem; }

	protected:
		SEffectEmitterParam m_Param;

		CEffectShape* m_pEmitterShape;

		CEffectParticleManager* m_pParticleMgr;

		CStdList<CEffectEmitter>::Item m_ListItem;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_EFFECT_EFFECT_EMITTER_H__)
