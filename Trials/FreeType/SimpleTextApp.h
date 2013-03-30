#if !defined(__SIMPLE_TEXT_APP_H__)
#define __SIMPLE_TEXT_APP_H__

#include "izSampleKit.h"


class CSimpleTextApp : public izanagi::sample::CSampleApp {
public:
    CSimpleTextApp();
    virtual ~CSimpleTextApp();

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
    izanagi::graph::CTexture* m_Tex;
};

#endif    // #if !defined(__FREETYPE_APP_H__)
