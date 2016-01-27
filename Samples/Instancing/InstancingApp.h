#if !defined(__INSTANCING_APP_H__)
#define __INSTANCING_APP_H__

#include "izSampleKit.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class CInstancingApp : public izanagi::sample::CSampleApp {
public:
    CInstancingApp();
    virtual ~CInstancingApp();

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
    static const IZ_UINT MeshNum = 10000;

    izanagi::CDebugMesh* m_Mesh;

    izanagi::graph::CVertexBuffer* m_InstancingVB;
    izanagi::graph::CVertexDeclaration* m_InstancingVD;

    izanagi::CImage* m_Img;

    izanagi::shader::CShaderBasic* m_Shader;

    IZ_BOOL m_enableInstaning{ IZ_TRUE };
};

#endif    // #if !defined(__INSTANCING_APP_H__)