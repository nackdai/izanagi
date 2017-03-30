#if !defined(__IZANAGI_DEBUG_UTIL_MESH_LINE_H__)
#define __IZANAGI_DEBUG_UTIL_MESH_LINE_H__

#include "DebugMesh.h"

namespace izanagi {
    class CDebugMeshLine : public CDebugMesh {
        friend class CDebugMeshTmpl<SMeshVtx>;

    public:
        // インスタンス作成
        static CDebugMeshLine* create(
            IMemoryAllocator* allocator,
            graph::CGraphicsDevice* device,
            IZ_COLOR color,
            const math::CVector3& p0,
            const math::CVector3& p1);

    protected:
        CDebugMeshLine() {}
        ~CDebugMeshLine() {}

        NO_COPIABLE(CDebugMeshLine);

    protected:
        // 初期化
        IZ_BOOL Init(
            graph::CGraphicsDevice* device,
            IZ_UINT flag);

        // 頂点データセット
        IZ_BOOL SetVtx(
            graph::CGraphicsDevice* device,
            IZ_UINT flag,
            IZ_COLOR color,
            const math::CVector3& p0,
            const math::CVector3& p1);
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_DEBUG_UTIL_MESH_WIRED_BOX_H__)
