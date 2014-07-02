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
    /** Render front face of photo item.
     */
    void RenderFront(izanagi::graph::CGraphicsDevice* device);

    /** Render top and side faces of photo item.
     */
    void RenderTopAndSide(izanagi::graph::CGraphicsDevice* device);

    izanagi::CStdList<PhotoItem>::Item* GetListItem();

    void SetTexture(izanagi::graph::CTexture* texture);

    /** Get if photo item has texture.
     */
    IZ_BOOL HasTexture();

private:
    izanagi::IMemoryAllocator* m_Allocator;

    izanagi::CStdList<PhotoItem>::Item m_ListItem;

    PhotoItemMesh* m_Mesh;
};

#endif    // #if !defined(__PHOTO_ITEM_H__)