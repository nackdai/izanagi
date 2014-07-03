#if !defined(__PHOTO_ITEM_H__)
#define __PHOTO_ITEM_H__

#include "izSampleKit.h"

class PhotoItemMesh;

/** Photo item.
 */
class PhotoItem : public izanagi::CObject {
public:
    /** Create an instance.
     */
    static PhotoItem* Create(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        PhotoItemMesh* mesh);

private:
    PhotoItem();
    virtual ~PhotoItem();

    NO_COPIABLE(PhotoItem);

    IZ_DEFINE_INTERNAL_RELEASE();

private:
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        PhotoItemMesh* mesh);

public:
    /** Render front face of photo item.
     */
    void RenderFront(izanagi::graph::CGraphicsDevice* device);

    /** Render top and side faces of photo item.
     */
    void RenderTopAndSide(izanagi::graph::CGraphicsDevice* device);

    izanagi::CStdList<PhotoItem>::Item* GetListItem();

    /** Set texture for rendering.
     */
    void SetTexture(izanagi::graph::CTexture* texture);

    /** Get if photo item has texture.
     */
    IZ_BOOL HasTexture();

    /** Set position and rotation.
     */
    void SetPositionAndRotation(
        const izanagi::math::CVector& pos,
        IZ_FLOAT rot);

    const izanagi::math::SMatrix& GetL2W();

    PhotoItemMesh* GetMesh();

    void SetIsRequestedLoadTexture(IZ_BOOL flag);
    IZ_BOOL IsRequestedLoadTexture();

private:
    izanagi::IMemoryAllocator* m_Allocator;

    izanagi::CStdList<PhotoItem>::Item m_ListItem;

    PhotoItemMesh* m_Mesh;

    izanagi::math::SMatrix m_L2W;

    IZ_BOOL m_IsRequestedLoadTexture;
};

#endif    // #if !defined(__PHOTO_ITEM_H__)