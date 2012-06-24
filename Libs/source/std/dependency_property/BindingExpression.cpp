#include "std/dependency_property/BindingExpression.h"

namespace izanagi
{
	// BindingExpression‚ðì¬.
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

	// ì¬‚µ‚½BindingExpression‚ð‰ð•ú.
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
	}
}	// namespace izanagi
