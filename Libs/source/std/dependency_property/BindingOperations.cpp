#include "std/dependency_property/BindingOperations.h"
#include "std/dependency_property/BindingExpression.h"

namespace izanagi
{
	BindingOperations::Dictionary BindingOperations::s_Dictionary;

	// �f�[�^�o�C���f�B���O��ݒ�.
	void BindingOperations::SetBindings(
		IMemoryAllocator* allocator,
		DependencyObjectBase& target,
		DependencyProperty& property,
		Binding* binding)
	{
		BindingExpression* exp = BindingExpression::Create(
			allocator,
			target,
			property,
			binding);

		if (exp != IZ_NULL)
		{
			s_Dictionary.Add(exp->GetItem());
		}
	}

	// �w�肳�ꂽ�v���p�e�B�ɑ΂���f�[�^�o�C���f�B���O���擾.
	BindingExpression* BindingOperations::GetBindingExpression(const DependencyProperty& property)
	{
		BindingExpression* exp = s_Dictionary.FindData(property.GetKey());
		return exp;
	}

	// �w�肳�ꂽ�v���p�e�B�ɑ΂���f�[�^�o�C���f�B���O���폜.
	IZ_BOOL BindingOperations::RemoveBindingExpression(const DependencyProperty& property)
	{
		Dictionary::Item* item = s_Dictionary.Find(property.GetKey());
		if (item != IZ_NULL)
		{
			BindingExpression* exp = item->GetData();
			delete exp;

			item->Leave();
			return IZ_TRUE;
		}

		return IZ_FALSE;
	}
}	// namespace izanagi
