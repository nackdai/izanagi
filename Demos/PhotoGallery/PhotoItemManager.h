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
    void RenderWithTexture(
        izanagi::graph::CGraphicsDevice* device,
        const izanagi::math::SMatrix& mtxRot,
        izanagi::shader::CShaderBasic* shader);

    void RenderWithoutTexture(
        izanagi::graph::CGraphicsDevice* device,
        const izanagi::math::SMatrix& mtxRot,
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
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        IZ_UINT itemNum);

    void Terminate();

    IZ_BOOL EnqueueLoadingRequest(
        izanagi::graph::CGraphicsDevice* device,
        const char* path);

    void Update(IZ_FLOAT time);

    void Render(
        izanagi::graph::CGraphicsDevice* device,
        const izanagi::CCamera& camera);

    void SetAngleRate(IZ_FLOAT angle);

    IZ_BOOL IsRotateAnimating();

    PhotoItem* HitTest(const izanagi::math::CRay& ray);

    const izanagi::math::SMatrix& GetRotationMtx() const;

private:
    PhotoItem* FindNotRequestedLoadTexture();

private:
    PhotoItemList m_PhotoItemList[3];

    IZ_FLOAT m_AngleRate;
    izanagi::math::SMatrix m_mtxRot;

    izanagi::shader::CShaderBasic* m_Shader;
};

#endif    // #if !defined(__PHOTO_ITEM_MANAGER_H__)