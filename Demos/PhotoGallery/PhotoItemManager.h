#if !defined(__PHOTO_ITEM_MANAGER_H__)
#define __PHOTO_ITEM_MANAGER_H__

#include "izSampleKit.h"

class PhotoItem;

/** List of photo items per floor.
 */
class PhotoItemList : public izanagi::CStdList<PhotoItem> {
public:
    PhotoItemList();
    virtual ~PhotoItemList();

    NO_COPIABLE(PhotoItemList);

public:
    /** Render photo item with texture.
     */
    void RenderWithTexture(
        izanagi::graph::CGraphicsDevice* device,
        const izanagi::CCamera& camera,
        const izanagi::math::SMatrix44& mtxRot,
        izanagi::shader::CShaderBasic* shader);

    /** Render photo item without texture.
     */
    void RenderWithoutTexture(
        izanagi::graph::CGraphicsDevice* device,
        const izanagi::CCamera& camera,
        const izanagi::math::SMatrix44& mtxRot,
        izanagi::shader::CShaderBasic* shader);
};

/** Manager of photo item.
 */
class PhotoItemManager {
private:
    static PhotoItemManager s_Instance;

public:
    static PhotoItemManager& Instance();

private:
    PhotoItemManager();
    ~PhotoItemManager();

public:
    /** Initialize.
     */
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        IZ_UINT itemNum);

    /** Terminate.
     */
    void Terminate();

    /** Enqueue load texture request.
     */
    IZ_BOOL EnqueueLoadingRequest(
        izanagi::graph::CGraphicsDevice* device,
        const char* path);

    void Update(
        IZ_FLOAT time,
        izanagi::graph::CGraphicsDevice* device,
        const izanagi::CCamera& camera);

    void Render(
        izanagi::graph::CGraphicsDevice* device,
        const izanagi::CCamera& camera);

    /** Set angle rate for rotating photo items.
     */
    void SetAngleRate(IZ_FLOAT angle);

    /** Set angle to stop rotation immediately.
     */
    void SetAngleForImmediateRot(IZ_FLOAT angle);

    /** Get if rotation animation is running. 
     */
    IZ_BOOL IsRotateAnimating();

    /** Get photo item that is hit by ray.
     */
    PhotoItem* HitTest(const izanagi::math::CRay& ray);

    /** Get rotation matrix.
     */
    const izanagi::math::SMatrix44& GetRotationMtx() const;

private:
    PhotoItem* FindNotRequestedLoadTexture();

private:
    PhotoItemList m_PhotoItemList[3];

    IZ_FLOAT m_AngleRate;
    izanagi::math::SMatrix44 m_mtxRot;

    IZ_FLOAT m_AngleImmediately;

    izanagi::shader::CShaderBasic* m_Shader;
};

#endif    // #if !defined(__PHOTO_ITEM_MANAGER_H__)