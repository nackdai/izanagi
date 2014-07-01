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
        IZ_UINT width,
        IZ_UINT height);

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
        IZ_UINT width,
        IZ_UINT height);

    class FrontFace;
    class SideFaces;

private:
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        IZ_UINT width,
        IZ_UINT height);

public:
    /** Render a photo item.
     */
    void Render(izanagi::graph::CGraphicsDevice* device);

private:
    FrontFace* m_FrontFace;
    SideFaces* m_SideFaces;
};

#endif    // #if !defined(__PHOTO_ITEM_MESH_H__)