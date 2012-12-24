#include "MouseHit.h"

CMouseHit::CMouseHit()
{
    memset(m_Rectangles, 0, sizeof(m_Rectangles));
    m_Shader = IZ_NULL;

    m_IsPressKey = IZ_FALSE;

    m_MousePoint.x = -1;
    m_MousePoint.y = -1;

    m_CrossRectIdx = -1;
}

CMouseHit::~CMouseHit()
{
}

// 初期化.
IZ_BOOL CMouseHit::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    izanagi::math::SMatrix::GetTrans(
        m_Rectangles[0].mtx,
        -10.0f, 0.0f, -10.0f);
    izanagi::math::SMatrix::GetTrans(
        m_Rectangles[1].mtx,
        -10.0f, 0.0f, 10.0f);
    izanagi::math::SMatrix::GetTrans(
        m_Rectangles[2].mtx,
        10.0f, 0.0f, -10.0f);
    izanagi::math::SMatrix::GetTrans(
        m_Rectangles[3].mtx,
        10.0f, 0.0f, 10.0f);

    static const IZ_FLOAT Length = 10.0f;
    static const IZ_COLOR color[] = {
        IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
        IZ_COLOR_RGBA(0xff, 0x00, 0x00, 0xff),
        IZ_COLOR_RGBA(0x00, 0xff, 0x00, 0xff),
        IZ_COLOR_RGBA(0xff, 0x00, 0xff, 0xff),
    };
    IZ_C_ASSERT(COUNTOF(color) == COUNTOF(m_Rectangles));

    for (IZ_UINT i = 0; i < COUNTOF(m_Rectangles); i++)
    {
        m_Rectangles[i].mesh = izanagi::CDebugMeshRectangle::CreateDebugMeshRectangle(
            allocator,
            device,
            izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR,
            color[i],
            1, 1,
            Length, Length);

        m_Rectangles[i].rc.Set(
            //izanagi::math::CVector(-Length * 0.5f, 0.0f, Length * 0.5f),
            izanagi::math::CVector(0.0f, 0.0f, 0.0f),
            Length, Length);

        m_Rectangles[i].rc.Transform(m_Rectangles[i].mtx);
    }

    // シェーダ
    {
        izanagi::CFileInputStream in;
        VGOTO(result = in.Open("data/BasicShader.shd"), __EXIT__);

        m_Shader = izanagi::CShaderBasic::CreateShader<izanagi::CShaderBasic>(
                    allocator,
                    device,
                    &in);
        VGOTO(result = (m_Shader != IZ_NULL), __EXIT__);
    }

    // カメラ
    camera.Init(
        izanagi::math::CVector(0.0f, 5.0f, -30.0, 1.0f),
        izanagi::math::CVector(0.0f, 5.0f, 0.0f, 1.0f),
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
void CMouseHit::ReleaseInternal()
{
    for (IZ_UINT i = 0; i < COUNTOF(m_Rectangles); i++)
    {
        SAFE_RELEASE(m_Rectangles[i].mesh);
    }

    SAFE_RELEASE(m_Shader);
}

// 更新.
void CMouseHit::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();

    m_CrossRectIdx = -1;
    izanagi::math::SVector dir;

    //if (m_IsPressKey)
    {
        // 2D -> 3D
        if (m_MousePoint.x >= 0
            && m_MousePoint.y >= 0)
        {
            izanagi::CSceneGraphUtil::Point2Ray(
                dir,
                GetCamera().GetParam(),
                device->GetViewport(),
                m_MousePoint.x,
                m_MousePoint.y);

            izanagi::math::CRay ray(
                GetCamera().GetParam().pos,
                dir);

            for (IZ_UINT i = 0; i < COUNTOF(m_Rectangles); i++)
            {
                if (m_Rectangles[i].rc.IsCross(ray))
                {
                    m_CrossRectIdx = i;
                    break;
                }
            }
        }
    }
}

namespace {
    inline void _SetShaderParam(
        izanagi::CShaderBasic* shader,
        const char* name,
        const void* value,
        IZ_UINT bytes)
    {
        izanagi::IZ_SHADER_HANDLE handle = shader->GetParameterByName(name);
        IZ_ASSERT(handle != 0);

        shader->SetParamValue(
            handle,
            value,
            bytes);
    }
}

// 描画.
void CMouseHit::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::sample::CSampleCamera& camera = GetCamera();

    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_CULLMODE,
        izanagi::graph::E_GRAPH_CULL_NONE);

    static const izanagi::math::SVector color[2] = {
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 0.5f },
    };

    m_Shader->Begin(0, IZ_FALSE);

    for (IZ_UINT i = 0; i < COUNTOF(m_Rectangles); i++)
    {
        if (m_Shader->BeginPass(0)) {
            // パラメータ設定
            _SetShaderParam(
                m_Shader,
                "g_mL2W",
                (void*)&m_Rectangles[i].mtx,
                sizeof(m_Rectangles[i].mtx));

            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&camera.GetParam().mtxW2C,
                sizeof(camera.GetParam().mtxW2C));

            const izanagi::math::SVector& clr = (m_CrossRectIdx == i ? color[1] : color[0]);

            _SetShaderParam(
                m_Shader,
                "g_Color",
                (void*)&clr,
                sizeof(clr));

            // シェーダ設定
            m_Shader->CommitChanges();

            m_Rectangles[i].mesh->Draw();

            m_Shader->EndPass();
        }
    }

    m_Shader->End();
}

IZ_BOOL CMouseHit::OnKeyDown(IZ_UINT nChar)
{
    m_IsPressKey = IZ_TRUE;
    return IZ_TRUE;
}

void CMouseHit::OnKeyUp(IZ_UINT nChar)
{
    m_IsPressKey = IZ_FALSE;
}

IZ_BOOL CMouseHit::OnMouseLBtnDown(const izanagi::CIntPoint& point)
{
    if (m_IsPressKey)
    {
        m_MousePoint.x = point.x;
        m_MousePoint.y = point.y;
    }

    return IZ_TRUE;
}
