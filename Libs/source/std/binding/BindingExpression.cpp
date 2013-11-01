#include "std/dependency_property/BindingExpression.h"

namespace izanagi
{
    // BindingExpressionを作成.
    BindingExpression* BindingExpression::Create(
        IMemoryAllocator* allocator,
        DependencyObjectBase& target,
        DependencyProperty& property,
        Binding* binding)
    {
        void* buf = ALLOC_ZERO(allocator, sizeof(BindingExpression));
        VRETURN_NULL(buf != IZ_NULL);

        BindingExpression* ret = new(buf) BindingExpression(target, property, binding);
        ret->m_Allocator = allocator;
        return ret;
    }

    // 作成したBindingExpressionを解放.
    void BindingExpression::Free(BindingExpression* exp)
    {
        IZ_ASSERT(exp != IZ_NULL);

        IMemoryAllocator* allocator = exp->m_Allocator;
        delete exp;
        FREE(allocator, exp);
    }

    BindingExpression::BindingExpression(
        DependencyObjectBase& target,
        DependencyProperty& property,
        Binding* binding)
        : m_Target(target), m_Property(property)
    {
        SAFE_REPLACE(m_Binding, binding);
        m_Allocator = IZ_NULL;

        m_Item.Init(
            property.GetKey(),
            this);

        m_IsBinded = IZ_FALSE;
    }

    // バインディング ソース プロパティまたはバインディング ターゲット プロパティの
    // どちらかにかかわらず強制的に値を送信
    void BindingExpression::Update()
    {
#if 0
        IZ_ASSERT(m_Binding != IZ_NULL);

        E_BINDING_MODE mode = m_Binding->GetMode();

        if (mode == E_BINDING_MODE_ONE_WAY)
        {
            CValue value;
            m_Binding->GetValue(value);
            m_Target.SetValue(m_Property, value);
        }
        else if (mode == E_BINDING_MODE_TWO_WAY)
        {
            CValue srcValue;
            m_Binding->GetValue(srcValue);

            CValue targetValue;
            m_Target.GetValue(m_Property, targetValue);

            if (srcValue != targetValue)
            {
                // 値が異なるのでセットする

                if (m_Target.IsValueChagned())
                {
                    // ターゲットの値が変更されている
                    m_Binding->SetValue(targetValue);
                }
                else
                {
                    m_Target.SetValue(m_Property, srcValue);
                }
            }

            m_Target.ClearValueChagned();
        }
        else if (mode == E_BINDING_MODE_ONE_TIME)
        {
            if (!m_IsBinded)
            {
                CValue value;
                m_Binding->GetValue(value);
                m_Target.SetValue(m_Property, value);

                m_IsBinded = IZ_TRUE;
            }
        }
        else if (mode == E_BINDING_MODE_ONE_WAY_TO_SOURCE)
        {
            CValue value;
            m_Target.GetValue(m_Property, value);
            m_Binding->SetValue(value);
        }
#else
        UpdateSource();
        UpdateTarget();
#endif
    }

    // 現在のバインディングターゲット値を
    // TwoWay バインディングまたは OneWayToSource バインディングの
    // バインディング ソース プロパティに送信
    void BindingExpression::UpdateSource()
    {
        IZ_ASSERT(m_Binding != IZ_NULL);

        E_BINDING_MODE mode = m_Binding->GetMode();

        if (mode == E_BINDING_MODE_TWO_WAY)
        {
            CValue srcValue;
            m_Binding->GetValue(srcValue);

            CValue targetValue;
            m_Target.GetValue(m_Property, targetValue);

            if (srcValue != targetValue)
            {
                // 値が異なるのでセットする

                if (m_Target.IsValueChagned())
                {
                    // ターゲットの値が変更されている
                    m_Binding->SetValue(targetValue);
                }
                else
                {
                    // NOTE
                    // ソースへの送信なのでターゲットに関しては無視
                    //m_Target.SetValue(m_Property, srcValue);
                }
            }

            // TODO
            //m_Target.ClearValueChagned();
        }
        else if (mode == E_BINDING_MODE_ONE_WAY_TO_SOURCE)
        {
            CValue value;
            m_Target.GetValue(m_Property, value);
            m_Binding->SetValue(value);
        }
    }

    // バインディング ソース プロパティからバインディング ターゲット プロパティへデータを強制的に転送
    void BindingExpression::UpdateTarget()
    {
        IZ_ASSERT(m_Binding != IZ_NULL);

        E_BINDING_MODE mode = m_Binding->GetMode();

        if (mode == E_BINDING_MODE_ONE_WAY)
        {
            CValue value;
            m_Binding->GetValue(value);
            m_Target.SetValue(m_Property, value);
        }
        else if (mode == E_BINDING_MODE_TWO_WAY)
        {
            CValue srcValue;
            m_Binding->GetValue(srcValue);

            CValue targetValue;
            m_Target.GetValue(m_Property, targetValue);

            if (srcValue != targetValue)
            {
                // 値が異なるのでセットする

                if (m_Target.IsValueChagned())
                {
                    // NOTE
                    // ターゲットへの送信なのでソースは無視する
                    //m_Binding->SetValue(targetValue);
                }
                else
                {
                    m_Target.SetValue(m_Property, srcValue);
                }
            }

            m_Target.ClearValueChagned();
        }
        else if (mode == E_BINDING_MODE_ONE_TIME)
        {
            if (!m_IsBinded)
            {
                CValue value;
                m_Binding->GetValue(value);
                m_Target.SetValue(m_Property, value);

                m_IsBinded = IZ_TRUE;
            }
        }
    }
}   // namespace izanagi
