#if !defined(__IZANAGI_STD_DPENDENCY_OBJECT_H__)
#define __IZANAGI_STD_DPENDENCY_OBJECT_H__

#include "izDefs.h"
#include "NotifyPropertyChanged.h"
#include "DependencyPropertyChangedEventArgs.h"
#include "DependencyProperty.h"
#include "std/container/StdHash.h"

namespace izanagi {
	/**
	 */
	template <typename T = NullClass>
	class DependencyObject : TNotifyPropertyChanged<T>
	{
		static const IZ_UINT HASH_NUM = 8;

		class Element
		{
		public:
			DependencyProperty::PropertyName name;
			CValue value;
			typename CStdHash<IZ_UINT, Element, HASH_NUM>::Item hashItem;

			IZ_DECL_PLACEMENT_NEW();
		};

	public:
		DependencyObject() {}
		virtual ~DependencyObject() {}

		/**
		 */
		template <typename V>
		void SetValue(const DependencyProperty& prop, V value)
		{
			IZ_ASSERT(IZ_FALSE);
		}

		template <>
		void SetValue(const DependencyProperty& prop, IZ_INT8 value)
		{
			SetValueInternal(prop, CValue(value));
		}

		template <>
		void SetValue(const DependencyProperty& prop, IZ_UINT8 value)
		{
			SetValueInternal(prop, CValue(value));
		}

		template <>
		void SetValue(const DependencyProperty& prop, IZ_INT16 value)
		{
			SetValueInternal(prop, CValue(value));
		}

		template <>
		void SetValue(const DependencyProperty& prop, IZ_UINT16 value)
		{
			SetValueInternal(prop, CValue(value));
		}

		template <>
		void SetValue(const DependencyProperty& prop, IZ_INT32 value)
		{
			SetValueInternal(prop, CValue(value));
		}

		template <>
		void SetValue(const DependencyProperty& prop, IZ_UINT32 value)
		{
			SetValueInternal(prop, CValue(value));
		}

		template <>
		void SetValue(const DependencyProperty& prop, IZ_FLOAT value)
		{
			SetValueInternal(prop, CValue(value));
		}

		template <>
		void SetValue(const DependencyProperty& prop, void* value)
		{
			SetValueInternal(prop, CValue(value));
		}

		/**
		 */
		IZ_BOOL GetValue(const DependencyProperty& prop, CValue& value)
		{
			return GetValueInternal(prop, value);
		}

		/**
		 */
		virtual CStdEvent<void, const DependencyProperty&>& PropertyChanged()
		{
			return m_Event;
		}

		/**
		 */
		virtual void OnPropertyChanged(const DependencyPropertyChangedEventArgs& e) = 0;

	protected:
		/**
		 */
		virtual void* AllocForDependencyObject(size_t size) = 0;

		/**
		 */
		virtual void FreeForDependencyObject(void* p) = 0;

	private:
		typename CStdHash<IZ_UINT, Element, HASH_NUM>::Item* Find(const DependencyProperty& prop)
		{
			DependencyProperty::PropertyName name(prop.GetName());
			typename CStdHash<IZ_UINT, Element, HASH_NUM>::Item* item = m_Dictionary.Find(name.GetKeyValue());
			return item;
		}

		void SetValueInternal(const DependencyProperty& prop, const CValue& value)
		{
			typename CStdHash<IZ_UINT, Element, HASH_NUM>::Item* item = Find(prop);

			IZ_BOOL isValueChanged = IZ_FALSE;
			
			if (item != IZ_NULL)
			{
				isValueChanged = (item->GetData()->value != value);
				if (isValueChanged)
				{
					item->GetData()->value = value;
				}
			}
			else
			{
				//isValueChanged = IZ_TRUE;

				void* p = AllocForDependencyObject(sizeof(Element));
				IZ_ASSERT(p != IZ_NULL);

				Element* element = new(p) Element;
				{
					element->name.SetString(prop.GetName().GetString());
					element->value = value;
					element->hashItem.Init(element->name.GetKeyValue(), element);
				}

				m_Dictionary.Add(&element->hashItem);
			}

			if (isValueChanged)
			{
				DependencyPropertyChangedEventArgs e(
					item->GetData()->value,
					value,
					prop);

				OnPropertyChanged(e);
			}
		}

		IZ_BOOL GetValueInternal(const DependencyProperty& prop, CValue& ret)
		{
			typename CStdHash<IZ_UINT, Element, HASH_NUM>::Item* item = Find(prop);

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

	private:
		typename CStdHash<IZ_UINT, Element, HASH_NUM> m_Dictionary;
		CStdEvent<void, const DependencyProperty&> m_Event;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_DPENDENCY_OBJECT_H__)
