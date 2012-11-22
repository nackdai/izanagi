#if !defined(__IZANAGI_POSTEFFECT_FUNCTOR_CREATOR_H__)
#define __IZANAGI_POSTEFFECT_FUNCTOR_CREATOR_H__

#include "izStd.h"
#include "PostEffectFunctor.h"
#include "posteffect/util/PostEffectUtilityParam.h"
#include "posteffect/PostEffectSystem.h"

namespace izanagi {
	/////////////////////////////////////////////////////
	/**
	* ファンクタクリエータインターフェース
	*/
	class IPostEffectFunctorCreator {
		friend class CPostEffectSystem;

	protected:
		IPostEffectFunctorCreator() {}
		virtual ~IPostEffectFunctorCreator() {}

	protected:
		// ファンクタ作成
		PURE_VIRTUAL(CPostEffectFunctor* CreateFunctor(const CKey& cKey, void* pBuffer));

		// ファンクタサイズ取得
		PURE_VIRTUAL(size_t GetFunctorSize() const);

		PURE_VIRTUAL(IZ_BOOL IsValid(IZ_UINT nKey) const);

		CPostEffectSystem::CFunctorCreatorHashItem* GetHashItem() { return &m_HashItem; }

	protected:
		enum {
			FUNCTOR_ID_LEN = 31,
		};

		CStdString<izChar, FUNCTOR_ID_LEN> m_Id;
		CKey m_Key;

		CPostEffectSystem::CFunctorCreatorHashItem m_HashItem;
	};

	/////////////////////////////////////////////////////
	/**
	* ファンクタクリエータ
	*/
	template <class _T>
	class CPostEffectFunctorCreator : public IPostEffectFunctorCreator {
	public:
		CPostEffectFunctorCreator(IZ_PCSTR pszName)
		{
			m_Id.SetString(pszName);
			m_Key.SetValue(pszName);

			m_HashItem.Init(m_Key, this);
		}
		
		~CPostEffectFunctorCreator() {}

	private:
		CPostEffectFunctorCreator() {}
		
		NO_COPIABLE(CPostEffectFunctorCreator);

	private:
		// ファンクタ作成
		CPostEffectFunctor* CreateFunctor(
			const CKey& cKey,
			void* pBuffer)
		{
			CPostEffectFunctor* ret = CPostEffectFunctor::CreateFunctor<_T>(cKey, pBuffer);
			IZ_ASSERT(ret != NULL);
			return ret;
		}

		// ファンクタサイズ取得
		size_t GetFunctorSize() const { return sizeof(_T); }

		IZ_BOOL IsValid(IZ_UINT nKey) const { return (m_Key == nKey); }
		IZ_BOOL IsValid(const CKey& cKey) const { return (m_Key == cKey); }
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_POSTEFFECT_FUNCTOR_CREATOR_H__)
