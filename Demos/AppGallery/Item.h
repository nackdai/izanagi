#if !defined(__ITEM_H__)
#define __ITEM_H__

#include "izSampleKit.h"

class Board;

class Item : public izanagi::CObject {
public:
    static Item* Create(
        izanagi::IMemoryAllocator* allocator,
        Board* board);

private:
    Item();
    virtual ~Item();

    IZ_DEFINE_INTERNAL_RELEASE();

public:
    void SetPosAndRotate(
        izanagi::math::SVector4& pos,
        IZ_FLOAT angle);

    void Render(
        izanagi::graph::CGraphicsDevice* device,
        izanagi::shader::CShaderBasic* shader);

    izanagi::CStdList<Item>::Item* GetListItem();

    const izanagi::math::SMatrix44& GetL2W();

private:
    izanagi::IMemoryAllocator* m_Allocator;

    Board* m_Board;

    izanagi::math::SMatrix44 m_L2W;

    izanagi::CStdList<Item>::Item m_ListItem;
};

#endif    // #if !defined(__ITEM_H__)
