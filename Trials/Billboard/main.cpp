#include "izSampleKit.h"
#include "izDebugUtil.h"
#include "izGraph.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

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

////////////////////////////////////

class CBillboardYAxis {
public:
    CBillboardYAxis();
    ~CBillboardYAxis();

public:
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device);

    IZ_BOOL Render(
        izanagi::graph::CGraphicsDevice* device,
        izanagi::sample::CSampleCamera& camera);

    void Release();

private:
    izanagi::CDebugMeshRectangle* m_Rect;
    izanagi::shader::CShaderBasic* m_Shader;
};

CBillboardYAxis::CBillboardYAxis()
{
}

CBillboardYAxis::~CBillboardYAxis()
{
}

void CBillboardYAxis::Release()
{
    SAFE_RELEASE(m_Rect);
    SAFE_RELEASE(m_Shader);
}

IZ_BOOL CBillboardYAxis::Init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device)
{
    m_Rect = izanagi::CDebugMeshRectangle::CreateDebugMeshRectangle(
        allocator,
        device,
        izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR,
        IZ_COLOR_RGBA(0xff, 0x00, 0x00, 0xff),
        1, 1, 
        20.0f, 200.0f);

    izanagi::CFileInputStream in;
    in.Open("data/BasicShader.shd");

    m_Shader = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
                allocator,
                device,
                &in);

    return IZ_TRUE;
}

IZ_BOOL CBillboardYAxis::Render(
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    izanagi::math::SMatrix mtx;
    izanagi::math::SMatrix::SetUnit(mtx);
    izanagi::math::SMatrix::RotByX(mtx, mtx, IZ_DEG2RAD(-90.0f));

    izanagi::math::SVector dir = camera.GetDir();

    if (dir.x == 0.0f) {
    }
    else {
        IZ_FLOAT deg = IZ_RAD2DEG(::atan2(dir.z, dir.x));
        izanagi::math::SMatrix::RotByY(mtx, mtx, -1.0f * IZ_DEG2RAD(deg) + IZ_MATH_PI1_2);
    }

    m_Shader->Begin(device, 0, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            _SetShaderParam(
                m_Shader,
                "g_mL2W",
                (void*)&mtx,
                sizeof(mtx));

            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&camera.GetParam().mtxW2C,
                sizeof(camera.GetParam().mtxW2C));

            // シェーダ設定
            m_Shader->CommitChanges(device);

            m_Rect->Draw(device);
        }
    }
    m_Shader->End(device);

    return IZ_TRUE;
}

////////////////////////////////////

class CBillboardZAxis {
public:
    CBillboardZAxis();
    ~CBillboardZAxis();

public:
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device);

    IZ_BOOL Render(
        izanagi::graph::CGraphicsDevice* device,
        izanagi::sample::CSampleCamera& camera);

    void Release();

private:
    izanagi::CDebugMeshRectangle* m_Rect;
    izanagi::CDebugMeshRectangle* m_Rect2;
    izanagi::shader::CShaderBasic* m_Shader;
};

CBillboardZAxis::CBillboardZAxis()
{
}

CBillboardZAxis::~CBillboardZAxis()
{
}

void CBillboardZAxis::Release()
{
    SAFE_RELEASE(m_Rect);
    SAFE_RELEASE(m_Rect2);
    SAFE_RELEASE(m_Shader);
}

IZ_BOOL CBillboardZAxis::Init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device)
{
    m_Rect = izanagi::CDebugMeshRectangle::CreateDebugMeshRectangle(
        allocator,
        device,
        izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR,
        IZ_COLOR_RGBA(0x00, 0xff, 0x00, 0xff),
        1, 1, 
        20.0f, 200.0f);

    m_Rect2 = izanagi::CDebugMeshRectangle::CreateDebugMeshRectangle(
        allocator,
        device,
        izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR,
        IZ_COLOR_RGBA(0x00, 0x00, 0xff, 0xff),
        1, 1, 
        20.0f, 200.0f);

    izanagi::CFileInputStream in;
    in.Open("data/BasicShader.shd");

    m_Shader = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
                allocator,
                device,
                &in);

    return IZ_TRUE;
}

