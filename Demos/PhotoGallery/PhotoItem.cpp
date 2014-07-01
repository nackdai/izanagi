#include "PhotoItem.h"
#include "PhotoItemMesh.h"

PhotoItem* PhotoItem::Create(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device)
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
            device);
    }

    if (!result) {
        SAFE_RELEASE(instance);
    }

    return instance;
}

PhotoItem::PhotoItem()
{
    m_Allocator = IZ_NULL;
    m_Mesh = IZ_NULL;

    m_ListItem.Init(this);
}

PhotoItem::~PhotoItem()
{
    SAFE_RELEASE(m_Mesh);
}

IZ_BOOL PhotoItem::Init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device)
{
    m_Mesh = PhotoItemMesh::Create(
        allocator,
        device,
        10.0f, 10.0f, 2.0f);
    VRETURN(m_Mesh != IZ_NULL);

    return IZ_TRUE;
}

void PhotoItem::Render(izanagi::graph::CGraphicsDevice* device)
{
    IZ_ASSERT(device != IZ_NULL);

    IZ_ASSERT(m_Mesh != IZ_NULL);
    m_Mesh->Render(device);
}
