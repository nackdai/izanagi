#if !defined(__STATE_ANM_INTERP_H__)
#define __STATE_ANM_INTERP_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "izShader.h"

class CStateAnmInterp : public izanagi::CGameState {
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
	IZ_BOOL Create();
	IZ_BOOL Render();
	IZ_BOOL Update();
	IZ_BOOL Destroy();
	IZ_BOOL Enter();
	IZ_BOOL Leave();

	IZ_BOOL OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags);
	IZ_BOOL OnMouseMove(IZ_UINT nFlags, IZ_INT x, IZ_INT y);
	IZ_BOOL OnMouseWheel(IZ_UINT nFlags, IZ_SHORT zDelta, IZ_INT x, IZ_INT y); 

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
};

#endif	// #if !defined(__STATE_ANM_LIST_H__)
