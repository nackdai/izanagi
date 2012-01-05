#if !defined(__STATE_STATE_0_H__)
#define __STATE_STATE_0_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izSceneGraph.h"

class CStateTest0 : public izanagi::CSceneStateBase {
public:
	CStateTest0();
	~CStateTest0();

public:
	IZ_BOOL Init();
	IZ_BOOL Render(izanagi::CGraphicsDevice* device);
	IZ_BOOL Update();
	IZ_BOOL Destroy();
	IZ_BOOL Enter(izanagi::IMemoryAllocator* allocator, void* val);
	IZ_BOOL Leave();

	IZ_BOOL OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags) { return IZ_TRUE; }
	IZ_BOOL OnMouseMove(IZ_UINT nFlags, IZ_INT x, IZ_INT y) { return IZ_TRUE; }
	IZ_BOOL OnMouseWheel(IZ_UINT nFlags, IZ_SHORT zDelta, IZ_INT x, IZ_INT y) { return IZ_TRUE; }
};

#endif	// #if !defined(__STATE_STATE_0_H__)
