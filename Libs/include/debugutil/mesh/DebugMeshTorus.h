#if !defined(__IZANAGI_DEBUG_UTIL_MESH_TORUS_H__)
#define __IZANAGI_DEBUG_UTIL_MESH_TORUS_H__

#include "DebugMesh.h"

namespace izanagi {
    /**
    * Torus
    */
    class CDebugMeshTorus : public CDebugMeshTmpl<SMeshVtx> {
        friend class CDebugMeshTmpl<SMeshVtx>;

    public:
        // インスタンス作成
        static CDebugMeshTorus* CreateDebugMeshTorus(
            IMemoryAllocator* pAllocator,
            graph::CGraphicsDevice* pDevice,
            IZ_UINT flag,
            IZ_COLOR nColor,
            IZ_FLOAT fInnerRadius,  // 内径
            IZ_FLOAT fOuterRadius,  // 外径
            IZ_UINT nSides,         // 横断面の辺の数
            IZ_UINT nRings);        // トーラスに含まれるリングの数

    protected:
        CDebugMeshTorus() {}
        ~CDebugMeshTorus() {}

        NO_COPIABLE(CDebugMeshTorus);

    protected:
        // 初期化
        IZ_BOOL Init(
            graph::CGraphicsDevice* pDevice,
            IZ_UINT flag,
            IZ_UINT nSides, 
            IZ_UINT nRings);

        // 頂点データセット
        IZ_BOOL SetVtx(
            IZ_UINT flag,
            IZ_COLOR nColor,
            IZ_FLOAT fInnerRadius,  // 内径
            IZ_FLOAT fOuterRadius,  // 外径
            IZ_UINT nSides,         // 横断面の辺の数
            IZ_UINT nRings);        // トーラスに含まれるリングの数

        void ComputeVtx(
            SMeshVtx* pVtx,
            IZ_UINT flag,
            IZ_COLOR nColor,
            IZ_FLOAT fCenterRadius, // リング中心までの径
            IZ_FLOAT fRingRadius,   // リングの半径
            IZ_FLOAT fLongitude,    // 経度
            IZ_FLOAT fLatitude);    // 緯度

        // インデックスデータセット
        IZ_BOOL SetIdx(
            IZ_UINT nSides,         // 横断面の辺の数
            IZ_UINT nRings);        // トーラスに含まれるリングの数
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_DEBUG_UTIL_MESH_TORUS_H__)
