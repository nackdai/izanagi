#if !defined(__IZANAGI_STD_BINDING_OPERATIONS_H__)
#define __IZANAGI_STD_BINDING_OPERATIONS_H__

#include "izDefs.h"
#include "DependencyObject.h"
#include "DependencyProperty.h"
#include "Binding.h"

namespace izanagi {
    class BindingExpression;

    /** データバインディング実行
     */
    class BindingOperations
    {
        friend class BindingExpression;

    private:
        BindingOperations();
        ~BindingOperations();
        NO_COPIABLE(BindingOperations);

    public:
        /** データバインディングを設定.
         */
        static void SetBindings(
            IMemoryAllocator* allocator,
            DependencyObjectBase& target,
            DependencyProperty& property,
            Binding* binding);

        /** 指定されたプロパティに対するデータバインディングを取得.
         */
        static BindingExpression* GetBindingExpression(const DependencyProperty& property);

        /** 指定されたプロパティに対するデータバインディングを削除.
         */
        static IZ_BOOL RemoveBindingExpression(const DependencyProperty& property);

    private:
        typedef CStdHash<IZ_UINT, BindingExpression, 5> Dictionary;

        static Dictionary s_Dictionary;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_BINDING_OPERATIONS_H__)
