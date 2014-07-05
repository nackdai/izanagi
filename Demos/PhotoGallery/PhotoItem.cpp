#include "PhotoItem.h"
#include "PhotoItemMesh.h"
#include "Configure.h"
#include "Utility.h"

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

PhotoItem::PhotoItem()
{
    m_Allocator = IZ_NULL;

    m_Texture = IZ_NULL;
    m_Mesh = IZ_NULL;

    izanagi::math::SMatrix::SetUnit(m_L2W);

    m_ListItem.Init(this);

    m_IsRequestedLoadTexture = IZ_FALSE;
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
        izanagi::math::CVector(-Width * 0.5f, 0.0f, 0.0f),  // Right-Bottom point.
        izanagi::math::CVector(Width, 0.0f, 0.0f),          // Left direction.
        izanagi::math::CVector(0.0f, Height, 0.0f));        // Up direction.

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

izanagi::CStdList<PhotoItem>::Item* PhotoItem::GetListItem()
{
    return &m_ListItem;
}

void PhotoItem::SetTexture(izanagi::graph::CTexture* texture)
{
    SAFE_REPLACE(m_Texture, texture);
}

IZ_BOOL PhotoItem::HasTexture()
{
    return (m_Texture != IZ_NULL);
}

void PhotoItem::SetPositionAndRotation(
    const izanagi::math::CVector& pos,
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

IZ_BOOL PhotoItem::HitTest(
    const izanagi::math::CRay& ray,
    const izanagi::math::SMatrix& mtxRot)
{
    izanagi::math::SMatrix mtx;
    izanagi::math::SMatrix::Mul(mtx, m_L2W, mtxRot);

    izanagi::math::CRectangle rc;
    m_Rectangle.Transform(rc, mtx);

    IZ_BOOL ret = rc.IsIntersect(ray);

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

void PhotoItem::GetNormal(izanagi::math::SVector& nml)
{
    izanagi::math::SMatrix::ApplyXYZ(nml, m_Rectangle.nml, m_L2W);
    nml.w = 0.0f;

    izanagi::math::SVector::Normalize(nml, nml);
}

void PhotoItem::GetCenterPosition(izanagi::math::SVector& pos)
{
    izanagi::math::SVector::Set(
        pos,
        0.0f,
        Height * 0.5f,
        0.0f,
        1.0f);

    izanagi::math::SMatrix::Apply(pos, pos, m_L2W);
}

void PhotoItem::SetShaderParam(izanagi::shader::CShaderBasic* shader)
{
    izanagi::math::CVector params(1.0f, 1.0f, 1.0f, 1.0f);

    if (HasTexture()) {
        params.x = izanagi::math::CMath::Clamp(m_Texture->GetWidth() / (IZ_FLOAT)Configure::MaxTextureSize, 0.0f, 1.0f);
        params.y = izanagi::math::CMath::Clamp(m_Texture->GetHeight() / (IZ_FLOAT)Configure::MaxTextureSize, 0.0f, 1.0f);
    }

    Utility::SetShaderParam(
        shader,
        "g_Params",
        (void*)&params,
        sizeof(params));
}
