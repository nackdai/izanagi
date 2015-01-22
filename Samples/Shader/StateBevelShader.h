#if !defined(__STATE_BEVEL_SHADER_H__)
#define __STATE_BEVEL_SHADER_H__

#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "StateBase.h"
#include "BevelShaderMesh.h"

class CStateBevelShader : public CStateBase {
public:
    CStateBevelShader(
        izanagi::sample::CSampleApp* app,
        izanagi::SCameraParam& camera);
    virtual ~CStateBevelShader();

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
    BevelShaderMesh* m_Mesh;
};

#endif    // #if !defined(__STATE_BEVEL_SHADER_H__)
