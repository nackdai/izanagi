#if !defined(__IZANAGI_SYSTEM_SYS_DISPLAY_H__)
#define __IZANAGI_SYSTEM_SYS_DISPLAY_H__

#include "izDefs.h"

namespace izanagi
{
namespace sys
{
	/**
	*/
	class CSysDisplay {
	private:
		CSysDisplay();
		~CSysDisplay();

		NO_COPIABLE(CSysDisplay);

	public:
		// Initialize.
		static void Init(void* nativeWndHandle);

		// Get display's pixel size.
		static IZ_UINT GetPixelWidth();
		static IZ_UINT GetPixelHeight();

		// Get display' physical size.
		static IZ_UINT GetPhysicalWidth();
		static IZ_UINT GetPhysicalHeight();

		// Get display's dpi.
		static IZ_UINT GetHorizontalDPI();
		static IZ_UINT GetVerticalDPI();

		// Get display's refresh rate.
		static IZ_UINT GetRefreshRate();
	};
}   // namespace sys
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SYSTEM_SYS_DISPLAY_H__)
