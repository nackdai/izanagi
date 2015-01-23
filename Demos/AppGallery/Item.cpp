#include "Item.h"
#include "Board.h"
#include "Utility.h"

Item* Item::Create(
    izanagi::IMemoryAllocator* allocator,
    Board* board)
{
    void* buf = ALLOC_ZERO(allocator, sizeof(Item));
    VRETURN_NULL(buf != IZ_NULL);

    Item* instance = new(buf) Item;
    {
        instance->AddRef();

        instance->m_Allocator = allocator;
        SAFE_REPLACE(instance->m_Board, board);
    }

    return instance;
}

Item::Item()
{
    m_Allocator = IZ_NULL;
    m_Board = IZ_NULL;

    m_ListItem.Init(this);
}

Item::~Item()
{
    SAFE_RELEASE(m_Board);

    FREE(m_Allocator, this);
}

void Item::SetPosAndRotate(
    izanagi::math::SVector4& pos,
    IZ_FLOAT angle)
{
    izanagi::math::SMatrix44::GetRotByY(m_L2W, angle);
    izanagi::math::SMatrix44::Trans(m_L2W, m_L2W, pos);
}

void Item::Render(
    izanagi::graph::CGraphicsDevice* device,
    izanagi::shader::CShaderBasic* shader)
{
    Utility::SetShaderParam(
        shader,
        "g_mL2W",
        (void*)&m_L2W,
        sizeof(m_L2W));

    shader->CommitChanges(device);

    m_Board->Draw(device);
}

izanagi::CStdList<Item>::Item* Item::GetListItem()
{
    return &m_ListItem;
}

 const izanagi::math::SMatrix44& Item::GetL2W()
 {
     return m_L2W;
 }
