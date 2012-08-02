#if !defined(__STATE_MONOTONE_H__)
#define __STATE_MONOTONE_H__

#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "StateBase.h"

class CStateMonotone : public CStateBase {
public:
	CStateMonotone(
		izanagi::sample::CSampleApp* app,
		izanagi::SCameraParam& camera);
	virtual ~CStateMonotone();

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
};

#endif	// #if !defined(__STATE_PHONG_SHADER_H__)
