#if !defined(__IZANAGI_SCENEGRAPH_SCENE_H__)
#define __IZANAGI_SCENEGRAPH_SCENE_H__

#include "izStd.h"
#include "scenegraph/SceneParam.h"

namespace izanagi {
	class CGraphicsDevice;
	class CMaterial;
	class CBaseTexture;
	class CShader;

	/**
	*/
	class CScene : public CObject {
	public:
		static CScene* CreateScene(IMemoryAllocator* pAllocator);

		enum {
			HASH_SIZE = 5,
		};

	private:
		template <class _T>
		class CHashElem : public CPlacementNew {
		public:
			static CHashElem* CreateHashElem(
				IMemoryAllocator* pAllocator,
				const CKey& cKey,
				_T* p)
			{
				void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CHashElem));
				VRETURN_NULL(pBuf);
				CHashElem* pInstance = new(pBuf) CHashElem;
				SAFE_REPLACE(pInstance->m_pData, p);
				//pInstance->m_HashItem.Init(cKey, pInstance);
				return pInstance;
			}

			static void DeleteHashElem(
				IMemoryAllocator* pAllocator,
				CHashElem* p)
			{
				delete p;
				FREE(pAllocator, p);
			}

		public:
			CHashElem()
			{
				m_pData = IZ_NULL;
			}
			~CHashElem()
			{
				SAFE_RELEASE(m_pData);
			}

		public:
			CStdHash<CKey, CHashElem<class _T>, HASH_SIZE>::Item* GetHashItem() { return &m_HashItem; }
			_T* GetData() { return m_pData; }

		public:
			_T* m_pData;

			CStdHash<CKey, CHashElem<class _T>, HASH_SIZE>::Item m_HashItem;
		};

		typedef CStdHash<CKey, CHashElem<CMaterial>, HASH_SIZE>		CMtrlHash;
		typedef CStdHash<CKey, CHashElem<CBaseTexture>, HASH_SIZE>	CTexHash;
		typedef CStdHash<CKey, CHashElem<CShader>, HASH_SIZE>		CShaderHash;

	private:
		CScene();
		~CScene();

		NO_COPIABLE(CScene);

		IZ_DEFINE_INTERNAL_RELEASE();

	public:
		template <typename _T_MESH>
		void Render(
			CGraphicsDevice* pDevice,
			_T_MESH* pMesh)
		{
			CMaterial* pMtrl = pMesh->GetMaterial();

			if (pMtrl == IZ_NULL) {
				const S_MSH_MTRL& sMtrlInfo = pMesh->GetMaterialInfo();
				pMtrl = GetMaterial(sMtrlInfo.nameKey);
				pMesh->SetMaterial(pMtrl);
			}

			IZ_ASSERT(pMtrl != IZ_NULL);

			// TODO
		}

	public:
		void SetMaterial(const CKey& cKey, CMaterial* pMtrl) { Set(m_MtrlHash, cKey, pMtrl); }
		CMaterial* GetMaterial(const CKey& cKey) { return Get(m_MtrlHash, cKey); }
		IZ_BOOL RemoveMaterial(const CKey& cKey) { return Remove(m_MtrlHash, cKey); }
		IZ_UINT GetMaterialNum() const { return GetNum(m_MtrlHash); }

		void SetTexture(const CKey& cKey, CBaseTexture* pTex) { Set(m_TexHash, cKey, pTex); }
		CBaseTexture* GetTexture(const CKey& cKey) { return Get(m_TexHash, cKey); }
		IZ_BOOL RemoveTexture(const CKey& cKey) { return Remove(m_TexHash, cKey); }
		IZ_UINT GetTextureNum() const { return GetNum(m_TexHash); }

		void SetShader(const CKey& cKey, CShader* pShader) { Set(m_ShaderHash, cKey, pShader); }
		CShader* GetShader(const CKey& cKey) { return Get(m_ShaderHash, cKey); }
		IZ_BOOL RemoveShader(const CKey& cKey) { return Remove(m_ShaderHash, cKey); }
		IZ_UINT GetShaderNum() const { return GetNum(m_ShaderHash); }

	private:
		template <typename _T>
		void Set(
			CStdHash<CKey, CHashElem<_T>, HASH_SIZE>& cHash, 
			const CKey& cKey, 
			_T* pData);

		template <typename _T>
		_T* Get(
			CStdHash<CKey, CHashElem<_T>, HASH_SIZE>& cHash, 
			const CKey& cKey);

		template <typename _T>
		IZ_BOOL Remove(
			CStdHash<CKey, CHashElem<_T>, HASH_SIZE>& cHash, 
			const CKey& cKey);

		template <typename _T>
		IZ_UINT GetNum(const CStdHash<CKey, CHashElem<_T>, HASH_SIZE>& cHash) const;

	public:
		void SetSceneParam(CSceneParam* pParam) { SAFE_REPLACE(m_pSceneParam, pParam); }
		CSceneParam* GetSceneParam() { return m_pSceneParam; }

	private:
		IMemoryAllocator* m_pAllocator;

		CSceneParam* m_pSceneParam;

		CMtrlHash m_MtrlHash;
		CTexHash m_TexHash;
		CShaderHash m_ShaderHash;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_SCENE_H__)
