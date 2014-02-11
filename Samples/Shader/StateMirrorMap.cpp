#include "StateMirrorMap.h"
#include "izGraph.h"
#include "izSystem.h"
#include "StateManager.h"

CStateMirrorMap::CStateMirrorMap(
    izanagi::sample::CSampleApp* app,
    izanagi::SCameraParam& camera)
: CStateBase(app, camera)
{
    m_Img = IZ_NULL;
    m_Shader = IZ_NULL;
    m_Sphere = IZ_NULL;
}

CStateMirrorMap::~CStateMirrorMap()
{
    Destroy();
}

// 描画.
IZ_BOOL CStateMirrorMap::Render(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::math::SMatrix mtxL2W;
    izanagi::math::SMatrix::SetUnit(mtxL2W);

    device->SetTexture(0, m_Img->GetTexture(0));

    m_Shader->Begin(device, 0, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            // パラメータ設定
            SetShaderParam(
                m_Shader,
                "g_mL2W",
                (void*)&mtxL2W,
                sizeof(mtxL2W));

            SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&m_Camera.mtxW2C,
                sizeof(m_Camera.mtxW2C));

            SetShaderParam(
                m_Shader,
                "g_mW2V",
                (void*)&m_Camera.mtxW2V,
                sizeof(m_Camera.mtxW2V));

            m_Shader->CommitChanges(device);

            m_Sphere->Draw(device);
        }
    }
    m_Shader->End(device);

    RenderName(device, "MirrorMap");

    return IZ_TRUE;
}

// 開始
IZ_BOOL CStateMirrorMap::Enter(
    izanagi::IMemoryAllocator* allocator,
    void* val)
{
    izanagi::graph::CGraphicsDevice* device = reinterpret_cast<izanagi::graph::CGraphicsDevice*>(val);

    IZ_BOOL result = IZ_TRUE;

    // テクスチャ
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/EnvMap.img"));

        m_Img = izanagi::CImage::CreateImage(
                    allocator,
                    device,
                    &in);
        VGOTO(result = (m_Img != IZ_NULL), __EXIT__);
    }

    // シェーダ
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/MirrorMapShader.shd"));

        m_Shader = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
                    allocator,
                    device,
                    &in);
        VGOTO(result = (m_Shader != IZ_NULL), __EXIT__);
    }

    // 球
    {
        IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
                        | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL;

        m_Sphere = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
                        allocator,
                        device,
                        flag,
                        IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
                        5.0f,
                        20, 20);
        VGOTO(result = (m_Sphere != IZ_NULL), __EXIT__);
    }

__EXIT__:
    if (!result) {
        Leave();
    }

    return result;
}

// ステートから抜ける（終了）.
IZ_BOOL CStateMirrorMap::Leave()
{
    SAFE_RELEASE(m_Img);
    SAFE_RELEASE(m_Shader);
    SAFE_RELEASE(m_Sphere);

    return IZ_TRUE;
}
