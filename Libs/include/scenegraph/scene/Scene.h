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

	private:
		CScene();
		~CScene();

		NO_COPIABLE(CScene);

		IZ_DEFINE_INTERNAL_RELEASE();

	public:
		/**
		*/
		template <typename _T_MESH>
		IZ_UINT Begin(_T_MESH* pMesh)
		{
			CMaterial* pMtrl = pMesh->GetMaterial();

			if (pMtrl == IZ_NULL) {
				const S_MSH_MTRL& sMtrlInfo = pMesh->GetMaterialInfo();
				pMtrl = GetMaterial(sMtrlInfo.nameKey);
				pMesh->SetMaterial(pMtrl);
			}

			IZ_ASSERT(pMtrl != IZ_NULL);

			CShader* pShader = pMtrl->GetShader();
			VRETURN(pShader != IZ_NULL);

			if ((m_pCurShader == IZ_NULL)
				|| (m_pCurShader->GetKey() != pShader->GetKey()))
			{
				SAFE_REPLACE(m_pCurShader, pShader);

				IZ_INT nTechIdx = pMtrl->GetShaderTechnique();
				IZ_ASSERT(nTechIdx >= 0);

				m_nCurShaderPassNum = pShader->Begin(nTechIdx);
			}

			return m_nCurShaderPassNum;
		}

		/**
		*/
		template <typename _T_MESH>
		IZ_BOOL Iter(IZ_UINT nPass, _T_MESH* pMesh)
		{
			IZ_ASSERT(m_nCurShaderPassNum > 0);
			IZ_ASSERT(m_nCurShaderPassNum > nPass);

			CMaterial* pMtrl = pMesh->GetMaterial();
			IZ_ASSERT(pMtrl != IZ_NULL);
			IZ_ASSERT(m_pCurShader != IZ_NULL);
			
			if (m_pCurShader->GetKey() != pMtrl->GetShader()->GetKey()) {
				// Specify differing shader.
				return IZ_FALSE;
			}

			if (m_nCurShaderPass != nPass) {
				// Change pass.
				VRETURN(m_pCurShader->BeginPass(nPass));
				m_nCurShaderPass = nPass;
			}

			// Set parameters to shader.
			SetShaderParam(
				m_pCurShader, 
				pMtrl, 
				m_pSceneParam);

			// TODO
			// Set textures.

			VRETURN(m_pCurShader->CommitChages());

			// TODO
			// Render mesh.

			return IZ_TRUE;
		}

		IZ_BOOL End();

	public:
		void SetSceneParam(CSceneParam* pParam) { SAFE_REPLACE(m_pSceneParam, pParam); }
		CSceneParam* GetSceneParam() { return m_pSceneParam; }

	private:
		IMemoryAllocator* m_pAllocator;

		CSceneParam* m_pSceneParam;

		CShader* m_pCurShader;
		IZ_UINT m_nCurShaderPassNum;
		IZ_INT m_nCurShaderPass;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_SCENE_H__)
