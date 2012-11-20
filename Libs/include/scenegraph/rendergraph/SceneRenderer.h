#if !defined(__IZANAGI_SCENEGRAPH_SCENE_RENDERER_H__)
#define __IZANAGI_SCENEGRAPH_SCENE_RENDERER_H__

#include "izStd.h"
#include "izGraph.h"
#include "scenegraph/rendergraph/SceneRendererInterface.h"
#include "scenegraph/SceneParam.h"

namespace izanagi {
	class CMaterial;
	class IShader;
	class IMeshSet;
	class IMshRenderHandler;

	/**
	 */
	class CSceneRenderer : public ISceneRenderer {
		friend class CGeometrySorter;

	public:
		/** インスタンス作成.
		 */
		static CSceneRenderer* CreateSceneRenderer(IMemoryAllocator* pAllocator);

	protected:
		CSceneRenderer();
		virtual ~CSceneRenderer();

		NO_COPIABLE(CSceneRenderer);

		IZ_DEFINE_INTERNAL_RELEASE();

	protected:
		/** 描画開始.
		 */
		virtual IZ_UINT BeginRender(
			IShader* shader,
			IZ_UINT techIdx);

		/** 描画途中.
		 */
		virtual IZ_BOOL IterRender(
			graph::CGraphicsDevice* device,
			IZ_UINT passIdx,
			IRenderElement* element,
			IMshRenderHandler* renderHandler);

		/** 描画終了.
		 */
		virtual IZ_BOOL EndRender();

	public:
		/** シーン描画パラメータセット.
		 */
		void SetSceneParam(CSceneParam* pParam) { SAFE_REPLACE(m_pSceneParam, pParam); }

		/** 設定されているシーン描画パラメータ取得.
		 */
		CSceneParam* GetSceneParam() { return m_pSceneParam; }

	private:
		IMemoryAllocator* m_Allocator;

		CSceneParam* m_pSceneParam;

		IShader* m_pCurShader;
		IZ_UINT m_nCurShaderPassNum;
		IZ_INT m_nCurShaderPass;

		CMaterial* m_pCurMtrl;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_SCENE_RENDERER_H__)
