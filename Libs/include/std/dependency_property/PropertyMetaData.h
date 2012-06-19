#if 0
#if !defined(__IZANAGI_STD_PROPERTY_META_DATA_H__)
#define __IZANAGI_STD_PROPERTY_META_DATA_H__

#include "izDefs.h"

namespace izanagi {
	class DependencyObject;
	class DependencyPropertyChangedEventArgs;

	/**
	 */
	class PropertyMetaData
	{
		friend class DependencyProperty;

	public:
		/**
		 */
		typedef void (*PropertyChangedCallback)(const DependencyObject&, const DependencyPropertyChangedEventArgs&);

	private:
		PropertyMetaData() {}

		PropertyMetaData(const PropertyMetaData& rhs) {}

		const PropertyMetaData& operator=(const PropertyMetaData& rhs)
		{
			m_IsValid = rhs.m_IsValid;
			m_Default = rhs.m_Default;
			m_Callback = rhs.m_Callback;
			return *this;
		}

	public:
		template <typename T>
		PropertyMetaData(T value)
		{
			IZ_ASSERT(IZ_FALSE);
		}

		template <>	PropertyMetaData(IZ_INT8 value)   : m_Default(value), m_IsValid(IZ_TRUE) {}
		template <>	PropertyMetaData(IZ_UINT8 value)  : m_Default(value), m_IsValid(IZ_TRUE) {}
		template <>	PropertyMetaData(IZ_INT16 value)  : m_Default(value), m_IsValid(IZ_TRUE) {}
		template <>	PropertyMetaData(IZ_UINT16 value) : m_Default(value), m_IsValid(IZ_TRUE) {}
		template <>	PropertyMetaData(IZ_INT32 value)  : m_Default(value), m_IsValid(IZ_TRUE) {}
		template <>	PropertyMetaData(IZ_UINT32 value) : m_Default(value), m_IsValid(IZ_TRUE) {}
		template <>	PropertyMetaData(IZ_FLOAT value)  : m_Default(value), m_IsValid(IZ_TRUE) {}
		template <>	PropertyMetaData(void* value)     : m_Default(value), m_IsValid(IZ_TRUE) {}

		PropertyMetaData(PropertyChangedCallback callback) : m_Callback(callback) {}

		template <typename T>
		PropertyMetaData(T value, PropertyChangedCallback callback)
		{
			IZ_ASSERT(IZ_FALSE);
		}

		template <>	PropertyMetaData(IZ_INT8 value, PropertyChangedCallback callback)   : m_Callback(callback), m_Default(value), m_IsValid(IZ_TRUE) {}
		template <>	PropertyMetaData(IZ_UINT8 value, PropertyChangedCallback callback)  : m_Callback(callback), m_Default(value), m_IsValid(IZ_TRUE) {}
		template <>	PropertyMetaData(IZ_INT16 value, PropertyChangedCallback callback)  : m_Callback(callback), m_Default(value), m_IsValid(IZ_TRUE) {}
		template <>	PropertyMetaData(IZ_UINT16 value, PropertyChangedCallback callback) : m_Callback(callback), m_Default(value), m_IsValid(IZ_TRUE) {}
		template <>	PropertyMetaData(IZ_INT32 value, PropertyChangedCallback callback)  : m_Callback(callback), m_Default(value), m_IsValid(IZ_TRUE) {}
		template <>	PropertyMetaData(IZ_UINT32 value, PropertyChangedCallback callback) : m_Callback(callback), m_Default(value), m_IsValid(IZ_TRUE) {}
		template <>	PropertyMetaData(IZ_FLOAT value, PropertyChangedCallback callback)  : m_Callback(callback), m_Default(value), m_IsValid(IZ_TRUE) {}
		template <>	PropertyMetaData(void* value, PropertyChangedCallback callback)     : m_Callback(callback), m_Default(value), m_IsValid(IZ_TRUE) {}

		~PropertyMetaData()
		{
		}

		IZ_BOOL IsValidDefaultValue()
		{
			return m_IsValid;
		}

		const CValue& GetDefaultValue() const
		{
			return m_Default;
		}

		PropertyChangedCallback GetCallback()
		{
			return m_Callback;
		}

	private:
		IZ_BOOL m_IsValid;
		CValue m_Default;
		PropertyChangedCallback m_Callback;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_PROPERTY_META_DATA_H__)
#endif
