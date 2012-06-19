#if !defined(__IZANAGI_STD_NOTIFY_PROPERTY_CHANGED_H__)
#define __IZANAGI_STD_NOTIFY_PROPERTY_CHANGED_H__

#include "izDefs.h"
#include "std/StdEvent.h"

namespace izanagi {
	class DependencyProperty;

	/**
	 */
	class NullClass {};

	/**
	 */
	template <typename T = NullClass>
	class INotifyPropertyChanged : public T
	{
	public:
		INotifyPropertyChanged() {}
		virtual ~INotifyPropertyChanged() {}

		/**
		 */
		virtual CStdEvent<void, const DependencyProperty&>& PropertyChanged() = 0;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_NOTIFY_PROPERTY_CHANGED_H__)
