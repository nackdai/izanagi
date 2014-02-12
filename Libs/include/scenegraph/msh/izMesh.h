#if !defined(__IZANAGI_SCENEGRAPH_IZ_MESH_H__)
#define __IZANAGI_SCENEGRAPH_IZ_MESH_H__

#include "MSHFormat.h"

namespace izanagi {
    namespace graph {
        class CGraphicsDevice;
        class CVertexBuffer;
    }
    class IInputStream;
    class CMeshGroup;
    class CSkeletonInstance;
    class IMshRenderHandler;

    /** メッシュデータ.
     */
    class CMesh : public CObject {
        friend class CModel;

    public:
        static CMesh* CreateMesh(
            IMemoryAllocator* pAllocator,
            graph::CGraphicsDevice* pDevice,
            IInputStream* pIn);

    private:
        CMesh();
        ~CMesh();

        NO_COPIABLE(CMesh);

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        /** メッシュを描画.
         */
        IZ_BOOL Render(
            graph::CGraphicsDevice* device,
            CSkeletonInstance* pSkl,
            IMshRenderHandler* pRenderHandler);

    public:
        /** メッシュグループ（= メッシュセットの集まり）を取得.
         */
        CMeshGroup* GetMeshGroup(IZ_UINT idx);

        const S_MSH_HEADER& GetHeader() const { return m_Header; }

        /** メッシュグループ総数を取得.
         */
        IZ_UINT GetMeshGroupNum() const { return m_Header.numMeshGroup; }

    private:
        IMemoryAllocator* m_Allocator;

        S_MSH_HEADER m_Header;

        CMeshGroup** m_pMeshGroup;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_IZ_MESH_H__)
