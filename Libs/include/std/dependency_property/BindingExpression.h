#if !defined(__IZANAGI_STD_BINDING_EXPRESSION_H__)
#define __IZANAGI_STD_BINDING_EXPRESSION_H__

#include "izDefs.h"
#include "std/StdPlacementNew.h"
#include "DependencyObject.h"
#include "DependencyProperty.h"
#include "Binding.h"
#include "BindingOperations.h"

namespace izanagi {
	/**
	 */
	class BindingExpression
	{
		friend class BindingOperations;

	private:
		// BindingExpression���쐬.
		static BindingExpression* Create(
			IMemoryAllocator* allocator,
			DependencyObjectBase& target,
			DependencyProperty& property,
			Binding* binding);

		// �쐬����BindingExpression�����.
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
		/** �^�[�Q�b�g�ւ̒l�̍X�V.
		 */
		void UpdateTarget()
		{
			IZ_ASSERT(m_Binding != IZ_NULL);

			CValue value;
			m_Binding->GetValue(value);

			m_Target.SetValue(m_Property, value);
		}

	private:
		BindingOperations::Dictionary::Item* GetItem()
		{
			return &m_Item;
		}

	private:
		IMemoryAllocator* m_Allocator;

		DependencyObjectBase& m_Target;
		DependencyProperty& m_Property;
		Binding* m_Binding;

		BindingOperations::Dictionary::Item m_Item;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_BINDING_EXPRESSION_H__)
