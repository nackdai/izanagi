#if !defined(__PHOTO_ITEM_MESH_H__)
#define __PHOTO_ITEM_MESH_H__

#include "izSampleKit.h"

/** Mesh for photo item.
 */
class PhotoItemMesh : public izanagi::CDebugMesh {
    friend class PhotoItem;

public:
    // Create an instance.
    static PhotoItemMesh* Create(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        IZ_FLOAT width,
        IZ_FLOAT height,
        IZ_FLOAT depth);

private:
    PhotoItemMesh();
    virtual ~PhotoItemMesh();

private:
    template <typename _T>
    _T* CreateFace(
        izanagi::IMemoryAllocator* allocator, 
        izanagi::graph::CGraphicsDevice* device,
        IZ_UINT flags,
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
    /** Render front face of photo item.
     */
    void RenderFront(izanagi::graph::CGraphicsDevice* device);

    /** Render top and side faces of photo item.
     */
    void RenderTopAndSide(izanagi::graph::CGraphicsDevice* device);

private:
    void SetMaterialToShader(izanagi::shader::CShaderBasic* shader);

private:
    FrontFace* m_FrontFace;             //< front face which is mapped texture.
    TopAndSideFaces* m_TopAndSideFaces; //< top and side faces which are not mapped texture.

    izanagi::SMaterialParam m_Mtrl;
};

#endif    // #if !defined(__PHOTO_ITEM_MESH_H__)