#if !defined(__IZANAGI_SHADER_SHADER_PASS_H__)
#define __IZANAGI_SHADER_SHADER_PASS_H__

#include "izGraph.h"
#include "SHDFormat.h"
#include "util/ShaderUtil.h"

namespace izanagi {
    class CShaderSamplerTable;

    /**
    * シェーダのパス
    */
    class CShaderPass {
        friend class CShaderBasic;

        // パラメータホルダ
        struct SParamInfo {
            IZ_UINT idx;                // パラメータインデックス（そのパスの中で何番目のパラメータか）
            SHADER_PARAM_HANDLE handle; // パラメータハンドル（頂点シェーダ）
        };

        // サンプラホルダ
        struct SSamplerInfo {
            IZ_UINT16 idx;              // パラメータインデックス（そのパスの中で何番目のサンプラか）
            IZ_UINT16 resource_id;      // リソースID (0〜7のいずれか)
            SHADER_PARAM_HANDLE handle; // パラメータハンドル
        };

        template <typename _T>
        struct SParams {
            IZ_UINT num;
            _T* list;

            SParams()
            {
                num = 0;
                list = IZ_NULL;
            }
        };

        template <typename _T>
        IZ_UINT8* SetInfoBuffer(
            SParams<_T>& sParams,
            IZ_UINT num,
            IZ_UINT8* buf)
        {
            if (num > 0) {
                sParams.num = num;
                sParams.list = reinterpret_cast<_T*>(buf);
                return (buf + sizeof(_T) * num);
            }
            return buf;
        }

        template <typename _T>
        IZ_BOOL InitInfo(
            SParams<_T>& sParams,
            IZ_UINT idx,
            IZ_UINT nIdxInParam,
            IZ_PCSTR name)
        {
            IZ_ASSERT(idx < sParams.num);

            IZ_BOOL ret = IZ_FALSE;

            sParams.list[idx].idx = nIdxInParam;
            sParams.list[idx].handle = m_Program->GetHandleByName(name);
            ret = (sParams.list[idx].handle != 0);

            return ret;
        }

        template <typename _T>
        const _T* GetInfo(
            const SParams<_T>& sParams,
            IZ_UINT idx) const
        {
            IZ_ASSERT(idx < sParams.num);
            return &sParams.list[idx];
        }

    private:
        static size_t ComputeBufferSize(
            IZ_UINT nParamNum,
            IZ_UINT nSmplNum)
        {
            size_t ret = sizeof(SParamInfo) * nParamNum;
            ret += sizeof(SSamplerInfo) * nSmplNum;
            return ret;
        }

    private:
        CShaderPass();
        ~CShaderPass();

        NO_COPIABLE(CShaderPass);

    private:
        // バッファ割り当て
        IZ_UINT8* SetParamBuffer(IZ_UINT num, IZ_UINT8* p)
        {
            return SetInfoBuffer<SParamInfo>(m_Params, num, p);
        }
        IZ_UINT8* SetSamplerBuffer(IZ_UINT num, IZ_UINT8* p)
        {
            return SetInfoBuffer<SSamplerInfo>(m_Samplers, num, p);
        }

        // 初期化
        void Init(
            IZ_UINT idx, 
            const S_SHD_PASS* pDesc);

        // 初期化
        void InitParam(
            IZ_UINT idx,
            IZ_UINT nParamIdx,
            IZ_PCSTR name);

        // 初期化
        IZ_BOOL InitSampler(
            const CShaderSamplerTable& samplerTbl,
            graph::CGraphicsDevice* device,
            IZ_UINT idx,
            IZ_UINT nSmplIdx,
            IZ_PCSTR name);

        void Clear();

        // パラメータ数取得
        IZ_UINT GetParamNum() const { return m_Params.num; }

        // サンプラ数取得
        IZ_UINT GetSamplerNum() const { return m_Samplers.num; }

        // ホルダ取得
        const SParamInfo* GetParamInfo(IZ_UINT idx) const { return GetInfo(m_Params, idx); }
        const SSamplerInfo* GetSamplerInfo(IZ_UINT idx) const { return GetInfo(m_Samplers, idx); }

        void SetShaderProgram(graph::CShaderProgram* program)
        {
            SAFE_REPLACE(m_Program, program);
        }

        graph::CShaderProgram* GetShaderProgram()
        {
            return m_Program;
        }

        IZ_UINT GetIdx() const { return m_nIdx; }

        void SetDesc(const S_SHD_PASS* pDesc) { m_pDesc = pDesc; }
        const S_SHD_PASS* GetDesc() const { return m_pDesc; }

    private:
        IZ_UINT m_nIdx;

        const S_SHD_PASS* m_pDesc;

        SParams<SParamInfo> m_Params;
        SParams<SSamplerInfo> m_Samplers;

        graph::CShaderProgram* m_Program;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SHADER_SHADER_PASS_H__)
