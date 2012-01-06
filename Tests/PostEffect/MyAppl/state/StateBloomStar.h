#if !defined(__STATE_BLOOMSTAR_H__)
#define __STATE_BLOOMSTAR_H__

#include "izDefs.h"
#include "izStd.h"
#include "izSceneGraph.h"

class CStateBloomStar : public izanagi::CSceneStateBase {
public:
	CStateBloomStar() {}
	~CStateBloomStar() {}

public:
	IZ_BOOL Init();
	IZ_BOOL Render(izanagi::CGraphicsDevice* device);
	IZ_BOOL Update();
	IZ_BOOL Destroy();
	IZ_BOOL Enter(izanagi::IMemoryAllocator* allocator, void* val);
	IZ_BOOL Leave();

	IZ_BOOL OnKeyDown(IZ_UINT nChar);
	//IZ_BOOL OnMouseMove(IZ_INT x, IZ_INT y);
	//IZ_BOOL OnMouseWheel(IZ_SHORT zDelta); 

protected:
	void Render2D();
	void Render3D();

protected:
	IZ_BOOL m_bBack;
};

#endif	// #if !defined(__STATE_BLOOMSTAR_H__)
