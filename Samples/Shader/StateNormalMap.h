#if !defined(__STATE_NORMAL_MAP_H__)
#define __STATE_NORMAL_MAP_H__

#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "StateBase.h"

class CStateNormalMap : public CStateBase {
public:
    CStateNormalMap(
        izanagi::sample::CSampleApp* app,
        izanagi::SCameraParam& camera);
    virtual ~CStateNormalMap();

public:
    // 描画.
    virtual IZ_BOOL Render(izanagi::graph::CGraphicsDevice* device);

    // 開始
    virtual IZ_BOOL Enter(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::CValue& arg);

    // ステートから抜ける（終了）.
    virtual IZ_BOOL Leave();

protected:
    izanagi::shader::CShaderBasic* m_Shader;

    izanagi::CDebugMeshAxis* m_Axis;
    izanagi::CDebugMeshRectangle* m_Plane;

    izanagi::CImage* m_Image;

    izanagi::math::SMatrix44 m_L2W;

    izanagi::SParallelLightParam m_ParallelLight;
    izanagi::SAmbientLightParam m_Ambient;
    izanagi::SMaterialParam m_Mtrl;
};

#endif    // #if !defined(__STATE_NORMAL_MAP_H__)
