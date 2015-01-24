#if !defined(__ITEM_BOX_H__)
#define __ITEM_BOX_H__

#include "izSampleKit.h"
#include "MeshVtx.h"

class ItemBox : public izanagi::CDebugMeshTmpl<SMeshVtx> {
    friend class izanagi::CDebugMeshTmpl<SMeshVtx>;

public:
    // Create an instance.
    static ItemBox* Create(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        IZ_UINT flag,
        IZ_COLOR color,
        IZ_FLOAT width,
        IZ_FLOAT height,
        IZ_FLOAT depth);

private:
    ItemBox() {}
    virtual ~ItemBox() {}

private:
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        IZ_UINT flag);

    IZ_BOOL CreateVB(
        izanagi::graph::CGraphicsDevice* device,
        IZ_UINT flag,
        IZ_UINT vtxNum);

    IZ_BOOL CreateVD(
        izanagi::graph::CGraphicsDevice* device,
        IZ_UINT flag);

    virtual IZ_UINT8* SetExtraVtxData(
        const void* sVtx,
        IZ_UINT flag,
        IZ_UINT8* data);

    IZ_BOOL SetData(
        izanagi::graph::CGraphicsDevice* device,
        IZ_UINT flag,
        IZ_COLOR color,
        IZ_FLOAT width,
        IZ_FLOAT height,
        IZ_FLOAT depth);

    IZ_BOOL SetIdx();
};

#endif    // #if !defined(__ITEM_BOX_H__)
