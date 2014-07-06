#if !defined(__IZANAGI_SCENE_GRAPH_SCENE_STATE_H__)
#define __IZANAGI_SCENE_GRAPH_SCENE_STATE_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izSystem.h"

namespace izanagi {
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
        virtual IZ_BOOL Render(graph::CGraphicsDevice* device);

        /** 更新.
         */
        virtual IZ_BOOL Update(
            IZ_FLOAT time,
            graph::CGraphicsDevice* device);

        /** 終了.
         */
        virtual IZ_BOOL Destroy();

        /** ステートに入る（開始）.
         */
        virtual IZ_BOOL Enter(
            IMemoryAllocator* allocator,
            graph::CGraphicsDevice* device,
            CValue& arg);

        /** ステートから抜ける（終了）.
         */
        virtual IZ_BOOL Leave();

        // For Windows

        /** キー押下
         */
        virtual IZ_BOOL OnKeyDown(sys::E_KEYBOARD_BUTTON key);

        virtual IZ_BOOL OnMouseLBtnDown(const izanagi::CIntPoint& point);

        virtual IZ_BOOL OnMouseLBtnUp(const izanagi::CIntPoint& point);

        /** マウス移動.
         */
        virtual void OnMouseMove(const izanagi::CIntPoint& point);

        /** マウスホイール操作.
         */ 
        virtual IZ_BOOL OnMouseWheel(IZ_SHORT delta);
    };
}

#endif  // #if !defined(__IZANAGI_SCENE_GRAPH_SCENE_STATE_H__)
