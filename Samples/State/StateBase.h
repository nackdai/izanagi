#if !defined(__STATE_BASE_H__)
#define __STATE_BASE_H__

#include "izSceneGraph.h"

class CStateBase : public izanagi::CSceneStateBase {
public:
	CStateBase();
	virtual ~CStateBase();

public:
	// 初期化.
	virtual IZ_BOOL Init();

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

	virtual IZ_UINT GetIndex() = 0;

protected:
	izanagi::CImage* m_Img;

	IZ_BOOL m_IsBack;
};

#endif	// #if !defined(__STATE_BASE_H__)
