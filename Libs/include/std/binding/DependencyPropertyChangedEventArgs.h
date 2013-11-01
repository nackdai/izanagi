#if !defined(__IZANAGI_STD_PROPERTY_CHANGED_EVENT_ARGS_H__)
#define __IZANAGI_STD_PROPERTY_CHANGED_EVENT_ARGS_H__

#include "izDefs.h"
#include "std/StdValue.h"

namespace izanagi {
    class DependencyProperty;

    /** 依存性プロパティ変更イベントに渡される引数クラス
     */
    class DependencyPropertyChangedEventArgs
    {
    private:
        NO_COPIABLE(DependencyPropertyChangedEventArgs);

    public:
        DependencyPropertyChangedEventArgs(
            const CValue& oldVal,
            const CValue& newVal,
            const DependencyProperty& prop);

        /**
         */
        const CValue& GetOldValue() const
        {
            return m_Old;
        }

        /**
         */
        const CValue& GetNewValue() const
        {
            return m_New;
        }

        /**
         */
        const DependencyProperty& GetProperty();

    private:
        CValue m_Old;
        CValue m_New;
        const DependencyProperty& m_Prop;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_PROPERTY_CHANGED_EVENT_ARGS_H__)
