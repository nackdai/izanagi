#if !defined(__STATE_STATE_1_H__)
#define __STATE_STATE_1_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izSceneGraph.h"
#include "izIo.h"

namespace izanagi {
	class CArchiveProxy;
}

class CStateTest1 : public izanagi::CSceneStateBase {
public:
	CStateTest1();
	~CStateTest1();

public:
	IZ_BOOL Init();
	IZ_BOOL Render(izanagi::graph::CGraphicsDevice* device);
	IZ_BOOL Update();
	IZ_BOOL Destroy();
	IZ_BOOL Enter(izanagi::IMemoryAllocator* allocator, void* val);
	IZ_BOOL Leave();

	IZ_BOOL OnKeyDown(IZ_UINT nChar) { return IZ_TRUE; }
	IZ_BOOL OnMouseMove(IZ_INT x, IZ_INT y) { return IZ_TRUE; }
	IZ_BOOL OnMouseWheel(IZ_SHORT zDelta) { return IZ_TRUE; }

private:
	izanagi::CFileInputStream m_Input;
	izanagi::CArchiveProxy* m_pArchProxy;
};

#endif	// #if !defined(__STATE_STATE_1_H__)
