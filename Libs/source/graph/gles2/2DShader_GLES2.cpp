#include "graph/gles2/2DShader_GLES2.h"
#include "graph/VertexShader.h"
#include "graph/PixelShader.h"
#include "graph/ShaderProgram.h"
#include "graph/GraphicsDevice.h"

#include "2DVS.vs"
#include "2DPS_OpAdd.ps"
#include "2DPS_OpModulate.ps"
#include "2DPS_OpModulate2x.ps"
#include "2DPS_OpModulate4x.ps"
#include "2DPS_OpModulateAlpha.ps"
#include "2DPS_OpNoTexAlpha.ps"
#include "2DPS_OpTex.ps"
#include "2DPS_OpVtx.ps"
#include "2DPS_OpTexAsDepth.ps"

static const char* PS_Programs[] = {
    main_OpVtx,
    main_OpTex,
    main_OpModulate,
    main_OpModulate2x,
    main_OpModulate4x,
    main_OpAdd,
    main_OpModulateAlpha,
    main_OpNoTexAlpha,
    main_OpTexAsDepth,
    IZ_NULL,
};

IZ_C_ASSERT(COUNTOF(PS_Programs) == izanagi::graph::E_GRAPH_2D_RENDER_OP_NUM);

namespace izanagi
{
namespace graph
{
    // インスタンス作成
    C2DShader* C2DShader::Create2DShader(
        IMemoryAllocator* allocator,
        CGraphicsDevice* device)
    {
        IZ_ASSERT(allocator != IZ_NULL);
        IZ_ASSERT(device != IZ_NULL);

        C2DShader* instance = IZ_NULL;

        // メモリ確保
        IZ_BYTE* buf = (IZ_BYTE*)ALLOC_ZERO(allocator, sizeof(C2DShaderGLES2));
        IZ_BOOL result = (buf != IZ_NULL);
        if (!result) {
            IZ_ASSERT(IZ_FALSE);
            goto __EXIT__;
        }

        // インスタンス作成
        instance = new (buf) C2DShaderGLES2();
        {
            instance->AddRef();
            instance->m_Allocator = allocator;
        
            // シェーダ作成
            result = instance->CreateShader(device);
            if (!result) {
                IZ_ASSERT(IZ_FALSE);
                goto __EXIT__;
            }
        }

    __EXIT__:
        if (!result) {
            if (instance != IZ_NULL) {
                SAFE_RELEASE(instance);
            }
            else if (buf != IZ_NULL) {
                allocator->Free(buf);
            }
        }
        return instance;
    }

    // コンストラクタ
    C2DShaderGLES2::C2DShaderGLES2()
    {
        FILL_ZERO(m_hVtxParam, sizeof(m_hVtxParam));
    }

    // デストラクタ
    C2DShaderGLES2::~C2DShaderGLES2()
    {
    }

    // シェーダ作成
    IZ_BOOL C2DShaderGLES2::CreateShader(CGraphicsDevice* device)
    {
        IZ_ASSERT(device != NULL);

        // 頂点シェーダ
        m_pVS = device->CreateVertexShader(mainVS);
        VRETURN(m_pVS != IZ_NULL);

        for (IZ_UINT i = 0; i < E_GRAPH_2D_RENDER_OP_NUM; ++i) {
            // ピクセルシェーダ
            if (PS_Programs[i] != IZ_NULL) {
                m_pPS[i] = device->CreatePixelShader(PS_Programs[i]);
                VRETURN(m_pPS[i] != IZ_NULL);
            }

            // シェーダプログラム
            m_ShaderProgram[i] = device->CreateShaderProgram();
            VRETURN(m_ShaderProgram[i] != IZ_NULL);

            m_ShaderProgram[i]->AttachVertexShader(m_pVS);

            if (m_pPS[i] != IZ_NULL) {
                m_ShaderProgram[i]->AttachPixelShader(m_pPS[i]);
            }
        }

        return IZ_TRUE;
    }

    // シェーダパラメータセット
    IZ_BOOL C2DShaderGLES2::SetShaderParams(CGraphicsDevice* device)
    {
        IZ_ASSERT(device != NULL);

        CShaderProgram* program = GetShaderProgram();
        E_GRAPH_2D_RENDER_OP op = GetRenderOp();

        if (m_hVtxParam[op][VTX_PARAM_NUM] == 0) {
            m_hVtxParam[op][VTX_PARAM_NUM] = program->GetHandleByName("g_vInvScreen");
        }

        // スクリーンサイズの逆数
        {
            math::SVector4 vTmp;
            vTmp.x = 1.0f / device->GetViewport().width;
            vTmp.y = 1.0f / device->GetViewport().height;

            VRETURN(
                program->SetVector(
                    device,
                    m_hVtxParam[op][VTX_PARAM_NUM],
                    vTmp));
        }

        return IZ_TRUE;
    }
}   // namespace graph
}   // namespace izanagi
