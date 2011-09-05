#if !defined(__IZANAGI_SCENEGRAPH_SCENE_RENDERER_H__)
#define __IZANAGI_SCENEGRAPH_SCENE_RENDERER_H__

#include "izStd.h"
#include "scenegraph/SceneParam.h"

namespace izanagi {
	class CGraphicsDevice;
	class CMaterial;
	class CBaseTexture;
	class CShader;
	class IMeshSet;
	class IMshRenderHandler;

	/**
	 */
	class CSceneRenderer : public CObject {
		friend class CGeometrySorter;

	public:
		/**
		 * インスタンス作成
		 */
		static CSceneRenderer* CreateSceneRenderer(IMemoryAllocator* pAllocator);

	protected:
		CSceneRenderer();
		~CSceneRenderer();

		NO_COPIABLE(CSceneRenderer);

		IZ_DEFINE_INTERNAL_RELEASE();

	protected:
		// 描画開始
		IZ_UINT BeginRender(IMeshSet* pMesh);

		// 描画途中
		IZ_BOOL IterRender(
			CGraphicsDevice* pDevice,
			IZ_UINT nPass,
			IMeshSet* pMesh,
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

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_SCENE_RENDERER_H__)
