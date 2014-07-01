#if !defined(__PHOTO_ITEM_H__)
#define __PHOTO_ITEM_H__

#include "izSampleKit.h"

class PhotoItemMesh;

class PhotoItem : public izanagi::CObject {
public:
    static PhotoItem* Create(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device);

private:
    PhotoItem();
    virtual ~PhotoItem();

    NO_COPIABLE(PhotoItem);

    IZ_DEFINE_INTERNAL_RELEASE();

private:
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device);

public:
    void Render(izanagi::graph::CGraphicsDevice* device);

public:
    izanagi::CStdList<PhotoItem>::Item* GetListItem();

private:
    izanagi::IMemoryAllocator* m_Allocator;

    izanagi::CStdList<PhotoItem>::Item m_ListItem;

    PhotoItemMesh* m_Mesh;
};

#endif    // #if !defined(__PHOTO_ITEM_H__)