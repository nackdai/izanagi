#if !defined(__IZANAGI_EFFECT_EFFECT_PARTICLE_MANAGER_H__)
#define __IZANAGI_EFFECT_EFFECT_PARTICLE_MANAGER_H__

#include "izStd.h"
#include "EffectDefs.h"
#include "EffectParticle.h"

namespace izanagi {
	class CGraphicsDevice;

	/**
	*/
	class CEffectParticleManager : public CObject {
		friend class CEffectEmitter;

	public:
		static CEffectParticleManager* CreateEffectParticleManager(
			IMemoryAllocator* pAllocator);

	protected:
		CEffectParticleManager();
		~CEffectParticleManager();

		NO_COPIABLE(CEffectParticleManager);

	protected:
		IZ_DEFINE_INTERNAL_RELEASE();

	public:
		// 更新
		void Update(IZ_FLOAT fDelta);

		// 描画
		void Render(CGraphicsDevice* pDevice);

	protected:
		inline CEffectParticle* GetParticleTopFromList(E_EFFECT_STATE nState);

		inline IZ_BOOL AddParticle(
			CEffectParticle* pParticle,
			E_EFFECT_STATE nState);

	protected:
		IMemoryAllocator* m_pAllocator;

		CStdList<CEffectParticle> m_ParticleList[E_EFFECT_STATE_NUM];
	};

	// inline **********************************************

	// Return specified state list.
	CEffectParticle* CEffectParticleManager::GetParticleTopFromList(E_EFFECT_STATE nState)
	{
		IZ_ASSERT(nState < E_EFFECT_STATE_NUM);

		CStdList<CEffectParticle>::Item* pItem = m_ParticleList[nState].GetTop();
		return (pItem != IZ_NULL ? pItem->GetData() : IZ_NULL);
	}

	// Add particle to specified state list.
	IZ_BOOL CEffectParticleManager::AddParticle(
		CEffectParticle* pParticle,
		E_EFFECT_STATE nState)
	{
		IZ_ASSERT(nState < E_EFFECT_STATE_NUM);
		IZ_ASSERT(pParticle != IZ_NULL);

		IZ_BOOL ret = m_ParticleList[nState].AddTail(pParticle->GetListItem());
		IZ_ASSERT(ret);

		return ret;
	}

}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_EFFECT_EFFECT_PARTICLE_MANAGER_H__)
