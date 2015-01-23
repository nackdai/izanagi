#if !defined(__BOARD_H__)
#define __BOARD_H__

#include "izSampleKit.h"
#include "MeshVtx.h"

class Board : public izanagi::CDebugMeshTmpl<SMeshVtx> {
    friend class izanagi::CDebugMeshTmpl<SMeshVtx>;

public:
    // Create an instance.
    static Board* Create(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        IZ_UINT flag,
        IZ_COLOR color,
        IZ_FLOAT width,
        IZ_FLOAT height,
        IZ_FLOAT depth);

private:
    Board() {}
    virtual ~Board() {}

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

#endif    // #if !defined(__BOARD_H__)
