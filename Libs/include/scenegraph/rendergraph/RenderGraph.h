#if !defined(__IZANAGI_SCENEGRAPH_RENDER_GRAPH_H__)
#define __IZANAGI_SCENEGRAPH_RENDER_GRAPH_H__

#include "izStd.h"
#include "izGraph.h"

namespace izanagi {
	class IMshRenderHandler;
	class IRenderElement;
	class CCamera;
	class IModel;
	class ISceneRenderer;

	/**
	 */
	class CRenderGraph : public CObject {
		// 描画順.
		enum ERenderOrder {
			ERenderOrder_Forward = 0,
			ERenderOrder_Reverse,
			ERenderOrder_Num,
			ERenderOrder_FORCE_INT32 = 0x7fffffff,
		};

	public:
		/** インスタンス作成.
		 */
		static CRenderGraph* CreateRenderGraph(
			IMemoryAllocator* allocator,
			IZ_UINT maxZLevel);

	protected:
		CRenderGraph();
		~CRenderGraph();

		NO_COPIABLE(CRenderGraph);

		IZ_DEFINE_INTERNAL_RELEASE();

	public:
		/** 登録開始.
		 */
		void BeginRegister()
		{
			Clear();
		}

		/** 登録終了.
		 */
		void EndRegister() {}

		/** 登録.
		 */
		IZ_BOOL Register(
			const CCamera& camera,
			const math::SVector& pos,
			IRenderElement* element);

		/** 登録.
		 */
		IZ_BOOL Register(
			const CCamera& camera,
			const math::SVector& pos,
			IModel* model);

		/** 描画.
		 */
		IZ_BOOL Render(
			graph::CGraphicsDevice* device,
			ISceneRenderer* sceneRenderer,
			IMshRenderHandler* renderHandler);

	private:
		// 描画内部処理
		IZ_BOOL RenderInternal(
			graph::CGraphicsDevice* device,
			ISceneRenderer* sceneRenderer,
			IMshRenderHandler* renderHandler,
			CStdSet<IRenderElement>& list,
			ERenderOrder order);
		
		void Clear();
		
	private:
		IMemoryAllocator* m_Allocator;

		IZ_UINT m_nZLevvel;

		// 描画要素リスト（不透明）
		CStdSet<IRenderElement>* m_Opaque;

		// 描画要素リスト（半透明）
		CStdSet<IRenderElement>* m_Translucent;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_RENDER_GRAPH_H__)
