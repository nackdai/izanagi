#if !defined(__IZANAGI_SCENE_GRAPH_SCENE_STATE_H__)
#define __IZANAGI_SCENE_GRAPH_SCENE_STATE_H__

#include "izDefs.h"
#include "izStd.h"

namespace izanagi {
	class CGraphicsDevice;

	/** ステートベース.
	 */
	class CSceneStateBase {
	public:
		CSceneStateBase() {}
		
		// 呼ばれないので、継承しても実装しないこと！！
		virtual ~CSceneStateBase() {}
		
		NO_COPIABLE(CSceneStateBase);
		
	public:
		/** 初期化.
		 */
		virtual IZ_BOOL Init();

		/** 描画.
		 */
		virtual IZ_BOOL Render(CGraphicsDevice* device);

		/** 更新.
		 */
		virtual IZ_BOOL Update();

		/** 終了.
		 */
		virtual IZ_BOOL Destroy();

		/** ステートに入る（開始）.
		 */
		virtual IZ_BOOL Enter(
			IMemoryAllocator* allocator,
			void* val = IZ_NULL);

		/** ステートから抜ける（終了）.
		 */
		virtual IZ_BOOL Leave();

		// For Windows

		/** キー押下
		 */
		virtual IZ_BOOL OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags);

		/** マウス移動.
		 */
		virtual IZ_BOOL OnMouseMove(IZ_UINT nFlags, IZ_INT x, IZ_INT y);

		/** マウスホイール操作.
		 */ 
		virtual IZ_BOOL OnMouseWheel(IZ_UINT nFlags, IZ_SHORT zDelta, IZ_INT x, IZ_INT y);
	};
}

#endif	// #if !defined(__IZANAGI_SCENE_GRAPH_SCENE_STATE_H__)
