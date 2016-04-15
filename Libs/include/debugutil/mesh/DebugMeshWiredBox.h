#if !defined(__IZANAGI_DEBUG_UTIL_MESH_WIRED_BOX_H__)
#define __IZANAGI_DEBUG_UTIL_MESH_WIRED_BOX_H__

#include "DebugMesh.h"

namespace izanagi {
    class CDebugMeshWiredBox : public CDebugMesh {
        friend class CDebugMeshTmpl<SMeshVtx>;

    public:
        // インスタンス作成
        static CDebugMeshWiredBox* create(
            IMemoryAllocator* allocator,
            graph::CGraphicsDevice* device,
            IZ_COLOR color,
            IZ_FLOAT width,
            IZ_FLOAT height,
            IZ_FLOAT depth);

    protected:
        CDebugMeshWiredBox() {}
        ~CDebugMeshWiredBox() {}

        NO_COPIABLE(CDebugMeshWiredBox);

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
            IZ_FLOAT width,
            IZ_FLOAT height,
            IZ_FLOAT depth);
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_DEBUG_UTIL_MESH_WIRED_BOX_H__)
