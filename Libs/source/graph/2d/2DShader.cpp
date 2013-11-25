#include "graph/2d/2DShader.h"
#include "graph/VertexShader.h"
#include "graph/PixelShader.h"
#include "graph/ShaderProgram.h"
#include "graph/GraphicsDevice.h"

namespace izanagi
{
namespace graph
{
    // コンストラクタ
    C2DShader::C2DShader()
    {
        FILL_ZERO(m_ShaderProgram, sizeof(m_ShaderProgram));;

        m_pVS = IZ_NULL;
        m_UserDefsVS = IZ_NULL;

        FILL_ZERO(m_pPS, sizeof(m_pPS));

        m_nOp = E_GRAPH_2D_RENDER_OP_MODULATE;
    }

    // デストラクタ
    C2DShader::~C2DShader()
    {
        SAFE_RELEASE(m_pVS);
        SAFE_RELEASE(m_UserDefsVS);

        for (IZ_UINT i = 0; i < COUNTOF(m_pPS); ++i) {
            SAFE_RELEASE(m_ShaderProgram[i]);
            SAFE_RELEASE(m_pPS[i]);
        }
    }

    // シェーダセット
    IZ_BOOL C2DShader::SetShader(CGraphicsDevice* device)
    {
        IZ_ASSERT(device != IZ_NULL);
        IZ_ASSERT(m_pPS[m_nOp] != IZ_NULL);

        if (m_nOp == E_GRAPH_2D_RENDER_OP_USER_DEFS
            && m_UserDefsVS != IZ_NULL)
        {
            VRETURN(m_ShaderProgram[m_nOp]->AttachVertexShader(m_UserDefsVS));
        }
        else {
            VRETURN(m_ShaderProgram[m_nOp]->AttachVertexShader(m_pVS));
        }

        VRETURN(m_ShaderProgram[m_nOp]->AttachPixelShader(m_pPS[m_nOp]));

        VRETURN(device->SetShaderProgram(m_ShaderProgram[m_nOp]));

        return IZ_TRUE;
    }

    // ユーザー定義のシェーダをセット
    void C2DShader::SetUserDefsShader(
        CVertexShader* vs,
        CPixelShader* ps)
    {
        SAFE_REPLACE(m_UserDefsVS, vs);
        SAFE_REPLACE(m_pPS[E_GRAPH_2D_RENDER_OP_USER_DEFS], ps);

        if (m_UserDefsVS != IZ_NULL)
        {
            m_ShaderProgram[E_GRAPH_2D_RENDER_OP_USER_DEFS]->AttachVertexShader(m_UserDefsVS);
        }
        else {
            m_ShaderProgram[E_GRAPH_2D_RENDER_OP_USER_DEFS]->AttachVertexShader(m_pVS);
        }

        m_ShaderProgram[E_GRAPH_2D_RENDER_OP_USER_DEFS]->AttachPixelShader(m_pPS[E_GRAPH_2D_RENDER_OP_USER_DEFS]);
    }
}   // namespace graph
}   // namespace izanagi
