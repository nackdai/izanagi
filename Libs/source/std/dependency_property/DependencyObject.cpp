#include "std/dependency_property/DependencyObject.h"

namespace izanagi
{
	DependencyObjectBase::DependencyObjectBase()
	{
		m_Flags.isCalledClearAll = IZ_FALSE;
		m_Flags.isValueChanged = IZ_FALSE;
	}

	void DependencyObjectBase::ClearAll()
	{
		CStdList<CStdHash<IZ_UINT, Element, HASH_NUM>::Item>::Item* item = m_Dictionary.GetOrderTop();
		while (item != IZ_NULL)
		{
			CStdList<CStdHash<IZ_UINT, Element, HASH_NUM>::Item>::Item* next = item->GetNext();
			Element* element = item->GetData()->GetData();
			delete element;
			FreeForDependencyObject(element);
			item = next;
		}

		m_Flags.isCalledClearAll = IZ_TRUE;
	}	

	CStdHash<IZ_UINT, DependencyObjectBase::Element, DependencyObjectBase::HASH_NUM>::Item* DependencyObjectBase::Find(const DependencyProperty& prop)
	{
		DependencyProperty::PropertyName name(prop.GetName());
		CStdHash<IZ_UINT, Element, HASH_NUM>::Item* item = m_Dictionary.Find(name.GetKeyValue());
		return item;
	}


	void DependencyObjectBase::SetValueInternal(const DependencyProperty& prop, const CValue& value)
	{
		// プロパティに該当するハッシュアイテムを探す
		CStdHash<IZ_UINT, Element, HASH_NUM>::Item* item = Find(prop);

		m_Flags.isValueChanged = IZ_FALSE;

		CValue oldVal;
		
		if (item != IZ_NULL)
		{
			// 登録済み

			oldVal = item->GetData()->value;

			m_Flags.isValueChanged = (item->GetData()->value != value);
			if (m_Flags.isValueChanged)
			{
				item->GetData()->value = value;
			}
		}
		else
		{
			// 初登録

			//m_Flags.isValueChanged = IZ_TRUE;

			// 登録用の要素を作成
			void* p = AllocForDependencyObject(sizeof(Element));
			IZ_ASSERT(p != IZ_NULL);

			Element* element = new(p) Element;
			{
				element->name.SetString(prop.GetName().GetString());
				element->value = value;
				element->hashItem.Init(element->name.GetKeyValue(), element);
			}

			// 登録
			m_Dictionary.Add(&element->hashItem);
		}

		if (m_Flags.isValueChanged)
		{
			// 値が変更された

			DependencyPropertyChangedEventArgs e(
				oldVal,
				value,
				prop);

			OnPropertyChanged(e);
		}
	}

	IZ_BOOL DependencyObjectBase::GetValueInternal(const DependencyProperty& prop, CValue& ret)
	{
		CStdHash<IZ_UINT, Element, HASH_NUM>::Item* item = Find(prop);

		if (item != IZ_NULL)
		{
			ret = item->GetData()->value;
			return IZ_TRUE;
		}
#if 0
		else if (prop.GetMetaData().IsValidDefaultValue())
		{
			ret = prop.GetMetaData().GetDefaultValue();
			return IZ_TRUE;
		}
#endif

		return IZ_FALSE;
	}
}	// namespace izanagi
