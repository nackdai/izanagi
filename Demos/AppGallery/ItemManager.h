#if !defined(__ITEM_MANAGER_H__)
#define __ITEM_MANAGER_H__

#include "izSampleKit.h"

class Board;
class Item;

class ItemManager {
private:
    static ItemManager instance;

public:
    static ItemManager& Instance();

private:
    ItemManager();
    virtual ~ItemManager();

public:
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device);

    void Terminate();

    void Render(
        izanagi::graph::CGraphicsDevice* device,
        izanagi::shader::CShaderBasic* shader);

private:
    Board* m_BoardMesh;

    izanagi::CStdList<Item> m_Items;
};

#endif    // #if !defined(__ITEM_MANAGER_H__)
