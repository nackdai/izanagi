#include "system/SysDisplay.h"

#pragma comment(lib, "Gdi32.lib")

using namespace izanagi;

static HDC g_HDC = IZ_NULL;

void CSysDisplay::Init(HDC hdc)
{
	g_HDC = hdc;
}

/**
* Get display's pixel width.
*/
IZ_UINT CSysDisplay::GetPixelWidth()
{
	IZ_ASSERT(g_HDC != IZ_NULL);
	IZ_UINT ret = ::GetDeviceCaps(g_HDC, HORZRES);
	return ret;
}

/**
* Get display's pixel height.
*/
IZ_UINT CSysDisplay::GetPixelHeight()
{
	IZ_ASSERT(g_HDC != IZ_NULL);
	IZ_UINT ret = ::GetDeviceCaps(g_HDC, VERTRES);
	return ret;
}

/**
* Get display' physical width.
*/
IZ_UINT CSysDisplay::GetPhysicalWidth()
{
	IZ_ASSERT(g_HDC != IZ_NULL);
	IZ_UINT ret = ::GetDeviceCaps(g_HDC, HORZSIZE);
	return ret;
}

/**
* Get display' physical height.
*/
IZ_UINT CSysDisplay::GetPhysicalHeight()
{
	IZ_ASSERT(g_HDC != IZ_NULL);
	IZ_UINT ret = ::GetDeviceCaps(g_HDC, VERTSIZE);
	return ret;
}

/**
* Get display's horizontal dpi.
*/
IZ_UINT CSysDisplay::GetHorizontalDPI()
{
	IZ_ASSERT(g_HDC != IZ_NULL);
	IZ_UINT ret = ::GetDeviceCaps(g_HDC, LOGPIXELSX);
	return ret;
}

/**
* Get display's vertical dpi.
*/
IZ_UINT CSysDisplay::GetVerticalDPI()
{
	IZ_ASSERT(g_HDC != IZ_NULL);
	IZ_UINT ret = ::GetDeviceCaps(g_HDC, LOGPIXELSY);
	return ret;
}

/**
* Get display's refresh rate.
*/
IZ_UINT CSysDisplay::GetRefreshRate()
{
	IZ_ASSERT(g_HDC != IZ_NULL);
	IZ_UINT ret = ::GetDeviceCaps(g_HDC, VREFRESH);
	return ret;
}
