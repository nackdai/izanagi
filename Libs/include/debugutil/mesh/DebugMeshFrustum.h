#if !defined(__IZANAGI_DEBUG_UTIL_MESH_FRUSTUM_H__)
#define __IZANAGI_DEBUG_UTIL_MESH_FRUSTUM_H__

#include "izDefs.h"
#include "DebugMesh.h"

namespace izanagi {
    /**
    */
    class CDebugMeshFrustum : public CDebugMesh {
        friend class CDebugMesh;

    public:
        static CDebugMeshFrustum* CreateDebugMeshFrustum(
            IMemoryAllocator* pAllocator,
            graph::CGraphicsDevice* pDevice,
            IZ_COLOR nColor,
            IZ_FLOAT fAspect,
            IZ_FLOAT fFov,
            IZ_FLOAT fNear, IZ_FLOAT fFar);

    protected:
        CDebugMeshFrustum() {}
        ~CDebugMeshFrustum() {}

        NO_COPIABLE(CDebugMeshFrustum);

    protected:
        enum {
            VTX_NUM = 3 * 4 + 3 * 2,

            VTX_FORM_FLAG = E_DEBUG_MESH_VTX_FORM_POS | E_DEBUG_MESH_VTX_FORM_COLOR,
        };

    protected:
        IZ_BOOL Init();

        IZ_BOOL SetVtx(
            IZ_COLOR nColor,
            IZ_FLOAT fAspect,
            IZ_FLOAT fFov,
            IZ_FLOAT fNear, IZ_FLOAT fFar);

    public:
        // 描画
        //virtual IZ_BOOL Draw(IZ_BOOL bEnableDrawDebugAxis = IZ_FALSE);
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_DEBUG_UTIL_MESH_FRUSTUM_H__)
