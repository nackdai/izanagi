#include "Item.h"
#include "ItemBox.h"
#include "Utility.h"
#include "ItemBoard.h"
#include "Configure.h"
#include "Environment.h"

Item* Item::Create(
    izanagi::IMemoryAllocator* allocator,
    ItemBox* box,
    ItemBoard* board)
{
    void* buf = ALLOC_ZERO(allocator, sizeof(Item));
    VRETURN_NULL(buf != IZ_NULL);

    Item* instance = new(buf) Item;
    {
        instance->AddRef();

        instance->m_Allocator = allocator;
        SAFE_REPLACE(instance->m_Box, box);
        SAFE_REPLACE(instance->m_Board, board);
    }

    return instance;
}

Item::Item()
{
    m_Allocator = IZ_NULL;
    m_Box = IZ_NULL;

    m_ListItem.Init(this);

    m_Board = IZ_NULL;
    m_Tex = IZ_NULL;

    m_IsRequestedLoadTexture = IZ_FALSE;
}

Item::~Item()
{
    SAFE_RELEASE(m_Box);
    SAFE_RELEASE(m_Board);
    SAFE_RELEASE(m_Tex);

    FREE(m_Allocator, this);
}

void Item::SetPosAndRotate(IZ_FLOAT theta)
{
    IZ_FLOAT angle = IZ_DEG2RAD(theta);
    IZ_FLOAT r = Configure::Radius;

    IZ_FLOAT s = r * ::sinf(angle);
    IZ_FLOAT c = r * ::cosf(angle);

    izanagi::math::SMatrix44::GetRotByY(
        m_BoxL2W,
        IZ_DEG2RAD(theta - 180.0f));
    izanagi::math::SMatrix44::Trans(m_BoxL2W, m_BoxL2W, s, 0.0f, c);

    r += Configure::ItemDepth * 0.5f;

    // TODO
    // 深度バイアスをもっとまじめにやる必要がある
    r += 0.001f;

    s = r * ::sinf(angle);
    c = r * ::cosf(angle);

    izanagi::math::SMatrix44::GetRotByY(
        m_BoardL2W,
        IZ_DEG2RAD(theta - 180.0f));
    izanagi::math::SMatrix44::Trans(m_BoardL2W, m_BoardL2W, s, 0.0f, c);
}

void Item::Render(
    izanagi::graph::CGraphicsDevice* device,
    izanagi::shader::CShaderBasic* shader,
    const izanagi::CCamera& camera,
    const izanagi::math::SMatrix44& mtxRot)
{
    izanagi::math::SMatrix44 mtx;
    izanagi::math::SMatrix44::Mul(mtx, m_BoxL2W, mtxRot);

    izanagi::math::SMatrix44::RotByZ(
        mtx, mtx,
        Configure::Gradient);

    Utility::SetShaderParam(
        shader,
        "g_mL2W",
        (void*)&mtx,
        sizeof(mtx));

    Environment::Instance().SetLocalLightParam(
        camera,
        mtx,
        shader);

    shader->CommitChanges(device);

    m_Box->Draw(device);
}

void Item::RenderBoard(
    izanagi::graph::CGraphicsDevice* device,
    izanagi::shader::CShaderBasic* shader,
    const izanagi::math::SMatrix44& mtxRot)
{
    static const IZ_FLOAT delta = 0.01f;

    izanagi::math::SMatrix44 mtx;
    izanagi::math::SMatrix44::Mul(mtx, m_BoardL2W, mtxRot);

    izanagi::math::SMatrix44::RotByZ(
        mtx, mtx,
        Configure::Gradient);

    if (m_Tex != IZ_NULL) {
        device->SetTexture(0, m_Tex);

        Utility::SetShaderParam(
            shader,
            "g_mL2W",
            (void*)&mtx,
            sizeof(mtx));

        shader->CommitChanges(device);

        m_Board->Draw(device);
    }
}

izanagi::CStdList<Item>::Item* Item::GetListItem()
{
    return &m_ListItem;
}

 const izanagi::math::SMatrix44& Item::GetL2W()
 {
     return m_BoxL2W;
 }

 void Item::SetTexture(izanagi::graph::CTexture* texture)
 {
     SAFE_REPLACE(m_Tex, texture);
 }

void Item::SetIsRequestedLoadTexture(IZ_BOOL flag)
{
    m_IsRequestedLoadTexture = flag;
}

IZ_BOOL Item::IsRequestedLoadTexture() const
{
    return m_IsRequestedLoadTexture;
}
