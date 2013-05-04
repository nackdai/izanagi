#include "posteffect/PostEffectShaderPass.h"
#include "posteffect/vs/PostEffectVSManager.h"

namespace izanagi
{
    // コンストラクタ
    CPostEffectPass::CPostEffectPass()
    {
        m_nIdx = 0;
        m_pDesc = IZ_NULL;
        m_Shader = IZ_NULL;
        m_pVS = IZ_NULL;
    }

    // デストラクタ
    CPostEffectPass::~CPostEffectPass()
    {
        Clear();
    }

    // 描画
    void CPostEffectPass::Render(
        graph::CGraphicsDevice* device,
        const SFloatRect& rcTexCoord)
    {
        m_pVS->PrepareRender(
            device,
            m_Shader,
            0.0f, 0.0f,
            &rcTexCoord);

        device->SetShaderProgram(m_Shader);

        // NOTE
        // 頂点数を指定する
        device->DrawPrimitive(
            graph::E_GRAPH_PRIM_TYPE_TRIANGLESTRIP, 
            0, 
            CPostEffectVSManager::PRIM_NUM);
    }

    // 初期化
    void CPostEffectPass::Init(
        IZ_UINT idx, 
        const S_PES_PASS* pDesc)
    {
        m_nIdx = idx;
        m_pDesc = pDesc;

        m_Shader = IZ_NULL;
        m_pVS = IZ_NULL;
    }

    // 初期化
    void CPostEffectPass::InitParam(
        IZ_UINT idx,
        IZ_UINT nParamIdx,
        IZ_PCSTR name)
    {
        IZ_BOOL result = InitInfo<SParamInfo>(m_Params, idx, nParamIdx, name);
    }

    // 初期化
    IZ_BOOL CPostEffectPass::InitSampler(
        IZ_UINT idx,
        IZ_UINT nSmpleIdx,
        IZ_PCSTR name)
    {
        IZ_BOOL ret = InitInfo<SSamplerInfo>(m_Samplers, idx, nSmpleIdx, name);
        
        if (ret) {
            IZ_ASSERT(m_Shader->IsValid());
            m_Samplers.list[idx].resource_id = (IZ_UINT16)m_Shader->GetSamplerIndex(m_Samplers.list[idx].handle);

            // NOTE
            // サンプラレジスタ 0 - 7 の８個まで
            ret = (m_Samplers.list[idx].resource_id < 8);
            IZ_ASSERT(ret);
        }

        return ret;
    }

    void CPostEffectPass::Clear()
    {
        SAFE_RELEASE(m_Shader);
        SAFE_RELEASE(m_pVS);
    }

    // シェーダプログラムのセット
    void CPostEffectPass::SetShaderProgram(graph::CShaderProgram* program)
    {
        SAFE_REPLACE(m_Shader, program);

        if (m_pVS != IZ_NULL) {
            m_Shader->AttachVertexShader(m_pVS->GetVertexShader());
        }
    }

    // シェーダプログラムの取得
    graph::CShaderProgram* CPostEffectPass::GetShaderProgram()
    {
        return m_Shader;
    }

    // 頂点シェーダのセット
    void CPostEffectPass::SetPostEffectVS(CPostEffectVS* pVS)
    {
        SAFE_REPLACE(m_pVS, pVS);

        if (m_pVS != IZ_NULL) {
            m_Shader->AttachVertexShader(m_pVS->GetVertexShader());
        }
    }

    // 頂点シェーダの取得
    CPostEffectVS* CPostEffectPass::GetPostEffectVS()
    {
        return m_pVS;
    }
}   // namespace izanagi
