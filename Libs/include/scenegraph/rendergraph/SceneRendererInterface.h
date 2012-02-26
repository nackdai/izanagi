#if !defined(__IZANAGI_SCENEGRAPH_SCENE_RENDERER_INTERFACE_H__)
#define __IZANAGI_SCENEGRAPH_SCENE_RENDERER_INTERFACE_H__

#include "izStd.h"

namespace izanagi {
	class CGraphicsDevice;
	class IRenderElement;
	class IMshRenderHandler;
	class IShader;

	/**
	 */
	class ISceneRenderer : public CObject {
		friend class CGeometrySorter;

	protected:
		ISceneRenderer() {}
		virtual ~ISceneRenderer() = 0 {}

		NO_COPIABLE(ISceneRenderer);

	public:
		/** 描画開始.
		 */
		PURE_VIRTUAL(
			IZ_UINT BeginRender(
				IShader* shader,
				IZ_UINT techIdx));

		/** 描画途中.
		 */
		PURE_VIRTUAL(
			IZ_BOOL IterRender(
				CGraphicsDevice* device,
				IZ_UINT passIdx,
				IRenderElement* element,
				IMshRenderHandler* renderHandler));

		/** 描画終了.
		 */
		PURE_VIRTUAL(IZ_BOOL EndRender());
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_SCENE_RENDERER_INTERFACE_H__)
