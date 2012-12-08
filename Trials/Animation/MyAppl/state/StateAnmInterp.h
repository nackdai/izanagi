#if !defined(__STATE_ANM_INTERP_H__)
#define __STATE_ANM_INTERP_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "izShader.h"
#include "izSceneGraph.h"

class CStateAnmInterp : public izanagi::CSceneStateBase {
	class CTimeOverHandler : public izanagi::CStdTimeline::CTimeOverHandler {
	public:
		CTimeOverHandler() { m_State = IZ_NULL; }
		virtual ~CTimeOverHandler() {}

	public:
		void Set(CStateAnmInterp* state) { m_State = state; }
		void Handle(const izanagi::CStdTimeline& timeline);

	private:
		CStateAnmInterp* m_State;
	};

public:
	CStateAnmInterp();
	~CStateAnmInterp();

public:
	IZ_BOOL Init();
	IZ_BOOL Render(izanagi::graph::CGraphicsDevice* device);
	IZ_BOOL Update();
	IZ_BOOL Destroy();
	IZ_BOOL Enter(izanagi::IMemoryAllocator* allocator, void* val);
	IZ_BOOL Leave();

	IZ_BOOL OnKeyDown(IZ_UINT nChar);
	IZ_BOOL OnMouseMove(IZ_INT x, IZ_INT y);
	IZ_BOOL OnMouseWheel(IZ_SHORT zDelta); 

protected:
	void Render2D();
	void Render3D();

	IZ_BOOL ReadAnm();

protected:
	izanagi::CAnimation* m_pAnm[2];
	izanagi::CAnimationInterp* m_AnmInterp;
	izanagi::CStdTimeline m_Timeline;

	CTimeOverHandler m_TimeOverHandler;

	izanagi::IAnimation* m_CurAnm;
	IZ_UINT m_TargetAnmIdx;

	IZ_BOOL m_IsBack;
};

#endif	// #if !defined(__STATE_ANM_LIST_H__)
