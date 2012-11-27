#if !defined(__STATE_HDR_2_H__)
#define __STATE_HDR_2_H__

#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "StatePostEffect.h"

class CStateHDR_2 : public CStatePostEffect {
public:
	CStateHDR_2(
		izanagi::sample::CSampleApp* app,
		izanagi::SCameraParam& camera);
	virtual ~CStateHDR_2();

public:
	// 描画.
	virtual IZ_BOOL Render(izanagi::graph::CGraphicsDevice* device);

	// 開始
	virtual IZ_BOOL Enter(
		izanagi::IMemoryAllocator* allocator,
		void* val);
};

#endif	// #if !defined(__STATE_HDR_2_H__)
