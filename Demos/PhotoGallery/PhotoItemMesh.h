#if !defined(__PHOTO_ITEM_MESH_H__)
#define __PHOTO_ITEM_MESH_H__

#include "izSampleKit.h"

/** Photos's seat.
 */
class PhotoItemMesh : public izanagi::CDebugMesh {
public:
    // Create an instance.
    static PhotoItemMesh* Create(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        IZ_FLOAT width,
        IZ_FLOAT height,
        IZ_FLOAT depth);

private:
    // Color for mesh.
    static const IZ_COLOR Color = IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff);

private:
    PhotoItemMesh();
    virtual ~PhotoItemMesh();

private:
    template <typename _T>
    _T* CreateFace(
        izanagi::IMemoryAllocator* allocator, 
        izanagi::graph::CGraphicsDevice* device,
        IZ_FLOAT width,
        IZ_FLOAT height,
        IZ_FLOAT depth);

    class FrontFace;
    class TopAndSideFaces;

private:
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        IZ_FLOAT width,
        IZ_FLOAT height,
        IZ_FLOAT depth);

public:
    /** Render a photo item.
     */
    void Render(izanagi::graph::CGraphicsDevice* device);

private:
    FrontFace* m_FrontFace;
    TopAndSideFaces* m_TopAndSideFaces;
};

#endif    // #if !defined(__PHOTO_ITEM_MESH_H__)