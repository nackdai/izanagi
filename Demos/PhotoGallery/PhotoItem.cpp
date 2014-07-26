#include "PhotoItem.h"
#include "PhotoItemMesh.h"
#include "Configure.h"
#include "Utility.h"
#include "LoadTextureJob.h"

// TODO
static const IZ_FLOAT Width = 10.0f;
static const IZ_FLOAT Height = 10.0f;

PhotoItem* PhotoItem::Create(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    PhotoItemMesh* mesh)
{
    void* buf = ALLOC(allocator, sizeof(PhotoItem));
    VRETURN_NULL(buf != IZ_NULL);

    IZ_BOOL result = IZ_TRUE;

    PhotoItem* instance = new (buf) PhotoItem;
    {
        instance->AddRef();
        instance->m_Allocator = allocator;
        result = instance->Init(
            allocator,
            device,
            mesh);
    }

    if (!result) {
        SAFE_RELEASE(instance);
    }

    return instance;
}

static IZ_UINT s_ID = 0;

PhotoItem::PhotoItem()
{
    m_ID = s_ID++;

    m_Allocator = IZ_NULL;

    m_Texture = IZ_NULL;
    m_Path = IZ_NULL;

    m_Mesh = IZ_NULL;

    izanagi::math::SMatrix::SetUnit(m_L2W);

    m_ListItem.Init(this);

    m_IsRequestedLoadTexture = IZ_FALSE;

    m_IsShown = IZ_FALSE;

    ResetFadeInParams();
}

PhotoItem::~PhotoItem()
{
    SAFE_RELEASE(m_Texture);
    SAFE_RELEASE(m_Mesh);
}

IZ_BOOL PhotoItem::Init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    PhotoItemMesh* mesh)
{
    if (mesh != IZ_NULL) {
        SAFE_REPLACE(m_Mesh, mesh);
    }
    else {
        m_Mesh = PhotoItemMesh::Create(
            allocator,
            device,
            Width, Height,
            Configure::Depth);
    }

    VRETURN(m_Mesh != IZ_NULL);

    // NOTE
    //          +y
    //      +---|---+
    //      |   |   |
    //      |   |   |
    //      |   |   |
    // +x <-+---0---+

    m_Rectangle.Set(
        izanagi::math::CVector4(-Width * 0.5f, 0.0f, 0.0f),  // Right-Bottom point.
        izanagi::math::CVector4(Width, 0.0f, 0.0f),          // Left direction.
        izanagi::math::CVector4(0.0f, Height, 0.0f));        // Up direction.

    return IZ_TRUE;
}

void PhotoItem::RenderFront(izanagi::graph::CGraphicsDevice* device)
{
    IZ_ASSERT(device != IZ_NULL);

    device->SetTexture(0, m_Texture);

    IZ_ASSERT(m_Mesh != IZ_NULL);
    m_Mesh->RenderFront(device);
}

void PhotoItem::RenderTopAndSide(izanagi::graph::CGraphicsDevice* device)
{
    IZ_ASSERT(device != IZ_NULL);

    IZ_ASSERT(m_Mesh != IZ_NULL);
    m_Mesh->RenderTopAndSide(device);
}

void PhotoItem::Update(
    IZ_FLOAT time,
    izanagi::graph::CGraphicsDevice* device,
    const izanagi::math::SMatrix& mtxRot,
    const izanagi::CCamera& camera)
{
    // NOTE
    // カメラは固定なので、カメラの後ろにいるかどうかで判定する
    // カメラはマイナスZ方向を向いているので、プラスZ方向がカメラの後ろになる

    izanagi::math::SVector4 pos;
    GetCenterPosition(pos);
    izanagi::math::SMatrix::ApplyXYZ(pos, pos, mtxRot);

    IZ_BOOL isShown = m_IsShown;
#if 0
    m_IsShown = (pos.z <= camera.GetParam().pos.z);
#else
    m_IsShown = (pos.z <= camera.GetParam().pos.z + Configure::PhotoItemShownRange);
#endif

    if (isShown != m_IsShown) {
        // State changed.
        if (isShown) {
            // Shown -> Hidden

            // Release texture.
            ResetFadeInParams();
            SAFE_RELEASE(m_Texture);
        }
        else {
            // Hidden -> Shown
            if (m_Texture == IZ_NULL
                && m_Path != IZ_NULL)
            {
                // Request loading texture again.
                TextureLoader::Instance().EnqueueLoadingRequest(
                    device,
                    m_Path,
                    this);
            }
        }
    }

    if (!HasTexture()) {
        return;
    }

    if (m_IsFading) {
        // Update fade-in animation.
        m_Timeline.Advance(time);

        izanagi::animation::CTweenerCubic tweener;
        IZ_FLOAT value = tweener.EaseInOut(
            m_Timeline.GetTime(),
            m_Timeline.GetDuration());

        m_FadeInAlpha = izanagi::math::CMath::Clamp(1.0f * value, 0.0f, 1.0f);
        m_FadeInHeight = Configure::FadeInHeight * (1.0f - value);

        if (m_FadeInAlpha >= 1.0f) {
            m_IsFading = IZ_FALSE;
        }
    }
}

izanagi::CStdList<PhotoItem>::Item* PhotoItem::GetListItem()
{
    return &m_ListItem;
}

