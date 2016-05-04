#include "StencilTestApp.h"

StencilTestApp::StencilTestApp()
{
}

StencilTestApp::~StencilTestApp()
{
}

// 初期化.
IZ_BOOL StencilTestApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    // Axis
    m_Axis = izanagi::CDebugMeshAxis::CreateDebugMeshAxisDefault(
                allocator,
                device);
    VGOTO(result = (m_Axis != IZ_NULL), __EXIT__);

    // Grid
    m_Grid = izanagi::CDebugMeshGrid::CreateDebugMeshGrid(
                allocator,
                device,
                IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
                10, 10,
                5.0f);
    VGOTO(result = (m_Grid != IZ_NULL), __EXIT__);

    IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
        | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR
        | izanagi::E_DEBUG_MESH_VTX_FORM_UV;


    m_Mesh = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
        allocator,
        device,
        flag,
        IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
        5.0f,
        10, 10);
    VGOTO(result = (m_Mesh != IZ_NULL), __EXIT__);

    // テクスチャ
    {
        izanagi::CFileInputStream in;
        VGOTO(result = in.Open("data/earth.img"), __EXIT__);

        m_Img = izanagi::CImage::CreateImage(
                    allocator,
                    device,
                    &in);
        VGOTO(result = (m_Img != IZ_NULL), __EXIT__);
    }

    // シェーダ
    {
        izanagi::CFileInputStream in;
        VGOTO(result = in.Open("data/BasicShader.shd"), __EXIT__);

        m_Shader = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
                    allocator,
                    device,
                    &in);
        VGOTO(result = (m_Shader != IZ_NULL), __EXIT__);
    }

    // カメラ
    camera.Init(
        izanagi::math::CVector4(0.0f, 10.0f, 30.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 0.0f, 0.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        500.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)device->GetBackBufferWidth() / device->GetBackBufferHeight());
    camera.Update();

__EXIT__:
    if (!result) {
        ReleaseInternal();
    }

    return IZ_TRUE;
}

// 解放.
void StencilTestApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Mesh);

    SAFE_RELEASE(m_Axis);
    SAFE_RELEASE(m_Grid);

    SAFE_RELEASE(m_Img);

    SAFE_RELEASE(m_Shader);
}

// 更新.
void StencilTestApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();
}

namespace {
    inline void _SetShaderParam(
        izanagi::shader::CShaderBasic* shader,
        const char* name,
        const void* value,
        IZ_UINT bytes)
    {
        izanagi::shader::IZ_SHADER_HANDLE handle = shader->GetParameterByName(name);
        IZ_ASSERT(handle != 0);

        shader->SetParamValue(
            handle,
            value,
            bytes);
    }
}

// 描画.
void StencilTestApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    if (device->Begin2D()) {
        device->SetRenderState(
            izanagi::graph::E_GRAPH_RS_STENCIL_ENABLE,
            IZ_TRUE);

        // 四角形の描画範囲にステンシル１を付加する.
        device->SetStencilFunc(
            izanagi::graph::E_GRAPH_CMP_FUNC_ALWAYS,
            1,
            0xffffffff);

        // これから描画するもののステンシル値にすべて１タグをつける.
        device->SetStencilOp(
            izanagi::graph::E_GRAPH_STENCIL_OP_REPLACE,
            izanagi::graph::E_GRAPH_STENCIL_OP_REPLACE,
            izanagi::graph::E_GRAPH_STENCIL_OP_REPLACE);

        // カラー・デプスバッファに書き込みしない.
        device->SetRenderState(
            izanagi::graph::E_GRAPH_RS_ZENABLE,
            IZ_FALSE);
        device->SetRenderState(
            izanagi::graph::E_GRAPH_RS_COLORWRITEENABLE_RGB,
            IZ_FALSE);
        device->SetRenderState(
            izanagi::graph::E_GRAPH_RS_COLORWRITEENABLE_A,
            IZ_FALSE);

        static const IZ_INT rcWidth = 300;
        static const IZ_INT rcHeight = 300;

        static const IZ_INT centerX = SCREEN_WIDTH / 2;
        static const IZ_INT centerY = SCREEN_HEIGHT / 2;

        device->Draw2DRect(
            izanagi::CIntRect(centerX - rcWidth / 2, centerY - rcHeight / 2, rcWidth, rcHeight),
            IZ_COLOR_RGBA(0, 0, 0, 0xff));

        device->End2D();
    }

    {
        device->SetRenderState(
            izanagi::graph::E_GRAPH_RS_STENCIL_ENABLE,
            IZ_TRUE);

        // 0と一致する場合にテストを通す.
        device->SetStencilFunc(
            izanagi::graph::E_GRAPH_CMP_FUNC_EQUAL,
            0,
            0xffffffff);

        // ステンシルは書き換えない.
        device->SetStencilOp(
            izanagi::graph::E_GRAPH_STENCIL_OP_KEEP,
            izanagi::graph::E_GRAPH_STENCIL_OP_KEEP,
            izanagi::graph::E_GRAPH_STENCIL_OP_KEEP);
    }

    izanagi::sample::CSampleCamera& camera = GetCamera();

    izanagi::math::SMatrix44 mtxL2W;
    izanagi::math::SMatrix44::SetUnit(mtxL2W);

    // テクスチャなし
    m_Shader->Begin(device, 1, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            // パラメータ設定
            _SetShaderParam(
                m_Shader,
                "g_mL2W",
                (void*)&mtxL2W,
                sizeof(mtxL2W));

            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&camera.GetParam().mtxW2C,
                sizeof(camera.GetParam().mtxW2C));

            // シェーダ設定
            m_Shader->CommitChanges(device);

            m_Grid->Draw(device);
            m_Axis->Draw(device);

            m_Shader->EndPass();
        }
    }
    m_Shader->End(device);

    device->SetTexture(0, m_Img->GetTexture(0));

    // テクスチャあり
    m_Shader->Begin(device, 0, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            // パラメータ設定
            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&camera.GetParam().mtxW2C,
                sizeof(izanagi::math::SMatrix44));

            _SetShaderParam(
                m_Shader,
                "g_mL2W",
                (void*)&mtxL2W,
                sizeof(izanagi::math::SMatrix44));

            m_Shader->CommitChanges(device);

            m_Mesh->Draw(device);

            m_Shader->EndPass();
        }
    }
    m_Shader->End(device);

    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_STENCIL_ENABLE,
        IZ_FALSE);
}
