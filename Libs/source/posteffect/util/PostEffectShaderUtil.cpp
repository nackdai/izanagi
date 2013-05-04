#include "posteffect/util/PostEffectShaderUtil.h"

namespace izanagi
{
    IZ_BOOL CPostEffectShaderUtil::BeginScene(
        graph::CGraphicsDevice* pDevice,
        IZ_INT nColorBufferNum,
        graph::CRenderTarget** pColorBuffer,
        IZ_BOOL bIsResetViewport,
        IZ_INT nTarget,
        IZ_DWORD nClearColor,
        IZ_FLOAT fClearZ,
        IZ_INT nClearStencil)
    {
        IZ_BOOL ret = pDevice->BeginScene(
                        pColorBuffer,
                        nColorBufferNum,
                        //bIsResetViewport, // TODO
                        nTarget,
                        nClearColor,
                        fClearZ,
                        nClearStencil);
        return ret;
    }
}   // namespace izanagi
