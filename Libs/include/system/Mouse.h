#if !defined(__IZANAGI_SYSTEM_MOUSE_H__)
#define __IZANAGI_SYSTEM_MOUSE_H__

#include "izStd.h"
#include "InputDeviceDefs.h"

namespace izanagi
{
namespace sys
{
    /** マウス
     */
    class CMouse
    {
        friend class CSysWindow;
        friend class InputManager;

    public:
        CMouse() {}
        ~CMouse() {}

        NO_COPIABLE(CMouse);

    public:
        // 更新
        IZ_BOOL Update();

        IZ_BOOL IsClickL() const; // 右
        IZ_BOOL IsClickR() const; // 左
        IZ_BOOL IsClickM() const; // 中
        
        IZ_DWORD GetX() const;
        IZ_DWORD GetY() const;
        IZ_DWORD GetZ() const;

    private:
        CStdHash<IZ_UINT64, CMouse, 4>::Item* GetHashItem() { return &m_item; }

    protected:
        CStdHash<IZ_UINT64, CMouse, 4>::Item m_item;
    };
}   // namespace sys
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SYSTEM_MOUSE_H__)
