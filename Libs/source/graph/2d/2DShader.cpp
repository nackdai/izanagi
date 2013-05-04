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
        m_ShaderProgram = IZ_NULL;

        m_pVS = IZ_NULL;
        FILL_ZERO(m_pPS, sizeof(m_pPS));

        m_nOp = E_GRAPH_2D_RENDER_OP_MODULATE;
    }

    // デストラクタ
    C2DShader::~C2DShader()
    {
        SAFE_RELEASE(m_pVS);

        for (IZ_UINT i = 0; i < COUNTOF(m_pPS); ++i) {
            SAFE_RELEASE(m_pPS[i]);
        }

        SAFE_RELEASE(m_ShaderProgram);
    }

    // シェーダセット
    IZ_BOOL C2DShader::SetShader(CGraphicsDevice* device)
    {
        IZ_ASSERT(device != NULL);

        VRETURN(m_ShaderProgram->AttachVertexShader(m_pVS));
        VRETURN(m_ShaderProgram->AttachPixelShader(m_pPS[m_nOp]));

        VRETURN(m_ShaderProgram->Link());

        VRETURN(device->SetShaderProgram(m_ShaderProgram));

        return IZ_TRUE;
    }
}   // namespace graph
}   // namespace izanagi
