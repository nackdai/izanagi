#if !defined(__IZANAGI_SCENEGRAPH_MSH_MESH_SUBSET_H__)
#define __IZANAGI_SCENEGRAPH_MSH_MESH_SUBSET_H__

#include "MSHFormat.h"

namespace izanagi {
    namespace graph {
        class CGraphicsDevice;
        class CVertexBuffer;
        class CIndexBuffer;
    }
    class IInputStream;
    class CSkeletonInstance;
    class IMshRenderHandler;

    /** プリミティブデータ.
     *
     * メッシュデータの最小単位
     */
    class CPrimitiveSet : public CObject {
        friend class CMesh;
        friend class CMeshGroup;
        friend class CMeshSet;

    private:
        CPrimitiveSet();
        ~CPrimitiveSet();

        NO_COPIABLE(CPrimitiveSet);

        void InternalRelease() { delete this; }

    public:
        /** 描画.
         */
        IZ_BOOL Render(
            graph::CGraphicsDevice* pDevice,
            CSkeletonInstance* pSkeleton,
            IMshRenderHandler* pRenderHandler);

    private:
        IZ_UINT8* Read(
            graph::CGraphicsDevice* pDevice,
            IInputStream* pIn,
            IZ_UINT8* buf);

        IZ_BOOL DebugRender(
            graph::CGraphicsDevice* pDevice,
            const CSkeletonInstance* pSkeleton,
            IMshRenderHandler* pRenderHandler);

    private:
        const S_MSH_PRIM_SET& GetInfo() const { return m_Info; }

        void SetVB(graph::CVertexBuffer* pVB) { SAFE_REPLACE(m_VB, pVB); }

    private:
        S_MSH_PRIM_SET m_Info;

        // ジョイントインデックスバッファ
        IZ_INT16* m_Joints;

        IZ_UINT m_PrimNum;

        graph::CVertexBuffer* m_VB;
        graph::CIndexBuffer* m_IB;

        struct {
            IZ_UINT m_IsTransform   : 1;
        };
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_MSH_MESH_SUBSET_H__)
