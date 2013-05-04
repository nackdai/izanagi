#include "posteffect/vs/PostEffectVS.h"
#include "posteffect/vs/PostEffectVSSampling.h"
#include "posteffect/vs/PostEffectVSManager.h"

using namespace izanagi;

///////////////////////////////////////////////////
// 頂点シェーダ基本

// コンストラクタ
CPostEffectVS::CPostEffectVS()
{
    m_Allocator = IZ_NULL;

    m_pShader = IZ_NULL;
    m_pVB = IZ_NULL;
    m_pVtxDecl = IZ_NULL;

    FILL_ZERO(m_hCommonHandle, sizeof(m_hCommonHandle));

    math::SVector::SetZero(m_vecPosOffset);
    math::SVector::SetZero(m_vecTexParam);
}

// デストラクタ
CPostEffectVS::~CPostEffectVS()
{
    SAFE_RELEASE(m_pShader);
    SAFE_RELEASE(m_pVB);
    SAFE_RELEASE(m_pVtxDecl);
}

// シェーダ作成
IZ_BOOL CPostEffectVS::CreateShader(
    graph::CGraphicsDevice* device,
    IZ_UINT8* pProgram)
{
    IZ_BOOL result = IZ_FALSE;

    // 頂点シェーダ作成
    m_pShader = device->CreateVertexShader(pProgram);
    result = (m_pShader != IZ_NULL);

    return result;
}

// シェーダパラメータ初期化済みかどうか
IZ_BOOL CPostEffectVS::IsInitilizedShaderParameter()
{
    return (m_hCommonHandle[0] != 0);
}

// シェーダパラメータ初期化
void CPostEffectVS::InitShaderParameter(
    graph::CGraphicsDevice* device,
    graph::CShaderProgram* program)
{
    // 共通シェーダ定数ハンドルを取得
    static IZ_PCSTR name[COMMON_HANDLE_NUM] = {
        "g_vPosOffset",
        "g_vTexParam",
    };

    for (IZ_UINT i = 0; i < COUNTOF(name); i++) {
        m_hCommonHandle[i] = program->GetHandleByName(name[i]);
    }
}

// 描画
void CPostEffectVS::PrepareRender(
    graph::CGraphicsDevice* device,
    graph::CShaderProgram* program,
    IZ_FLOAT fPosOffsetX,
    IZ_FLOAT fPosOffsetY,
    const SFloatRect* pTexCoord)
{
    IZ_ASSERT(m_pVB != IZ_NULL);
    IZ_ASSERT(m_pVtxDecl != IZ_NULL);
    IZ_ASSERT(m_pShader != IZ_NULL);
    IZ_ASSERT(device != IZ_NULL);

    // 共通パラメータセット
    ApplyShaderParameter(
        device,
        program,
        fPosOffsetX,
        fPosOffsetY,
        pTexCoord);

    // パラメータセット
    ApplyShaderParameter(device, program);

    // 頂点宣言
    device->SetVertexDeclaration(m_pVtxDecl);

    // 頂点バッファ
    device->SetVertexBuffer(
        0, 
        0,
        sizeof(CPostEffectVSManager::CUSTOM_FVF),
        m_pVB);
}

// パラメータセット
void CPostEffectVS::RegisterParameter(
    const math::SVector* pVector,
    IZ_UINT num)
{
    // 何もしない
    UNUSED_ALWAYS(pVector);
    UNUSED_ALWAYS(num);
}

// 頂点シェーダ本体を取得
graph::CVertexShader* CPostEffectVS::GetVertexShader()
{
    return m_pShader;
}

// 共通パラメータセット
void CPostEffectVS::ApplyShaderParameter(
    graph::CGraphicsDevice* device,
    graph::CShaderProgram* program,
    IZ_FLOAT fPosOffsetX,
    IZ_FLOAT fPosOffsetY,
    const SFloatRect* pTexCoord)
{
    // 位置オフセット
    {
        graph::CRenderTarget* rt = device->GetRenderTarget(0);
        IZ_ASSERT(rt != IZ_NULL);

        IZ_FLOAT fWidth = (IZ_FLOAT)rt->GetWidth();
        IZ_FLOAT fHeight = (IZ_FLOAT)rt->GetHeight();

        // テクセル中心となるようなオフセット位置
        // ポリゴンの描画位置を-0.5fずらすことで、テクセル位置が0.5fずれるようにするらしい
        IZ_FLOAT fOffsetX = -0.5f / fWidth;
        IZ_FLOAT fOffsetY = -0.5f / fHeight;

        fOffsetX += fPosOffsetX / fWidth;
        fOffsetY += fPosOffsetY / fHeight;

        m_vecPosOffset.Set(
            fOffsetX, fOffsetY,
            0.0f, 0.0f);

        program->SetVector(
            device,
            m_hCommonHandle[COMMON_HANDLE_PosOffset],
            m_vecPosOffset);
    }

    // テクスチャ座標
    {
        IZ_FLOAT fTexScaleX = 1.0f;
        IZ_FLOAT fTexScaleY  = 1.0f;
        IZ_FLOAT fTexOffsetX = 0.0f;
        IZ_FLOAT fTexOffsetY = 0.0f;

        if (pTexCoord != IZ_NULL) {
            // テクスチャ座標(UV)を計算するためのパラメータ
            // 全て 0.0f〜1.0f の間にクランプ済み・・・のはず
            fTexScaleX = pTexCoord->right - pTexCoord->left;
            fTexScaleY = pTexCoord->bottom - pTexCoord->top;
            fTexOffsetX = pTexCoord->left;
            fTexOffsetY = pTexCoord->top;
        }

        m_vecTexParam.Set(
            fTexScaleX, fTexScaleY,
            fTexOffsetX, fTexOffsetY);

        program->SetVector(
            device,
            m_hCommonHandle[COMMON_HANDLE_TexParam],
            m_vecTexParam);
    }
}

// パラメータセット
void CPostEffectVS::ApplyShaderParameter(
    graph::CGraphicsDevice* device,
    graph::CShaderProgram* program)
{
    // 何もしない
}
