#if !defined(__IZANAGI_POSTEFFECT_UTIL_H__)
#define __IZANAGI_POSTEFFECT_UTIL_H__

#include "izGraph.h"
#include "PostEffectUtilityParam.h"

namespace izanagi {
    class CPostEffectShaderUtil {
    private:
        CPostEffectShaderUtil();
        ~CPostEffectShaderUtil();

        CPostEffectShaderUtil(const CPostEffectShaderUtil& rhs);
        const CPostEffectShaderUtil& operator=(const CPostEffectShaderUtil& rhs);

    public:
        // パラメータをセットする
        template <class _T>
        static IZ_BOOL SetValue(
            graph::CGraphicsDevice* device,
            _T* pShader,
            SHADER_PARAM_HANDLE handle,
            const void* pValue,
            E_POSTEFFECT_PARAMETER_TYPE type,
            IZ_UINT nElements);

    public:
        static inline IZ_BOOL BeginScene(
            graph::CGraphicsDevice* pDevice,
            IZ_INT nColorBufferNum,
            graph::CRenderTarget** pColorBuffer,
            IZ_BOOL bIsResetViewport,
            IZ_INT nTarget,
            IZ_DWORD nClearColor,
            IZ_FLOAT fClearZ,
            IZ_INT nClearStencil);
    };

    // パラメータをセットする
    template <class _T>
    IZ_BOOL CPostEffectShaderUtil::SetValue(
        graph::CGraphicsDevice* device,
        _T* pShader,
        SHADER_PARAM_HANDLE handle,
        const void* pValue,
        E_POSTEFFECT_PARAMETER_TYPE type,
        IZ_UINT nElements)
    {
        IZ_BOOL ret = IZ_TRUE;

        IZ_UINT nRow = CPostEffectParamUtil::GetParamRowFromParamType(type);
        IZ_UINT nColumn = CPostEffectParamUtil::GetParamColumnFromParamType(type);

        if (CPostEffectParamUtil::IsBoolType(type)) {
            // BOOL
            pShader->SetBoolArray(
                device,
                handle,
                (const IZ_BOOL*)pValue,
                nRow * nColumn * nElements);
        }
        else if (CPostEffectParamUtil::IsIntType(type)) {
            // INT
            pShader->SetIntArray(
                device,
                handle,
                (const IZ_INT*)pValue,
                nRow * nColumn * nElements);
        }
        else if (CPostEffectParamUtil::IsMatrixType(type)) {
            // FLOAT4x4 -> MATRIX
            pShader->SetMatrixArray(
                device,
                handle,
                (const math::SMatrix*)pValue,
                nElements);
        }
        else if (CPostEffectParamUtil::IsFloatType(type)) {
            if (nColumn < 4) {
                // FLOATn x M
                pShader->SetFloatArray(
                    device,
                    handle,
                    (const IZ_FLOAT*)pValue,
                    nRow * nColumn * nElements);
            }
            else {
                // FLOAT4xN -> VECTOR4 x N
                pShader->SetVectorArray(
                    device,
                    handle,
                    (const math::SVector*)pValue,
                    nRow * nElements);
            }
        }
        else {
            ret = IZ_FALSE;
        }

        IZ_ASSERT(ret);
        return ret;
    }
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_POSTEFFECT_UTIL_H__)
