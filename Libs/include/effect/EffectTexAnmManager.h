#if !defined(__IZANAGI_EFFECT_EFFECT_TEX_ANM_MANAGER_H__)
#define __IZANAGI_EFFECT_EFFECT_TEX_ANM_MANAGER_H__

#include "izStd.h"

namespace izanagi {
	class CTexture;
	class IInputStream;

	/**
	*/
	struct SEffectTexAnmParam {
		IZ_UINT frame;
		SFloatPoint ptLeftTopUV;
		SFloatPoint ptRightBottomUV;

	private:
		CStdList<SEffectTexAnmParam>::Item item;

		inline SEffectTexAnmParam();
		inline CStdList<SEffectTexAnmParam>::Item* GetItem();

		friend class CEffectTexAnmManager;
	};

	/**
	*/
	class CEffectTexAnmManager : public CObject {
	public:
		static CEffectTexAnmManager* CreateEffectTexAnmManager(
			IMemoryAllocator* pAllocator,
			IInputStream* pIn,
			IZ_UINT nParamNum);

		static CEffectTexAnmManager* CreateEffectTexAnmManager(
			IMemoryAllocator* pAllocator);

	protected:
		CEffectTexAnmManager();
		~CEffectTexAnmManager();

		NO_COPIABLE(CEffectTexAnmManager);

	public:

	public:
		IZ_UINT GetParamNum() { return m_ParamList.GetItemNum(); }

		const SEffectTexAnmParam& GetParamAt(IZ_UINT idx);
		IZ_BOOL AddParamTop(const SEffectTexAnmParam& param);
		IZ_BOOL AddParamTail(const SEffectTexAnmParam& param);
		IZ_BOOL InsertParam(
			IZ_UINT idx,
			const SEffectTexAnmParam& param);

		CTexture* GetTexture();

	protected:
		IZ_DEFINE_INTERNAL_RELEASE();

	protected:
		IMemoryAllocator* m_pAllocator;

		CTexture* m_pTex;
		CStdList<SEffectTexAnmParam> m_ParamList;

		SEffectTexAnmParam* m_pParamBuffer;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_EFFECT_EFFECT_TEX_ANM_MANAGER_H__)
