#if !defined(__POINT_SPRITE_APP_H__)
#define __POINT_SPRITE_APP_H__

#include "izSampleKit.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class PointSpriteApp : public izanagi::sample::CSampleApp {
public:
    PointSpriteApp();
    virtual ~PointSpriteApp();

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

    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key) override;

private:
    static const IZ_UINT POINT_NUM = 1;

    struct Vertex {
        IZ_FLOAT pos[4];
        IZ_COLOR color;
    };

    izanagi::graph::CVertexBuffer* m_vb{ nullptr };
    izanagi::graph::CVertexDeclaration* m_vd{ nullptr };

    izanagi::graph::CVertexShader* m_vs{ nullptr };
    izanagi::graph::CPixelShader* m_ps{ nullptr };

    izanagi::graph::CShaderProgram* m_shd{ nullptr };
};

#endif    // #if !defined(__POINT_SPRITE_APP_H__)