void PhotoItem::SetTexture(
    izanagi::graph::CTexture* texture,
    const char* path)
{
    SAFE_REPLACE(m_Texture, texture);
    m_Path = path;

    m_IsFading = IZ_TRUE;
}

IZ_BOOL PhotoItem::HasTexture()
{
    return (m_Texture != IZ_NULL);
}

void PhotoItem::SetPositionAndRotation(
    const izanagi::math::CVector4& pos,
    IZ_FLOAT rot)
{
    izanagi::math::SMatrix::GetRotByY(m_L2W, rot);

    izanagi::math::SMatrix::Trans(
        m_L2W,
        m_L2W,
        pos);
}

const izanagi::math::SMatrix& PhotoItem::GetL2W() const
{
    return m_L2W;
}

PhotoItemMesh* PhotoItem::GetMesh()
{
    return m_Mesh;
}

inline void _ComputeScale(
    izanagi::graph::CTexture* tex,
    IZ_FLOAT& scaleX,
    IZ_FLOAT& scaleY)
{
    scaleX = izanagi::math::CMath::Clamp(tex->GetWidth() / (IZ_FLOAT)Configure::MaxTextureSize, 0.0f, 1.0f);
    //IZ_FLOAT scaleY = izanagi::math::CMath::Clamp(m_Texture->GetHeight() / (IZ_FLOAT)Configure::MaxTextureSize, 0.0f, 1.0f);
        
    IZ_FLOAT aspect = tex->GetHeight() / (IZ_FLOAT)tex->GetWidth();
    scaleY = scaleX * aspect;
}

IZ_BOOL PhotoItem::HitTest(
    const izanagi::math::CRay& ray,
    const izanagi::math::SMatrix& mtxRot)
{
    if (!HasTexture() || m_IsFading) {
        return IZ_FALSE;
    }

    izanagi::math::SMatrix mtx;
    izanagi::math::SMatrix::Mul(mtx, m_L2W, mtxRot);

    izanagi::math::CRectangle rc;

    if (HasTexture()) {
        // Fit size to aspect of texture size.
        IZ_FLOAT scaleX = 1.0f;
        IZ_FLOAT scaleY = 1.0f;

        _ComputeScale(m_Texture, scaleX, scaleY);

        IZ_FLOAT w = Width * scaleX;
        IZ_FLOAT h = Height * scaleY;

        // Set rectangle parameters.
        rc.Set(
            izanagi::math::CVector4(-w * 0.5f, 0.0f, 0.0f),  // Right-Bottom point.
            izanagi::math::CVector4(w, 0.0f, 0.0f),          // Left direction.
            izanagi::math::CVector4(0.0f, h, 0.0f));         // Up direction.

        rc.Transform(mtx);
    }
    else {
        m_Rectangle.Transform(rc, mtx);
    }

    IZ_FLOAT t = 0.0f;
    IZ_BOOL ret = rc.IsIntersect(ray, &t);

    // NOTE
    // マイナス方向でのヒットはヒットしたことにしない
    ret = (t >= 0.0f ? ret : IZ_FALSE);

    return ret;
}

void PhotoItem::SetIsRequestedLoadTexture(IZ_BOOL flag)
{
    m_IsRequestedLoadTexture = flag;
}

IZ_BOOL PhotoItem::IsRequestedLoadTexture() const
{
    return m_IsRequestedLoadTexture;
}

void PhotoItem::GetNormal(izanagi::math::SVector4& nml)
{
    izanagi::math::SMatrix::ApplyXYZ(nml, m_Rectangle.nml, m_L2W);
    nml.w = 0.0f;

    izanagi::math::SVector4::Normalize(nml, nml);
}

void PhotoItem::GetCenterPosition(izanagi::math::SVector4& pos)
{
    IZ_FLOAT height = Height;

    if (HasTexture()) {
        // Fit size to aspect of texture size.
        IZ_FLOAT scaleX = 1.0f;
        IZ_FLOAT scaleY = 1.0f;

        _ComputeScale(m_Texture, scaleX, scaleY);

        height *= scaleY;
    }

    izanagi::math::SVector4::Set(
        pos,
        0.0f,
        height * 0.5f,
        0.0f,
        1.0f);

    izanagi::math::SMatrix::Apply(pos, pos, m_L2W);
}

void PhotoItem::SetShaderParam(izanagi::shader::CShaderBasic* shader)
{
    izanagi::math::CVector4 params(1.0f, 1.0f, m_FadeInHeight, m_FadeInAlpha);

    if (HasTexture()) {
        // Fit size to aspect of texture size.
        IZ_FLOAT scaleX = 1.0f;
        IZ_FLOAT scaleY = 1.0f;

        _ComputeScale(m_Texture, scaleX, scaleY);

        params.x = scaleX;
        params.y = scaleY;
    }

    Utility::SetShaderParam(
        shader,
        "g_Params",
        (void*)&params,
        sizeof(params));

    m_Mesh->SetMaterialToShader(shader);
}

IZ_BOOL PhotoItem::IsShown() const
{
    return m_IsShown;
}

void PhotoItem::ResetFadeInParams()
{
    m_IsFading = IZ_FALSE;

    m_FadeInAlpha = 0.0f;
    m_FadeInHeight = Configure::FadeInHeight;

    m_Timeline.Init(Configure::FadeInDuration, 0.0f);
    m_Timeline.Reset();
    m_Timeline.Start();
}
