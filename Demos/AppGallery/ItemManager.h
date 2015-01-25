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

    void Update(
        IZ_FLOAT time,
        izanagi::graph::CGraphicsDevice* device,
        const izanagi::CCamera& camera);

    void RenderBox(
        izanagi::graph::CGraphicsDevice* device,
        izanagi::shader::CShaderBasic* shader,
        const izanagi::CCamera& camera);

    void RenderBoard(
        izanagi::graph::CGraphicsDevice* device,
        izanagi::shader::CShaderBasic* shader);

    /** Set angle rate for rotating photo items.
     */
    void SetAngleRate(IZ_FLOAT angle);

    /** Set angle to stop rotation immediately.
     */
    void SetAngleForImmediateRot(IZ_FLOAT angle);

    /** Get if rotation animation is running. 
     */
    IZ_BOOL IsRotateAnimating();

private:
    Item* FindNotRequestedLoadTexture();

private:
    ItemBox* m_BoxMesh;
    ItemBoard* m_BoardMesh;

    izanagi::CStdList<Item> m_Items;

    IZ_FLOAT m_AngleRate;
    izanagi::math::SMatrix44 m_mtxRot;

    IZ_FLOAT m_AngleImmediately;
};

#endif    // #if !defined(__ITEM_MANAGER_H__)
