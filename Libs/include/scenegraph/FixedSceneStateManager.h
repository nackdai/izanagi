#if !defined(__IZANAGI_SCENE_GRAPH_FIXED_SCENE_STATE_MANAGER_H__)
#define __IZANAGI_SCENE_GRAPH_FIXED_SCENE_STATE_MANAGER_H__

#include "izDefs.h"
#include "izGraph.h"
#include "izSystem.h"
#include "SceneState.h"

namespace izanagi {

    /** 固定ステート管理.
     */
    template <
        typename STATE,
        IZ_UINT STATE_NUM
    >
    class CFixedSceneStateManager {
    private:
        enum {
            StateNumInternal = STATE_NUM,
        };

    public:
        CFixedSceneStateManager()
        {
            FILL_ZERO(m_pStateTable, sizeof(m_pStateTable));

            m_nNextState = (STATE)STATE_NUM;
            m_nPrevState = (STATE)STATE_NUM;
            m_nCurrentState = (STATE)STATE_NUM;

            m_EnterArg = IZ_NULL;
        }

        ~CFixedSceneStateManager() {}

        NO_COPIABLE(CFixedSceneStateManager);

    public:
        /** 登録
         */
        IZ_BOOL Register(IZ_UINT idx, CSceneStateBase* state)
        {
            VRETURN(
                idx < StateNumInternal
                && m_pStateTable[idx] == IZ_NULL);

            m_pStateTable[idx] = state;

            return IZ_TRUE;
        }

        /** 初期化.
         */
        IZ_BOOL Init()
        {
            for (IZ_INT i = 0; i < STATE_NUM; ++i) {
                if (!m_pStateTable[i]->Init()) {
                    IZ_ASSERT(IZ_FALSE);
                    return IZ_FALSE;
                }
            }
            
            // 開始タスクを実行
            ChangeState((STATE)0);
            
            return IZ_TRUE;
        }

        /** 描画.
         */
        IZ_BOOL Render(graph::CGraphicsDevice* device)
        {
            if (GetCurrentState() == STATE_NUM) {
                return IZ_TRUE;
            }

            // 描画
            IZ_BOOL bRet = GetState(GetCurrentState())->Render(device);
            
            return bRet;
        }

        /** 更新.
         */
        IZ_BOOL Update(IMemoryAllocator* allocator, void* args = IZ_NULL)
        {       
            if (m_nNextState != STATE_NUM && GetCurrentState() != m_nNextState) {
                // 次のタスクへ移行
                if (GetCurrentState() != STATE_NUM) {
                    // 現在のタスクの後始末
                    if (!GetState(GetCurrentState())->Leave()) {
                        IZ_ASSERT(IZ_FALSE);
                        return IZ_FALSE;
                    }
                }
                    
                // 次のタスクの初期化
                m_nPrevState = GetCurrentState();
                if (! GetState(m_nNextState)->Enter(allocator, args != IZ_NULL ? CValue(args) : m_EnterArg)) {
                    // まだ入れないって意味にする
                    m_EnterArg = IZ_NULL;
                    return IZ_TRUE;
                }

                m_EnterArg = IZ_NULL;
            
                SetCurrentState(m_nNextState);
                m_nNextState = (STATE)STATE_NUM;        
            }
            
            IZ_BOOL bRet = GetState(GetCurrentState())->Update();
            
            return bRet;
        }

        /** 終了.
         */
        IZ_BOOL Destroy()
        {
            // ねんのため 
            GetState(GetCurrentState())->Leave();

            for (IZ_INT i = STATE_NUM - 1; i >= 0; --i) {
                if (!GetState((STATE)i)->Destroy()) {
                    IZ_ASSERT(IZ_FALSE);
                    return IZ_FALSE;
                }
            }

            return IZ_TRUE;
    }

        // For Windows

        /** キー押下.
         */
        IZ_BOOL OnKeyDown(sys::E_KEYBOARD_BUTTON key)
        {
            return GetState(GetCurrentState())->OnKeyDown(key);
        }

        /** マウス移動.
         */
        IZ_BOOL OnMouseMove(IZ_INT x, IZ_INT y)
        {
            return GetState(GetCurrentState())->OnMouseMove(x, y);
        }

        /** マウスホイール操作.
         */
        IZ_BOOL OnMouseWheel(IZ_SHORT delta)
        {
            return GetState(GetCurrentState())->OnMouseWheel(delta);
        }

    public:
        /** ステート変更.
         */
        void ChangeState(STATE task, CValue enterArg)
        {
            IZ_ASSERT((task >= (STATE)0) && (task < STATE_NUM));
            
            m_nNextState = task;
            m_EnterArg = enterArg;
        }

        void ChangeState(STATE task)
        {
            ChangeState(task, CValue());
        }

        /** 前のステートに変更.
         */
        void ChangePrevState()
        {
            if (m_nPrevState != STATE_NUM) {
                ChangeState(m_nPrevState);
            }
        }

        /** ステート取得.
         */
        CSceneStateBase* GetState(STATE task)
        {
            IZ_ASSERT((task >= (STATE)0) && (task < STATE_NUM));
            return m_pStateTable[task];
        }
        
        /** 現在のステートかどうか.
         */ 
        IZ_BOOL IsCurrentState(STATE task)
        {
            return (m_nCurrentState == task);
        }

        /** 現在のステートかどうか.
         */ 
        IZ_BOOL IsCurrentState(CSceneStateBase* task)
        {
            CSceneStateBase* current_task = GetState(m_nCurrentState);
            return (current_task == task);
        }

        /** ステート数を取得.
         */
        IZ_UINT GetStateNum()
        {
            return StateNumInternal;
        }

        /** 現在のステートを取得.
         */
        STATE GetCurrentState()
        {
            return m_nCurrentState;
        }

        /** 前のステートを取得
         */
        STATE GetPrevState()
        {
            return m_nPrevState;
        }
        
    private:
        void SetCurrentState(STATE task) { m_nCurrentState = task; }

    protected:
        CSceneStateBase* m_pStateTable[STATE_NUM];
    
        STATE m_nCurrentState;  // 現在のタスク
        STATE m_nNextState;     // 次に遷移するタスク
        STATE m_nPrevState;     // 遷移前のタスク

        CValue m_EnterArg;
    };
}

#endif  // #if !defined(__IZANAGI_SCENE_GRAPH_FIXED_SCENE_STATE_MANAGER_H__)
