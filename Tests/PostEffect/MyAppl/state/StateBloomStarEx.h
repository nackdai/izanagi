#if !defined(__STATE_BLOOMSTAR_EX_H__)
#define __STATE_BLOOMSTAR_EX_H__

#include "izDefs.h"
#include "izStd.h"
#include "izSceneGraph.h"

class CStateBloomStarEx : public izanagi::CSceneStateBase {
public:
	CStateBloomStarEx() {}
	~CStateBloomStarEx() {}

public:
	IZ_BOOL Init();
	IZ_BOOL Render(izanagi::CGraphicsDevice* device);
	IZ_BOOL Update();
	IZ_BOOL Destroy();
	IZ_BOOL Enter(izanagi::IMemoryAllocator* allocator, void* val);
	IZ_BOOL Leave();

	IZ_BOOL OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags);
	//IZ_BOOL OnMouseMove(IZ_UINT nFlags, IZ_INT x, IZ_INT y);
	//IZ_BOOL OnMouseWheel(IZ_UINT nFlags, IZ_SHORT zDelta, IZ_INT x, IZ_INT y); 

protected:
	void Render2D();
	void Render3D();

protected:
	IZ_BOOL m_bBack;
};

#endif	// #if !defined(__STATE_BLOOMSTAR_EX_H__)
