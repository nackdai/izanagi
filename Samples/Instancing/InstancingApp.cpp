#include "InstancingApp.h"

static const IZ_FLOAT POS_X = -50.0f;
static const IZ_FLOAT DISTANCE = 10.0f;
static const IZ_UINT ORDER = 20;

CInstancingApp::CInstancingApp()
{
    m_Mesh = IZ_NULL;

    m_InstancingVB = IZ_NULL;
    m_InstancingVD = IZ_NULL;

    m_Img = IZ_NULL;

    m_Shader = IZ_NULL;
}

CInstancingApp::~CInstancingApp()
{
}

// 初期化.
IZ_BOOL CInstancingApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

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

    {
        izanagi::graph::SVertexElement elements[10];
        
        IZ_UINT count = 0;

        for (count = 0; count < m_Mesh->GetVD()->GetElementNum(); count++) {
            elements[count] = m_Mesh->GetVD()->GetElement()[count];
        }

        for (IZ_UINT i = 0; i < 4; i++) {
            elements[count + i].Stream = 1;
            elements[count + i].Offset = sizeof(IZ_FLOAT) * 4 * i;
            elements[count + i].Type = izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT4;
            elements[count + i].Usage = izanagi::graph::E_GRAPH_VTX_DECL_USAGE_TEXCOORD;

            // NOTE
            // TexCoord0 は UV座標に使用されるの、１から開始する
            elements[count + i].UsageIndex = i + 1;
        }

        m_InstancingVD = device->CreateVertexDeclaration(elements, count + 4);
    }

    {
        m_InstancingVB = device->CreateVertexBuffer(
            sizeof(izanagi::math::SMatrix44),
            MeshNum,
            izanagi::graph::E_GRAPH_RSC_USAGE_STATIC);
        
        izanagi::math::SMatrix44* mtx;

        m_InstancingVB->Lock(device, 0, 0, (void**)&mtx, IZ_FALSE);

        for (IZ_UINT i = 0; i < MeshNum; i++)
        {
            izanagi::math::SMatrix44::SetUnit(mtx[i]);
            izanagi::math::SMatrix44::GetTrans(
                mtx[i],
                POS_X + DISTANCE * (i % ORDER),
                0.0f,
                -DISTANCE * (i / ORDER));

            izanagi::math::SMatrix44::Transpose(mtx[i], mtx[i]);
        }

        m_InstancingVB->Unlock(device);
    }

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
        VGOTO(result = in.Open("data/InstancingShader.shd"), __EXIT__);

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
void CInstancingApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Mesh);

    SAFE_RELEASE(m_InstancingVB);
    SAFE_RELEASE(m_InstancingVD);

    SAFE_RELEASE(m_Img);

    SAFE_RELEASE(m_Shader);
}

// 更新.
void CInstancingApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
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
void CInstancingApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    static const IZ_BOOL isDrawTangentSpaceAxis = IZ_FALSE;

    izanagi::sample::CSampleCamera& camera = GetCamera();

    device->SetTexture(0, m_Img->GetTexture(0));

    IZ_UINT techIdx = (m_enableInstaning ? 0 : 1);

    m_Shader->Begin(device, techIdx, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            // パラメータ設定
            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&camera.GetParam().mtxW2C,
                sizeof(izanagi::math::SMatrix44));

            if (m_enableInstaning) {
                m_Shader->CommitChanges(device);

                device->SetIndexBuffer(m_Mesh->GetIB());

                device->SetVertexDeclaration(m_InstancingVD);
                    
                device->SetVertexBufferInstanced(
                    0,
                    0,
                    m_Mesh->GetVB()->GetStride(),
                    izanagi::graph::E_GRAPH_VB_USAGE_INDEXEDDATA,
                    MeshNum,
                    m_Mesh->GetVB());

                device->SetVertexBufferInstanced(
                    1,
                    0,
                    m_InstancingVB->GetStride(),
                    izanagi::graph::E_GRAPH_VB_USAGE_INSTANCEDATA,
                    1,
                    m_InstancingVB);

                device->DrawIndexedInstancedPrimitive(
                    m_Mesh->GetPrimitiveType(),
                    0,
                    m_Mesh->GetVB()->GetVtxNum(),
                    0,
                    m_Mesh->GetPrimitiveCount());
            }
            else {
                izanagi::math::SMatrix44 mtxL2W;

                for (IZ_UINT i = 0; i < MeshNum; i++) {
                    izanagi::math::SMatrix44::GetTrans(
                        mtxL2W,
                        POS_X + DISTANCE * (i % ORDER),
                        0.0f,
                        -DISTANCE * (i / ORDER));

                    _SetShaderParam(
                        m_Shader,
                        "g_mL2W",
                        (void*)&mtxL2W,
                        sizeof(izanagi::math::SMatrix44));

                    m_Shader->CommitChanges(device);

                    m_Mesh->Draw(device);
                }
            }

            m_Shader->EndPass();
        }
    }
    m_Shader->End(device);

    izanagi::CDebugFont* debugFont = GetDebugFont();

    if (device->Begin2D()) {
        debugFont->Begin(device, 0, izanagi::CDebugFont::FONT_SIZE * 2);

        debugFont->DBPrint(
            device,
            "%s\n",
            m_enableInstaning ? "Instacing" : "No Instacing");

        debugFont->End();

        device->End2D();
    }
}

IZ_BOOL CInstancingApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    if (key == izanagi::sys::E_KEYBOARD_BUTTON_SPACE) {
        m_enableInstaning = !m_enableInstaning;
    }

    return IZ_TRUE;
}
