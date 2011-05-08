#if !defined(__IZANAGI_SCENEGRAPH_SCENE_H__)
#define __IZANAGI_SCENEGRAPH_SCENE_H__

#include "izStd.h"
#include "scenegraph/SceneParam.h"

namespace izanagi {
	class CGraphicsDevice;
	class CMaterial;
	class CBaseTexture;
	class CShader;
	class CMeshSetInstance;
	class IMshRenderHandler;

	/**
	 */
	class CScene : public CObject {
		friend class CGeometrySorter;

	public:
		/**
		 * インスタンス作成
		 */
		static CScene* CreateScene(IMemoryAllocator* pAllocator);

	protected:
		CScene();
		~CScene();

		NO_COPIABLE(CScene);

		IZ_DEFINE_INTERNAL_RELEASE();

	protected:
		// 描画開始
		IZ_UINT BeginRender(CMeshSetInstance* pMesh);

		// 描画途中
		IZ_BOOL IterRender(
			CGraphicsDevice* pDevice,
			IZ_UINT nPass,
			CMeshSetInstance* pMesh,
			IMshRenderHandler* pRenderHandler);

		// 描画終了
		IZ_BOOL EndRender();

	public:
		void SetSceneParam(CSceneParam* pParam) { SAFE_REPLACE(m_pSceneParam, pParam); }
		CSceneParam* GetSceneParam() { return m_pSceneParam; }

	private:
		IMemoryAllocator* m_pAllocator;

		CSceneParam* m_pSceneParam;

		CShader* m_pCurShader;
		IZ_UINT m_nCurShaderPassNum;
		IZ_INT m_nCurShaderPass;

		CMaterial* m_pCurMtrl;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_SCENE_H__)
