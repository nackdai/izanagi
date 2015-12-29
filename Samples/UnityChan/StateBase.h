#if !defined(__STATE_BASE_H__)
#define __STATE_BASE_H__

#include "izSceneGraph.h"
#include "izDebugUtil.h"

namespace izanagi {
    namespace sample {
        class CSampleApp;
        class CSampleMdlRenderHandler;
    }
}

class CStateBase : public izanagi::CSceneStateBase {
public:
    CStateBase(izanagi::sample::CSampleApp* app);
    virtual ~CStateBase() {}

public:
    // 初期化.
    virtual IZ_BOOL Init();

    // 更新.
    virtual IZ_BOOL Update(
        IZ_FLOAT time,
        izanagi::graph::CGraphicsDevice* device);

    // 終了.
    virtual IZ_BOOL Destroy();

    virtual IZ_BOOL Render(izanagi::graph::CGraphicsDevice* device);

    virtual IZ_BOOL EnableBgColor() const { return IZ_FALSE; }
    virtual IZ_COLOR GetBgColor() const { return 0xffffffff; }

protected:
    virtual void OnUpdate(
        IZ_FLOAT time,
        izanagi::graph::CGraphicsDevice* device) = 0;

    virtual IZ_BOOL CreateMaterial(izanagi::IMemoryAllocator* allocator) = 0;

    IZ_BOOL InitObject(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        IZ_FLOAT cameraZ,
        IZ_PCSTR imgPath,
        IZ_PCSTR mshPath,
        IZ_PCSTR sklPath,
        IZ_PCSTR shdPath);

    void ReleaseObject();

    void RenderName(
        izanagi::graph::CGraphicsDevice* device,
        const char* name);

    virtual IZ_UINT GetTexIdx() const = 0;
    virtual IZ_UINT GetShaderPassIdx() const = 0;

protected:
    izanagi::sample::CSampleApp* m_App;

    izanagi::CImage* m_Img;
    izanagi::CModel* m_Mdl;
    izanagi::CMesh* m_Msh;
    izanagi::CSkeleton* m_Skl;
    izanagi::shader::CShaderBasic* m_Shd;

    izanagi::CMaterial* m_Mtrl[9];

    izanagi::SParallelLightParam m_ParallelLight;

    izanagi::CRenderGraph* m_RenderGraph;
    izanagi::CSceneRenderer* m_Renderer;

    izanagi::math::SMatrix44 m_L2W;

    izanagi::sample::CSampleMdlRenderHandler* m_MdlRenderHandler;
};

#endif    // #if !defined(__MODEL_APP_H__)
