#if !defined(__URANUS_SYSTEM_SYS_DISPLAY_H__)
#define __URANUS_SYSTEM_SYS_DISPLAY_H__

#include <windows.h>
#include "unDefs.h"

namespace uranus {
	/**
	*/
	class CSysDisplay {
	private:
		CSysDisplay();
		~CSysDisplay();

		NO_COPIABLE(CSysDisplay);

	public:
		// Initialize.
		static void Init(HDC hdc);

		// Get display's pixel size.
		static UN_UINT GetPixelWidth();
		static UN_UINT GetPixelHeight();

		// Get display' physical size.
		static UN_UINT GetPhysicalWidth();
		static UN_UINT GetPhysicalHeight();

		// Get display's dpi.
		static UN_UINT GetHorizontalDPI();
		static UN_UINT GetVerticalDPI();

		// Get display's refresh rate.
		static UN_UINT GetRefreshRate();
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_SYSTEM_SYS_DISPLAY_H__)
