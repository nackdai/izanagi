#if !defined(__STATE_ANM_LIST_H__)
#define __STATE_ANM_LIST_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "izShader.h"

class CStateAnmList : public izanagi::CGameState {
public:
	CStateAnmList();
	~CStateAnmList();

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
	izanagi::CAnimation* m_pAnm;
	izanagi::CStdTimeline m_Timeline;

	IZ_INT m_nCurAnmIdx;

	IZ_BOOL m_IsBack;
};

#endif	// #if !defined(__STATE_ANM_LIST_H__)
