#if !defined(__IZANAGI_STD_DPENDENCY_PROPERTY_H__)
#define __IZANAGI_STD_DPENDENCY_PROPERTY_H__

#include "izDefs.h"
#include "std/StdPlacementNew.h"
#include "std/StdString.h"

namespace izanagi {
	/** 依存性プロパティ
	 */
	class DependencyProperty
	{
		friend class DependencyPropertyChangedEventArgs;

		IZ_DECL_PLACEMENT_NEW();

	public:
		static const IZ_UINT PropertyNameMax = 63;

		typedef CStdString<IZ_CHAR, PropertyNameMax> PropertyName;

	private:
		DependencyProperty() {}

		DependencyProperty(const DependencyProperty& rhs)
		{
			m_Name.SetString(rhs.GetName().GetString());
		}

	public:
		DependencyProperty(IZ_PCSTR name)
			: m_Name(name)
		{
		}

		~DependencyProperty() {}

		/**
		 */
		const CStdString<IZ_CHAR, PropertyNameMax>& GetName() const
		{
			return m_Name;
		}

	private:
		CStdString<IZ_CHAR, PropertyNameMax> m_Name;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_DPENDENCY_PROPERTY_H__)
