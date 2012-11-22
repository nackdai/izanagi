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

	// 値の更新.
	void BindingExpression::Update()
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
					// ターゲットの値が変更された
					m_Target.SetValue(m_Property, srcValue);
					m_Target.ClearValueChagned();
				}
				else
				{
					m_Binding->SetValue(targetValue);
				}
			}
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
	}
}	// namespace izanagi
