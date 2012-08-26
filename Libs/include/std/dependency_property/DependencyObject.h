#if !defined(__IZANAGI_STD_DPENDENCY_OBJECT_H__)
#define __IZANAGI_STD_DPENDENCY_OBJECT_H__

#include "izDefs.h"
#include "NotifyPropertyChanged.h"
#include "DependencyPropertyChangedEventArgs.h"
#include "DependencyProperty.h"
#include "std/collection/StdHash.h"

namespace izanagi {
	/** 依存性プロパティを持つオブジェクトのベースクラス
	 */
	class DependencyObjectBase
	{
		friend class BindingExpression;

		static const IZ_UINT HASH_NUM = 8;

		class Element
		{
		public:
			DependencyProperty::PropertyName name;
			CValue value;
			CStdHash<IZ_UINT, Element, HASH_NUM>::Item hashItem;

			IZ_DECL_PLACEMENT_NEW();
		};

	public:
		DependencyObjectBase();

		virtual ~DependencyObjectBase()
		{
			IZ_ASSERT(m_Flags.isCalledClearAll);
		}

		/**
		 */
		void SetValue(const DependencyProperty& prop, const CValue& value)
		{
			SetValueInternal(prop, value);
		}

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
		virtual void OnPropertyChanged(const DependencyPropertyChangedEventArgs& e) = 0;

	protected:
		void ClearAll();

		/**
		 */
		virtual void* AllocForDependencyObject(size_t size) = 0;

		/**
		 */
		virtual void FreeForDependencyObject(void* p) = 0;

	private:
		CStdHash<IZ_UINT, Element, HASH_NUM>::Item* Find(const DependencyProperty& prop);

		void SetValueInternal(const DependencyProperty& prop, const CValue& value);

		IZ_BOOL GetValueInternal(const DependencyProperty& prop, CValue& ret);

		IZ_BOOL IsValueChagned() const { return m_Flags.isValueChanged; }

		void ClearValueChagned() { m_Flags.isValueChanged = IZ_FALSE; }

	private:
		CStdHash<IZ_UINT, Element, HASH_NUM> m_Dictionary;

		struct
		{
			IZ_UINT32 isCalledClearAll	: 1;
			IZ_UINT32 isValueChanged	: 1;
		} m_Flags;
	};

	/** 依存性プロパティを持つオブジェクト
	 */
	template <typename T = NullClass>
	class DependencyObject : public DependencyObjectBase, public INotifyPropertyChanged<T>
	{
	public:
		DependencyObject() {}
		virtual ~DependencyObject() {}

		/**
		 */
		virtual CStdEvent<void, const DependencyProperty&>& PropertyChanged()
		{
			return m_Event;
		}

	private:
		CStdEvent<void, const DependencyProperty&> m_Event;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_DPENDENCY_OBJECT_H__)
