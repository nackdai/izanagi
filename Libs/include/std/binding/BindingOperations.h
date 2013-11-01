#if !defined(__IZANAGI_STD_BINDING_OPERATIONS_H__)
#define __IZANAGI_STD_BINDING_OPERATIONS_H__

#include "izDefs.h"
#include "DependencyObject.h"
#include "DependencyProperty.h"
#include "Binding.h"

namespace izanagi {
    class BindingExpression;

    /** Binding オブジェクトのバインディングを操作するための静的メソッドを提供する.
     */
    class BindingOperations
    {
        friend class BindingExpression;

    private:
        BindingOperations();
        ~BindingOperations();
        NO_COPIABLE(BindingOperations);

    public:
        template <typename _T>
        static void SetBindings(
            IMemoryAllocator* allocator,
            _T* target,
            DependencyProperty& property,
            Binding* binding)
        {
            SetBindings(
                allocator,
                *(DependencyObjectBase*)target,
                property,
                binding);
        }

        /** データバインディングを設定.
         */
        static void SetBindings(
            IMemoryAllocator* allocator,
            DependencyObjectBase& target,
            DependencyProperty& property,
            Binding* binding);

        /** 指定されたオブジェクト上の指定されたバインディング ターゲット プロパティに関連付けられた BindingExpression オブジェクトを取得.
         */
        static BindingExpression* GetBindingExpression(const DependencyProperty& property);

        /** 指定されたオブジェクト上の指定されたバインディング ターゲット プロパティに関連付けられた BindingExpression オブジェクトを削除.
         */
        static IZ_BOOL RemoveBindingExpression(const DependencyProperty& property);

    private:
        typedef CStdHash<IZ_UINT, BindingExpression, 5> Dictionary;

        static Dictionary s_Dictionary;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_BINDING_OPERATIONS_H__)