IZ_BOOL CBillboardZAxis::Render(
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    device->SetRenderState(izanagi::graph::E_GRAPH_RS_CULLMODE, izanagi::graph::E_GRAPH_CULL_NONE);

    izanagi::math::SMatrix mtx;
    izanagi::math::SMatrix::SetUnit(mtx);
    izanagi::math::SMatrix::RotByZ(mtx, mtx, IZ_DEG2RAD(-90.0f));

    izanagi::math::SMatrix mtx2;
    izanagi::math::SMatrix::SetUnit(mtx2);

    izanagi::math::SVector dir = camera.GetDir();

    if (dir.x == 0.0f) {
    }
    else {
        IZ_FLOAT deg = IZ_RAD2DEG(::atan2(dir.y, dir.x));
        izanagi::math::SMatrix::RotByZ(mtx, mtx, -1.0f * IZ_DEG2RAD(deg) + IZ_MATH_PI1_2);
        izanagi::math::SMatrix::RotByZ(mtx2, mtx2, -1.0f * IZ_DEG2RAD(deg) + IZ_MATH_PI1_2);
    }

    m_Shader->Begin(device, 0, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            {
                _SetShaderParam(
                    m_Shader,
                    "g_mL2W",
                    (void*)&mtx,
                    sizeof(mtx));

                _SetShaderParam(
                    m_Shader,
                    "g_mW2C",
                    (void*)&camera.GetParam().mtxW2C,
                    sizeof(camera.GetParam().mtxW2C));

                // シェーダ設定
                m_Shader->CommitChanges(device);

                m_Rect->Draw(device);
            }
            {
                _SetShaderParam(
                    m_Shader,
                    "g_mL2W",
                    (void*)&mtx2,
                    sizeof(mtx2));

                _SetShaderParam(
                    m_Shader,
                    "g_mW2C",
                    (void*)&camera.GetParam().mtxW2C,
                    sizeof(camera.GetParam().mtxW2C));

                // シェーダ設定
                m_Shader->CommitChanges(device);

                m_Rect->Draw(device);
            }
        }
    }
    m_Shader->End(device);

    return IZ_TRUE;
}

////////////////////////////////////

class CBillboard {
public:
    CBillboard();
    ~CBillboard();

public:
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device);

    IZ_BOOL Render(
        izanagi::graph::CGraphicsDevice* device,
        izanagi::sample::CSampleCamera& camera);

    void Release();

private:
    izanagi::CDebugMeshRectangle* m_Rect;
    izanagi::shader::CShaderBasic* m_Shader;
};

CBillboard::CBillboard()
{
}

CBillboard::~CBillboard()
{
}

void CBillboard::Release()
{
    SAFE_RELEASE(m_Rect);
    SAFE_RELEASE(m_Shader);
}

IZ_BOOL CBillboard::Init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device)
{
    m_Rect = izanagi::CDebugMeshRectangle::CreateDebugMeshRectangle(
        allocator,
        device,
        izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR,
        IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
        1, 1, 
        100.0f, 100.0f);

    izanagi::CFileInputStream in;
    in.Open("data/BasicShader.shd");

    m_Shader = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
                allocator,
                device,
                &in);

    return IZ_TRUE;
}

