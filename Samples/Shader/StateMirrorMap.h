#if !defined(__STATE_MIRROR_MAP_H__)
#define __STATE_MIRROR_MAP_H__

#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "StateBase.h"

class CStateMirrorMap : public CStateBase {
public:
    CStateMirrorMap(
        izanagi::sample::CSampleApp* app,
        izanagi::SCameraParam& camera);
    virtual ~CStateMirrorMap();

public:
    // 描画.
    virtual IZ_BOOL Render(izanagi::graph::CGraphicsDevice* device);

    // 開始
    virtual IZ_BOOL Enter(
        izanagi::IMemoryAllocator* allocator,
        izanagi::CValue& arg);

    // ステートから抜ける（終了）.
    virtual IZ_BOOL Leave();

protected:
    izanagi::CImage* m_Img;
    izanagi::shader::CShaderBasic* m_Shader;
    izanagi::CDebugMeshSphere* m_Sphere;
};

#endif    // #if !defined(__STATE_MIRROR_MAP_H__)
