#if !defined(__IZANAGI_SYSTEM_KEYBOARD_H__)
#define __IZANAGI_SYSTEM_KEYBOARD_H__

#include "izStd.h"
#include "system/SysKeyDefs.h"

namespace izanagi
{
namespace sys
{
    /** キーボード
     */
    class CKeyboard
    {
        friend class CSysWindow;
        friend class InputManager;
        
    public:
        CKeyboard()
        {
            FILL_ZERO(m_State, sizeof(m_State));
        }
        ~CKeyboard() {}

        NO_COPIABLE(CKeyboard);

    public:
        void PressKey(E_KEYBOARD_BUTTON key)
        {
			if (key < E_KEYBOARD_BUTTON_NUM) {
				m_State[1 - m_Pos][key] = 1;
			}
        }
        void ReleaseKey(E_KEYBOARD_BUTTON key)
        {
			if (key < E_KEYBOARD_BUTTON_NUM) {
				m_State[1 - m_Pos][key] = 0;
			}
        }

        void Update()
        {
            // flip.
            m_Pos = 1 - m_Pos;
        }

        // キーを押し続けているか.
        IZ_BOOL IsPressKey(E_KEYBOARD_BUTTON key)
        {
            IZ_BOOL ret = (m_State[m_Pos][key] > 0);
            return ret;
        }

        // キーを一度だけ押したかどうか.
        IZ_BOOL IsPressOneShotKey(E_KEYBOARD_BUTTON key)
        {
            IZ_BOOL ret = ((m_State[m_Pos][key] > 0)
                && (m_State[1 - m_Pos][key] == 0));
            return ret;
        }

        // キーが離されたかどうか.
        IZ_BOOL IsReleaseKey(E_KEYBOARD_BUTTON key)
        {
            IZ_BOOL ret = ((m_State[m_Pos][key] == 0)
                && (m_State[1 - m_Pos][key] > 0));
            return ret;
        }

    private:
        CStdHash<IZ_UINT64, CKeyboard, 4>::Item* GetHashItem() { return &m_item; }

    protected:
        IZ_UINT m_Pos{ 0 };

        // キーバッファ
        IZ_UINT8 m_State[2][E_KEYBOARD_BUTTON_NUM];

        CStdHash<IZ_UINT64, CKeyboard, 4>::Item m_item;
    };
}   // namespace sys
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SYSTEM_KEYBOARD_H__)
