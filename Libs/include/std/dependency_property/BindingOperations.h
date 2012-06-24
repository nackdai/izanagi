#if !defined(__IZANAGI_STD_BINDING_OPERATIONS_H__)
#define __IZANAGI_STD_BINDING_OPERATIONS_H__

#include "izDefs.h"
#include "DependencyObject.h"
#include "DependencyProperty.h"
#include "Binding.h"

namespace izanagi {
	class BindingExpression;

	/**
	 */
	class BindingOperations
	{
		friend class BindingExpression;

	private:
		BindingOperations();
		~BindingOperations();
		NO_COPIABLE(BindingOperations);

	public:
		/** �f�[�^�o�C���f�B���O��ݒ�.
		 */
		static void SetBindings(
			IMemoryAllocator* allocator,
			DependencyObjectBase& target,
			DependencyProperty& property,
			Binding* binding);

		/** �w�肳�ꂽ�v���p�e�B�ɑ΂���f�[�^�o�C���f�B���O���擾.
		 */
		static BindingExpression* GetBindingExpression(const DependencyProperty& property);

		/** �w�肳�ꂽ�v���p�e�B�ɑ΂���f�[�^�o�C���f�B���O���폜.
		 */
		static IZ_BOOL RemoveBindingExpression(const DependencyProperty& property);

	private:
		typedef CStdHash<IZ_UINT, BindingExpression, 5>	Dictionary;

		static Dictionary s_Dictionary;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_BINDING_OPERATIONS_H__)
