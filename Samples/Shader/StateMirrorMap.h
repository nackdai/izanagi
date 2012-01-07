#if !defined(__STATE_MIRROR_MAP_H__)
#define __STATE_MIRROR_MAP_H__

#include "izSceneGraph.h"
#include "izDebugUtil.h"

class CStateMirrorMap : public izanagi::CSceneStateBase {
public:
	CStateMirrorMap(izanagi::SCameraParam& camera);
	virtual ~CStateMirrorMap();

public:
	// 初期化.
	virtual IZ_BOOL Init();

	// 更新.
	virtual IZ_BOOL Update();

	// 描画.
	virtual IZ_BOOL Render(izanagi::CGraphicsDevice* device);

	// 開始
	virtual IZ_BOOL Enter(
		izanagi::IMemoryAllocator* allocator,
		void* val);

	// 終了.
	virtual IZ_BOOL Destroy();

	// ステートから抜ける（終了）.
	virtual IZ_BOOL Leave();

	// For Windows

	// キー押下
	virtual IZ_BOOL OnKeyDown(IZ_UINT nChar);

protected:
	izanagi::CImage* m_Img;
	izanagi::CShaderBasic* m_Shader;
	izanagi::CDebugMeshSphere* m_Sphere;

	izanagi::SCameraParam& m_Camera;
};

#endif	// #if !defined(__STATE_MIRROR_MAP_H__)
