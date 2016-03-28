#if !defined(__IZANAGI_DEBUG_UTIL_MESH_SCREEN_FILL_PLANE_H__)
#define __IZANAGI_DEBUG_UTIL_MESH_SCREEN_FILL_PLANE_H__

#include "DebugMesh.h"

namespace izanagi {
    /**
     * スクリーン全面描画平面.
     */
    class CDebugMeshScreenFillPlane : public CDebugMesh {
        friend class CDebugMeshTmpl<SMeshVtx>;

    public:
        // インスタンス作成
        static CDebugMeshScreenFillPlane* create(
            IMemoryAllocator* allocator,
            graph::CGraphicsDevice* device,
            IZ_COLOR color);

    protected:
        CDebugMeshScreenFillPlane() {}
        ~CDebugMeshScreenFillPlane() {}

        NO_COPIABLE(CDebugMeshScreenFillPlane);

    protected:
        // 初期化
        IZ_BOOL Init(
            graph::CGraphicsDevice* device,
            IZ_UINT flag);

        // 頂点データセット
        IZ_BOOL SetVtx(
            graph::CGraphicsDevice* device,
            IZ_UINT flag,
            IZ_COLOR color);

        IZ_BOOL SetIdx(graph::CGraphicsDevice* device);
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_DEBUG_UTIL_MESH_SCREEN_FILL_PLANE_H__)
