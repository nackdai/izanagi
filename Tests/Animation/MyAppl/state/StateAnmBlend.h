#if !defined(__STATE_ANM_BLEND_H__)
#define __STATE_ANM_BLEND_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "izShader.h"

class CStateAnmBlend : public izanagi::CSceneStateBase {
public:
	CStateAnmBlend();
	~CStateAnmBlend();

public:
	IZ_BOOL Init();
	IZ_BOOL Render(izanagi::CGraphicsDevice* device);
	IZ_BOOL Update();
	IZ_BOOL Destroy();
	IZ_BOOL Enter(izanagi::IMemoryAllocator* allocator, void* val);
	IZ_BOOL Leave();

	IZ_BOOL OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags);

protected:
	void Render2D();
	void Render3D();

protected:
	izanagi::CAnimation* m_pAnm[2];
	izanagi::CAnmLinearBlender* m_AnmBlender;
	izanagi::CStdTimeline m_Timeline;

	IZ_BOOL m_IsBack;
};

#endif	// #if !defined(__STATE_ANM_BLEND_H__)
