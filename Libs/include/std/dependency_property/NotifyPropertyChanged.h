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
	template <typename T>
	class TNotifyPropertyChanged : public T
	{
	public:
		TNotifyPropertyChanged() {}
		virtual ~TNotifyPropertyChanged() {}

		/**
		 */
		virtual CStdEvent<void, const DependencyProperty&>& PropertyChanged() = 0;
	};

	/**
	 */
	class INotifyPropertyChanged : TNotifyPropertyChanged<NullClass> {};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_NOTIFY_PROPERTY_CHANGED_H__)
