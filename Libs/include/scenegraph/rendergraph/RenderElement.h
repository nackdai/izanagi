#if !defined(__IZANAGI_SCENEGRAPH_RENDER_ELEMENT_H__)
#define __IZANAGI_SCENEGRAPH_RENDER_ELEMENT_H__

#include "izStd.h"
#include "scenegraph/mtrl/izMaterial.h"

namespace izanagi {
    namespace graph {
        class CGraphicsDevice;
    }

    /**
     */
    class IRenderElement : public CObject {
        friend class CRenderGraph;
        friend class CStdSet<IRenderElement>;

    protected:
        IRenderElement() : m_Z(0.0f), m_IsVisible(IZ_TRUE) { m_Item.Init(this); }
        virtual ~IRenderElement() {}

        NO_COPIABLE(IRenderElement);

    public:
        /** 描画.
         */
        virtual IZ_BOOL Render(
            graph::CGraphicsDevice* device,
            void* additional) = 0;

        /** 半透明かどうかを取得.
         */
        virtual IZ_BOOL IsTranslucent() const = 0;

        /** 設定されているマテリアルを取得.
         */
        virtual CMaterial* GetMaterial() { return IZ_NULL; }

    public:
        /**
         */
        void SetZ(IZ_FLOAT z) { m_Z = z; }

        /**
         */
        IZ_FLOAT GetZ() const { return m_Z; }

        /**
         */
        void SetIsVisible(IZ_BOOL visible) { m_IsVisible = visible; }

        /**
         */
        IZ_BOOL IsVisible() const { return m_IsVisible; }

    protected:
        CStdSet<IRenderElement>::Item* GetListItem() { return &m_Item; }

        IZ_BOOL operator>(IRenderElement& rhs)
        {
            return m_Z > rhs.m_Z;
        }

    private:
        IZ_FLOAT m_Z;
        CStdSet<IRenderElement>::Item m_Item;

        IZ_BOOL m_IsVisible;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_RENDER_ELEMENT_H__)
