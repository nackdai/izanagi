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
		/** 値の更新.
		 */
		void Update();

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

		// For E_BINDING_MODE_ONE_TIME.
		IZ_BOOL m_IsBinded;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_BINDING_EXPRESSION_H__)
