#if !defined(__IZANAGI_EFFECT_EFFECT_OBJECT_H__)
#define __IZANAGI_EFFECT_EFFECT_OBJECT_H__

namespace izanagi {
	/**
	*/
	class CEffectObject : public CObject {
	public:
		// インスタンス作成
		static CEffectObject CreateEffectObject(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice);

	protected:
		CEffectObject();
		~CEffectObject();

	public:
		void Update(IZ_FLOAT fDelta);
		void Render();

		IZ_INT AddEmitter(CEffectEmitter* pEmitter);
		IZ_INT AddParticleManager(CEffectParticleManager* pMgr);

		IZ_BOOL RemoveEmitter(IZ_UINT idx);
		IZ_BOOL RemoveParticleManager(IZ_UINT idx);

	protected:
		IMemoryAllocator* m_pAllocator;
		CGraphicsDevice* m_pDevice;

		CStdList<CEffectEmitter> m_EmitterList;
		CStdList<CEffectParticleManager> m_ParticleMgrList;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_EFFECT_EFFECT_OBJECT_H__)
