#include "graph/dx9/2DShader_DX9.h"
#include "graph/dx9/VertexShader_DX9.h"
#include "graph/dx9/PixelShader_DX9.h"
#include "graph/GraphicsDevice.h"

#include "../2d/shader/2DVS.h"
#include "../2d/shader/2DPS_OpVtx.h"
#include "../2d/shader/2DPS_OpTex.h"
#include "../2d/shader/2DPS_OpModulate.h"
#include "../2d/shader/2DPS_OpModulate2x.h"
#include "../2d/shader/2DPS_OpModulate4x.h"
#include "../2d/shader/2DPS_Add.h"
#include "../2d/shader/2DPS_OpModulateAlpha.h"
#include "../2d/shader/2DPS_OpNoTexAlpha.h"

static const IZ_BYTE* PS_Programs[] = {
    (const IZ_BYTE*)g_ps20_main_OpVtx,
    (const IZ_BYTE*)g_ps20_main_OpTex,
    (const IZ_BYTE*)g_ps20_main_OpModulate,
    (const IZ_BYTE*)g_ps20_main_OpModulate2x,
    (const IZ_BYTE*)g_ps20_main_OpModulate4x,
    (const IZ_BYTE*)g_ps20_main_OpAdd,
    (const IZ_BYTE*)g_ps20_main_OpModulateAlpha,
    (const IZ_BYTE*)g_ps20_main_OpNoTexAlpha,
};

C_ASSERT(COUNTOF(PS_Programs) == izanagi::graph::E_GRAPH_2D_RENDER_OP_NUM);

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
        IZ_BYTE* buf = (IZ_BYTE*)ALLOC_ZERO(allocator, sizeof(C2DShaderDX9));
        IZ_BOOL result = (buf != IZ_NULL);
        if (!result) {
            IZ_ASSERT(IZ_FALSE);
            goto __EXIT__;
        }

        // インスタンス作成
        instance = new (buf) C2DShaderDX9();
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
    C2DShaderDX9::C2DShaderDX9()
    {
        FILL_ZERO(m_hVtxParam, sizeof(m_hVtxParam));
    }

    // デストラクタ
    C2DShaderDX9::~C2DShaderDX9()
    {
    }

    // シェーダ作成
    IZ_BOOL C2DShaderDX9::CreateShader(CGraphicsDevice* device)
    {
        IZ_ASSERT(device != NULL);

        // シェーダプログラム
        m_ShaderProgram = device->CreateShaderProgram();
        VRETURN(m_ShaderProgram != IZ_NULL);

        // 頂点シェーダ
        m_pVS = device->CreateVertexShader(g_vs20_main);
        VRETURN(m_pVS != IZ_NULL);

        // ピクセルシェーダ
        for (IZ_UINT i = 0; i < E_GRAPH_2D_RENDER_OP_NUM; ++i) {
            m_pPS[i] = device->CreatePixelShader(PS_Programs[i]);
            VRETURN(m_pPS[i] != IZ_NULL);
        }

        // シェーダパラメータ
        static IZ_PCSTR ParamName[] = {
            "g_vInvScreen",
        };
        C_ASSERT(COUNTOF(ParamName) == VTX_PARAM_NUM);

        for (IZ_UINT i = 0; i < VTX_PARAM_NUM; ++i) {
            m_hVtxParam[i].vsParam = ((CVertexShaderDX9*)m_pVS)->GetHandleByName(ParamName[i]);
            VRETURN(m_hVtxParam[i].vsParam != IZ_NULL);
        }

        return IZ_TRUE;
    }

    // シェーダパラメータセット
    IZ_BOOL C2DShaderDX9::SetShaderParams(CGraphicsDevice* device)
    {
        IZ_ASSERT(device != NULL);
        IZ_ASSERT(m_pVS != NULL);

        // スクリーンサイズの逆数
        {
            math::SVector vTmp;
            vTmp.x = 1.0f / device->GetViewport().width;
            vTmp.y = 1.0f / device->GetViewport().height;

            IZ_ASSERT(m_hVtxParam[VTX_PARAM_INV_SCREEN].vsParam != IZ_NULL);

            VRETURN(
                ((CVertexShaderDX9*)m_pVS)->SetVector(
                    device,
                    m_hVtxParam[VTX_PARAM_INV_SCREEN].vsParam, 
                    vTmp));
        }

        return IZ_TRUE;
    }
}   // namespace graph
}   // namespace izanagi
