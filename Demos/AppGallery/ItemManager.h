#if !defined(__ITEM_MANAGER_H__)
#define __ITEM_MANAGER_H__

#include "izSampleKit.h"

class ItemBox;
class ItemBoard;
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

    IZ_BOOL EnqueueLoadingRequest(
        izanagi::graph::CGraphicsDevice* device,
        const char* path);

    void RenderBox(
        izanagi::graph::CGraphicsDevice* device,
        izanagi::shader::CShaderBasic* shader,
        const izanagi::CCamera& camera);

    void RenderBoard(
        izanagi::graph::CGraphicsDevice* device,
        izanagi::shader::CShaderBasic* shader);

private:
    Item* FindNotRequestedLoadTexture();

private:
    ItemBox* m_BoxMesh;
    ItemBoard* m_BoardMesh;

    izanagi::CStdList<Item> m_Items;
};

#endif    // #if !defined(__ITEM_MANAGER_H__)