IZ_BOOL CBillboard::Render(
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    izanagi::math::SMatrix mtx;
    izanagi::math::SMatrix::SetUnit(mtx);

    // 軸と並行になる向きにする
    izanagi::math::SMatrix::GetRotByX(mtx, IZ_DEG2RAD(-90.0f));

    // カメラに正対するようにする
    izanagi::math::SMatrix inv;
    {
        izanagi::math::SMatrix::Copy(inv, camera.GetParam().mtxW2V);

        // 移動成分は消す
        inv.m[3][0] = inv.m[3][1] = inv.m[3][2] = 0.0f;

        // 逆行列にすることで、カメラのW2Vマトリクスと打ち消しあうようにする
        izanagi::math::SMatrix::Inverse(inv, inv);
    }

    izanagi::math::SMatrix::Mul(mtx, mtx, inv);

    m_Shader->Begin(device, 0, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            _SetShaderParam(
                m_Shader,
                "g_mL2W",
                (void*)&mtx,
                sizeof(mtx));

            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&camera.GetParam().mtxW2C,
                sizeof(camera.GetParam().mtxW2C));

            // シェーダ設定
            m_Shader->CommitChanges(device);

            m_Rect->Draw(device);
        }
    }
    m_Shader->End(device);

    return IZ_TRUE;
}


/////////////////////////////

// Yビルボードメモ
// http://www.c3.club.kyutech.ac.jp/gamewiki/index.php?%A5%D3%A5%EB%A5%DC%A1%BC%A5%C9

class CBillboardApp : public izanagi::sample::CSampleApp {
public:
    CBillboardApp();
    virtual ~CBillboardApp();

protected:
    // 初期化.
    virtual IZ_BOOL InitInternal(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::sample::CSampleCamera& camera);

    // 解放.
    virtual void ReleaseInternal();

    // 更新.
    virtual void UpdateInternal(izanagi::graph::CGraphicsDevice* device);

    // 描画.
    virtual void RenderInternal(izanagi::graph::CGraphicsDevice* device);

    virtual IZ_BOOL OnMouseLBtnDown(const izanagi::CIntPoint& point);
    virtual IZ_BOOL OnMouseLBtnUp(const izanagi::CIntPoint& point);
    virtual void OnMouseMove(const izanagi::CIntPoint& point);

private:
    CBillboard m_Billboard;
    CBillboardYAxis m_YBillboard;
    CBillboardZAxis m_ZBillboard;
};

CBillboardApp::CBillboardApp()
{
}

CBillboardApp::~CBillboardApp()
{
}

// 初期化.
IZ_BOOL CBillboardApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    m_Billboard.Init(allocator, device);
    m_YBillboard.Init(allocator, device);
    m_ZBillboard.Init(allocator, device);

    // カメラ
    camera.Init(
        izanagi::math::CVector(0.0f, 30.0f, -200.0f, 1.0f),
        izanagi::math::CVector(0.0f, 0.0f, 0.0f, 1.0f),
        izanagi::math::CVector(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        5000.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)SCREEN_WIDTH / SCREEN_HEIGHT);
    camera.Update();

    return IZ_TRUE;
}

// 解放.
void CBillboardApp::ReleaseInternal()
{
    m_Billboard.Release();
    m_YBillboard.Release();
    m_ZBillboard.Release();
}

// 更新.
void CBillboardApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::CCamera& camera = GetCamera();
    camera.Update();
}

IZ_BOOL CBillboardApp::OnMouseLBtnDown(const izanagi::CIntPoint& point)
{
    return IZ_TRUE;
}

IZ_BOOL CBillboardApp::OnMouseLBtnUp(const izanagi::CIntPoint& point)
{
    return IZ_TRUE;
}

void CBillboardApp::OnMouseMove(const izanagi::CIntPoint& point)
{
}

// 描画.
void CBillboardApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::sample::CSampleCamera& camera = GetCamera();

    //m_Billboard.Render(device, camera);
    //m_YBillboard.Render(device, camera);
    m_ZBillboard.Render(device, camera);

    if (device->Begin2D()) {
        device->End2D();
    }
}

static const IZ_UINT BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

CBillboardApp app;

IzMain(SCREEN_WIDTH, SCREEN_HEIGHT)
{
    int ret = SampleMain(
        IzGetSystemDataForMainFunc(),
        &app,
        "Billboard",
        IzGetScreenWidth(), IzGetScreenHeight(),
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
