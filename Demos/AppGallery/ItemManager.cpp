#include "ItemManager.h"
#include "ItemBox.h"
#include "Configure.h"
#include "Item.h"
#include "ItemBoard.h"
#include "LoadTextureJob.h"

ItemManager ItemManager::instance;

ItemManager& ItemManager::Instance()
{
    return instance;
}

ItemManager::ItemManager()
{
    m_BoxMesh = IZ_NULL;
    m_BoardMesh = IZ_NULL;

    m_AngleRate = 0.0f;
    m_AngleImmediately = 0.0f;

    izanagi::math::SMatrix44::SetUnit(m_mtxRot);
}

ItemManager::~ItemManager()
{
}

IZ_BOOL ItemManager::Init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device)
{
    m_BoxMesh = ItemBox::Create(
        allocator,
        device,
        Configure::BoxMeshFlags,
        IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
        Configure::ItemWidth,
        Configure::ItemHeight,
        Configure::ItemDepth);

    m_BoardMesh = ItemBoard::Create(
        allocator,
        device,
        Configure::BoardMeshFlags,
        IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
        Configure::ItemWidth - Configure::ItemBoardMargin,
        Configure::ItemHeight - Configure::ItemBoardMargin);

    IZ_FLOAT d = 360.0f / Configure::ItemNum;

    IZ_FLOAT theta = 0.0f;

    for (IZ_UINT i = 0; i< Configure::ItemNum; i++) {
        Item* item = Item::Create(
            allocator,
            m_BoxMesh,
            m_BoardMesh);

        item->SetPosAndRotate(theta);

        theta += d;

        m_Items.AddTail(item->GetListItem());
    }

    return IZ_TRUE;
}

void ItemManager::Terminate()
{
    izanagi::CStdList<Item>::Item* listItem = m_Items.GetTop();
    while (listItem != IZ_NULL) {
        izanagi::CStdList<Item>::Item* next = listItem->GetNext();

        Item* item = listItem->GetData();
        listItem->Leave();
        SAFE_RELEASE(item);

        listItem = next;
    }

    SAFE_RELEASE(m_BoxMesh);
    SAFE_RELEASE(m_BoardMesh);
}

IZ_BOOL ItemManager::EnqueueLoadingRequest(
    izanagi::graph::CGraphicsDevice* device,
    const char* path)
{
    // Find a photo item which does not request to load a texture.
    Item* item = FindNotRequestedLoadTexture();
    if (item == IZ_NULL) {
        return IZ_FALSE;
    }

    TextureLoader::Instance().EnqueueLoadingRequest(
        device,
        path,
        item);

    item->SetIsRequestedLoadTexture(IZ_TRUE);

    return IZ_TRUE;

}

void ItemManager::Update(
    IZ_FLOAT time,
    izanagi::graph::CGraphicsDevice* device,
    const izanagi::CCamera& camera)
{
    // Update rotation of photo items.
    if (m_AngleImmediately != 0.0f) {
        izanagi::math::SMatrix44::RotByY(
            m_mtxRot,
            m_mtxRot,
            m_AngleImmediately);
        m_AngleImmediately = 0.0f;
    }
    else if (m_AngleRate != 0.0f) {
        izanagi::math::SMatrix44::RotByY(
            m_mtxRot,
            m_mtxRot,
            m_AngleRate);

        m_AngleRate *= 0.95f;

        if (izanagi::math::CMath::Absf(m_AngleRate) < IZ_MATH_PI2 / 10000.0f) {
            m_AngleRate = 0.0f;
        }
    }
}

void ItemManager::RenderBox(
    izanagi::graph::CGraphicsDevice* device,
    izanagi::shader::CShaderBasic* shader,
    const izanagi::CCamera& camera)
{
    izanagi::CStdList<Item>::Item* listItem = m_Items.GetTop();
    while (listItem != IZ_NULL) {
        Item* item = listItem->GetData();

        item->Render(device, shader, camera, m_mtxRot);

        listItem = listItem->GetNext();
    }
}

void ItemManager::RenderBoard(
    izanagi::graph::CGraphicsDevice* device,
    izanagi::shader::CShaderBasic* shader)
{
    izanagi::CStdList<Item>::Item* listItem = m_Items.GetTop();
    while (listItem != IZ_NULL) {
        Item* item = listItem->GetData();

        item->RenderBoard(device, shader, m_mtxRot);

        listItem = listItem->GetNext();
    }
}

void ItemManager::SetAngleRate(IZ_FLOAT angle)
{
    m_AngleRate = angle;
}

void ItemManager::SetAngleForImmediateRot(IZ_FLOAT angle)
{
    m_AngleImmediately = angle;
}

IZ_BOOL ItemManager::IsRotateAnimating()
{
    return (m_AngleRate != 0.0f);
}

Item* ItemManager::FindNotRequestedLoadTexture()
{
    izanagi::CStdList<Item>::Item* listItem = m_Items.GetTop();

    while (listItem != IZ_NULL) {
        Item* item = listItem->GetData();

        if (!item->IsRequestedLoadTexture()) {
            return item;
        }

        listItem = listItem->GetNext();
    }

    return IZ_NULL;
}
