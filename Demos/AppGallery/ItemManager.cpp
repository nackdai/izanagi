#include "ItemManager.h"
#include "Board.h"
#include "Configure.h"
#include "Item.h"

ItemManager ItemManager::instance;

ItemManager& ItemManager::Instance()
{
    return instance;
}

ItemManager::ItemManager()
{
    m_BoardMesh = IZ_NULL;
}

ItemManager::~ItemManager()
{
}

IZ_BOOL ItemManager::Init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device)
{
    m_BoardMesh = Board::Create(
        allocator,
        device,
        Configure::MeshFlags,
        IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
        5.0f,
        10.0f,
        2.0f);

    IZ_FLOAT d = IZ_MATH_PI2 / Configure::ItemNum;

    IZ_FLOAT theta = 0.0f;

    for (IZ_UINT i = 0; i< Configure::ItemNum; i++) {
        Item* item = Item::Create(
            allocator,
            m_BoardMesh);

        IZ_FLOAT s = Configure::Radius * ::sinf(theta);
        IZ_FLOAT c = Configure::Radius * ::cosf(theta);

        item->SetPosAndRotate(
            izanagi::math::CVector4(s, 0.0f, c),
            theta);

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

    SAFE_RELEASE(m_BoardMesh);
}

void ItemManager::Render(
    izanagi::graph::CGraphicsDevice* device,
    izanagi::shader::CShaderBasic* shader)
{
    izanagi::CStdList<Item>::Item* listItem = m_Items.GetTop();
    while (listItem != IZ_NULL) {
        Item* item = listItem->GetData();

        item->Render(device, shader);

        listItem = listItem->GetNext();
    }
}
