#if !defined(__PHOTO_ITEM_H__)
#define __PHOTO_ITEM_H__

#include "izSampleKit.h"
#include "izAnimation.h"

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

    void Update(
        IZ_FLOAT time,
        izanagi::graph::CGraphicsDevice* device,
        const izanagi::math::SMatrix44& mtxRot,
        const izanagi::CCamera& camera);

    izanagi::CStdList<PhotoItem>::Item* GetListItem();

    /** Set texture for rendering.
     */
    void SetTexture(
        izanagi::graph::CTexture* texture,
        const char* path);

    /** Get if photo item has texture.
     */
    IZ_BOOL HasTexture();

    /** Set position and rotation.
     */
    void SetPositionAndRotation(
        const izanagi::math::CVector4& pos,
        IZ_FLOAT rot);

    /** Get local to world matrix.
     */
    const izanagi::math::SMatrix44& GetL2W() const;

    PhotoItemMesh* GetMesh();

    /** Check if ray hits photo item.
     */
    IZ_BOOL HitTest(
        const izanagi::math::CRay& ray,
        const izanagi::math::SMatrix44& mtxRot);

    /** Set flag if loading request is requested for photo item.
     */
    void SetIsRequestedLoadTexture(IZ_BOOL flag);

    /** Get if loading request is requested for photo item.
     */
    IZ_BOOL IsRequestedLoadTexture() const;

    /** Get normal vector of photo item.
     */
    void GetNormal(izanagi::math::SVector4& nml);

    /** Get center positon of photo item.
     */
    void GetCenterPosition(izanagi::math::SVector4& pos);

    /** Set parameter for shader.
     */
    void SetShaderParam(izanagi::shader::CShaderBasic* shader);

    /** Get if photo item is shown.
     */
    IZ_BOOL IsShown() const;

private:
    void ResetFadeInParams();

private:
    IZ_UINT m_ID;

    izanagi::IMemoryAllocator* m_Allocator;

    izanagi::CStdList<PhotoItem>::Item m_ListItem;

    izanagi::graph::CTexture* m_Texture;
    const char* m_Path;

    PhotoItemMesh* m_Mesh;

    izanagi::math::SMatrix44 m_L2W;

    izanagi::math::CRectangle m_Rectangle;

    IZ_BOOL m_IsRequestedLoadTexture;

    IZ_BOOL m_IsFading;
    izanagi::animation::CTimeline m_Timeline;
    IZ_FLOAT m_FadeInAlpha;
    IZ_FLOAT m_FadeInHeight;

    IZ_BOOL m_IsShown;
};

#endif    // #if !defined(__PHOTO_ITEM_H__)