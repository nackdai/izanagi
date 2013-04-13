#if !defined(__IZANAGI_STD_BINDING_EXPRESSION_H__)
#define __IZANAGI_STD_BINDING_EXPRESSION_H__

#include "izDefs.h"
#include "std/StdPlacementNew.h"
#include "DependencyObject.h"
#include "DependencyProperty.h"
#include "Binding.h"
#include "BindingOperations.h"

namespace izanagi {
    /** データバインディングの関係状態
     */
    class BindingExpression
    {
        friend class BindingOperations;

    private:
        // BindingExpressionを作成.
        static BindingExpression* Create(
            IMemoryAllocator* allocator,
            DependencyObjectBase& target,
            DependencyProperty& property,
            Binding* binding);

        // 作成したBindingExpressionを解放.
        static void Free(BindingExpression* exp);

    private:
        BindingExpression(
            DependencyObjectBase& target,
            DependencyProperty& property,
            Binding* binding);

        ~BindingExpression()
        {
            SAFE_RELEASE(m_Binding);
        }

        NO_COPIABLE(BindingExpression);

        IZ_DECL_PLACEMENT_NEW();

    public:
        /** バインディング ソース プロパティまたはバインディング ターゲット プロパティの
         *  どちらかにかかわらず強制的に値を送信
         */
        void Update();

        /** 現在のバインディングターゲット値を
         *  TwoWay バインディングまたは OneWayToSource バインディングの
         *  バインディング ソース プロパティに送信
         */
        void UpdateSource();

        /** バインディング ソース プロパティからバインディング ターゲット プロパティへデータを強制的に転送
         */
        void UpdateTarget();

    private:
        BindingOperations::Dictionary::Item* GetItem()
        {
            return &m_Item;
        }

    private:
        IMemoryAllocator* m_Allocator;

        // バインディング ターゲット オブジェクト
        DependencyObjectBase& m_Target;

        // バインディング ターゲット プロパティ
        DependencyProperty& m_Property;

        // バインディング ターゲット オブジェクトのプロパティと任意のデータ ソース とを接続
        Binding* m_Binding;

        BindingOperations::Dictionary::Item m_Item;

        // For E_BINDING_MODE_ONE_TIME.
        IZ_BOOL m_IsBinded;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_BINDING_EXPRESSION_H__)
