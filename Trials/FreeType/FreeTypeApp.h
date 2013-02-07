#if !defined(__FREETYPE_APP_H__)
#define __FREETYPE_APP_H__

#include <ft2build.h>
#include FT_FREETYPE_H

#include "izSampleKit.h"


class CFreeTypeApp : public izanagi::sample::CSampleApp {
public:
    CFreeTypeApp();
    virtual ~CFreeTypeApp();

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
    FT_Library  m_Library;
    FT_Face     m_Face;

    izanagi::graph::CTexture* m_Tex;
};

#endif    // #if !defined(__FREETYPE_APP_H__)
