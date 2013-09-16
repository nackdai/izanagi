#if !defined(__RESOURCE_MANAGER_APP_H__)
#define __RESOURCE_MANAGER_APP_H__

#include "izSampleKit.h"
#include "izResource.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class CResourceManagerApp : public izanagi::sample::CSampleApp {
public:
    CResourceManagerApp();
    virtual ~CResourceManagerApp();

protected:
    // 初期化.
    virtual IZ_BOOL InitInternal(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::sample::CSampleCamera& camera);

    // 解放.
    virtual void ReleaseInternal();

    // 更新.
    virtual void UpdateInternal(izanagi::graph::CGraphicsDevice* device);

    // 描画.
    virtual void RenderInternal(izanagi::graph::CGraphicsDevice* device);

private:
    izanagi::resource::CResourceManager* m_RscMgr;
};

#endif    // #if !defined(__RESOURCE_MANAGER_APP_H__)
