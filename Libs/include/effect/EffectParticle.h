#if !defined(__IZANAGI_EFFECT_EFFECT_PARTICLE_H__)
#define __IZANAGI_EFFECT_EFFECT_PARTICLE_H__

#include "izStd.h"

namespace izanagi {
	class CGraphicsDevice;

	/**
	*/
	class CEffectParticle {
		friend class CEffectParticleManager;

	protected:
		CEffectParticle() {}
		virtual ~CEffectParticle() {}

		NO_COPIABLE(CEffectParticle);

	protected:
		// 更新
		virtual void Update(IZ_FLOAT fDelta) = 0;

		// 描画
		virtual void Render(CGraphicsDevice* pDevice) = 0;

		CStdList<CEffectParticle>::Item* GetListItem() { return &m_ListItem; }

	protected:
		CStdList<CEffectParticle>::Item m_ListItem;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_EFFECT_EFFECT_PARTICLE_H__)
