#include "PhotoItemManager.h"
#include "PhotoItem.h"

//////////////////////////////

PhotoItemList::PhotoItemList()
{
}

PhotoItemList::~PhotoItemList()
{
}

void PhotoItemList::RenderFront(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::CStdList<PhotoItem>::Item* item = this->GetTop();

    while (item != IZ_NULL) {
        PhotoItem* photoItem = item->GetData();

        photoItem->RenderFront(device);

        item = item->GetNext();
    }
}

void PhotoItemList::RenderTopAndSide(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::CStdList<PhotoItem>::Item* item = this->GetTop();

    while (item != IZ_NULL) {
        PhotoItem* photoItem = item->GetData();

        photoItem->RenderTopAndSide(device);

        item = item->GetNext();
    }
}

//////////////////////////////

PhotoItemManager PhotoItemManager::s_Instance;

PhotoItemManager& PhotoItemManager::Instance()
{
    return s_Instance;
}

PhotoItemManager::PhotoItemManager()
{
    m_Shader = IZ_NULL;
}

PhotoItemManager::~PhotoItemManager()
{
    SAFE_RELEASE(m_Shader);
}

IZ_BOOL PhotoItemManager::Init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT itemNum)
{
    // Compute item count per list.
    IZ_UINT itemNumPerList = itemNum / COUNTOF(m_PhotoItemList);
    itemNumPerList = (itemNumPerList == 0 ? itemNum : itemNumPerList);

    // Create photo items.
    for (IZ_UINT i = 0; i < COUNTOF(m_PhotoItemList); i++) {
        for (IZ_UINT n = 0; n < itemNumPerList; n++) {
            PhotoItem* item = PhotoItem::Create(allocator, device);

            // TODO
            // Set position and rotation.

            m_PhotoItemList[i].AddTail(item->GetListItem());
        }
        
        itemNum -= itemNumPerList;

        if (itemNum == 0) {
            break;
        }
    }

    // Create shader.
    izanagi::CFileInputStream in;
    VRETURN(in.Open("data/BasicShader.shd"));

    m_Shader = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
                allocator,
                device,
                &in);
    VRETURN(m_Shader != IZ_NULL);

    return IZ_TRUE;
}
