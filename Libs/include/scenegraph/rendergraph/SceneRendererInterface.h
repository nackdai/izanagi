#if !defined(__IZANAGI_SCENEGRAPH_SCENE_RENDERER_INTERFACE_H__)
#define __IZANAGI_SCENEGRAPH_SCENE_RENDERER_INTERFACE_H__

#include "izStd.h"
#include "izGraph.h"

namespace izanagi {
    namespace shader {
        class IShader;
    }

    class IRenderElement;
    class IMshRenderHandler;

    /**
     */
    class ISceneRenderer : public CObject {
        friend class CGeometrySorter;

    protected:
        ISceneRenderer() {}
        virtual ~ISceneRenderer() {}

        NO_COPIABLE(ISceneRenderer);

    public:
        /** 描画開始.
         */
        PURE_VIRTUAL(
            IZ_UINT BeginRender(
                izanagi::graph::CGraphicsDevice* device,
                shader::IShader* shader,
                IZ_UINT techIdx));

        /** 描画途中.
         */
        PURE_VIRTUAL(
            IZ_BOOL IterRender(
                graph::CGraphicsDevice* device,
                IZ_UINT passIdx,
                IRenderElement* element,
                IMshRenderHandler* renderHandler));

        /** 描画終了.
         */
        PURE_VIRTUAL(IZ_BOOL EndRender(izanagi::graph::CGraphicsDevice* device));
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_SCENE_RENDERER_INTERFACE_H__)
