#include "InstancingApp.h"

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
            sizeof(izanagi::math::SMatrix),
            MeshNum,
            izanagi::graph::E_GRAPH_RSC_USAGE_STATIC);
        
        izanagi::math::SMatrix* mtx;

        m_InstancingVB->Lock(0, 0, (void**)&mtx, IZ_FALSE);

        IZ_FLOAT x = -50.0f;

        for (IZ_UINT i = 0; i < MeshNum; i++)
        {
            izanagi::math::SMatrix::SetUnit(mtx[i]);
            izanagi::math::SMatrix::GetTrans(
                mtx[i],
                x + 10.0f * (i % 10),
                0.0f,
                -10.0f * (i / 10));

            izanagi::math::SMatrix::Transpose(mtx[i], mtx[i]);
        }

        m_InstancingVB->Unlock();
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
        izanagi::math::CVector(0.0f, 10.0f, 30.0f, 1.0f),
        izanagi::math::CVector(0.0f, 0.0f, 0.0f, 1.0f),
        izanagi::math::CVector(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        500.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)SCREEN_WIDTH / SCREEN_HEIGHT);
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

    // テクスチャあり
    m_Shader->Begin(device, 0, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            // パラメータ設定
            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&camera.GetParam().mtxW2C,
                sizeof(izanagi::math::SMatrix));

            m_Shader->CommitChanges(device);

            device->SetIndexBuffer(m_Mesh->GetIB());

            device->SetVertexDeclaration(m_InstancingVD);

            device->SetVertexBuffer(
                0, 0, 
                m_Mesh->GetVB()->GetStride(),
                m_Mesh->GetVB());
            device->SetVertexBufferInstanced(
                0,
                izanagi::graph::E_GRAPH_VB_USAGE_INDEXEDDATA,
                MeshNum);

            device->SetVertexBuffer(
                1, 0, 
                m_InstancingVB->GetStride(),
                m_InstancingVB);
            device->SetVertexBufferInstanced(
                1,
                izanagi::graph::E_GRAPH_VB_USAGE_INSTANCEDATA,
                1);

            device->DrawIndexedPrimitive(
                m_Mesh->GetPrimitiveType(),
                0,
                m_Mesh->GetVB()->GetVtxNum(),
                0,
                m_Mesh->GetPrimitiveCount());

            m_Shader->EndPass();
        }
    }
    m_Shader->End(device);
}
