#if !defined(__CAPTURE_APP_H__)
#define __CAPTURE_APP_H__

#include "izSampleKit.h"
#include "izThreadModel.h"

#include "FrameCapture.h"
#include "FBOManager.h"
#include "FBOManager2.h"
#include "CaptureManager.h"
#include "CaptureManager2.h"

#include "DxtEncoder.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class CaptureApp : public izanagi::sample::CSampleApp {
public:
    CaptureApp();
    virtual ~CaptureApp();

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
    static const IZ_UINT MAX_MESH_NUM = 1;

    izanagi::CDebugMesh* m_Mesh;

    izanagi::CImage* m_Img;

    izanagi::shader::CShaderBasic* m_Shader;

    IZ_UINT m_curNum{ MAX_MESH_NUM };

    struct ObjectInfo {
        izanagi::math::SMatrix44 mtxL2W;
        izanagi::math::SVector4 points[8];

        IZ_FLOAT point2D[2][2];
    };

    ObjectInfo m_objects[MAX_MESH_NUM];

    // Clip -> Screen
    izanagi::math::SMatrix44 m_mtxC2S;

    izanagi::threadmodel::CThreadPool m_theadPool;

    izanagi::IMemoryAllocator* m_allocator{ nullptr };

    IZ_BOOL m_enabled2DRender{ IZ_FALSE };

    FrameCapture m_frameCapture;
    CaptureManager2 m_fboManager;

    DxtEncoder m_dxtEncoder;
};

#endif    // #if !defined(__CAPTURE_APP_H__)