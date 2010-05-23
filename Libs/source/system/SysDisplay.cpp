#include "system/SysDisplay.h"

#pragma comment(lib, "Gdi32.lib")

using namespace uranus;

static HDC g_HDC = UN_NULL;

void CSysDisplay::Init(HDC hdc)
{
	g_HDC = hdc;
}

/**
* Get display's pixel width.
*/
UN_UINT CSysDisplay::GetPixelWidth()
{
	UN_ASSERT(g_HDC != UN_NULL);
	UN_UINT ret = ::GetDeviceCaps(g_HDC, HORZRES);
	return ret;
}

/**
* Get display's pixel height.
*/
UN_UINT CSysDisplay::GetPixelHeight()
{
	UN_ASSERT(g_HDC != UN_NULL);
	UN_UINT ret = ::GetDeviceCaps(g_HDC, VERTRES);
	return ret;
}

/**
* Get display' physical width.
*/
UN_UINT CSysDisplay::GetPhysicalWidth()
{
	UN_ASSERT(g_HDC != UN_NULL);
	UN_UINT ret = ::GetDeviceCaps(g_HDC, HORZSIZE);
	return ret;
}

/**
* Get display' physical height.
*/
UN_UINT CSysDisplay::GetPhysicalHeight()
{
	UN_ASSERT(g_HDC != UN_NULL);
	UN_UINT ret = ::GetDeviceCaps(g_HDC, VERTSIZE);
	return ret;
}

/**
* Get display's horizontal dpi.
*/
UN_UINT CSysDisplay::GetHorizontalDPI()
{
	UN_ASSERT(g_HDC != UN_NULL);
	UN_UINT ret = ::GetDeviceCaps(g_HDC, LOGPIXELSX);
	return ret;
}

/**
* Get display's vertical dpi.
*/
UN_UINT CSysDisplay::GetVerticalDPI()
{
	UN_ASSERT(g_HDC != UN_NULL);
	UN_UINT ret = ::GetDeviceCaps(g_HDC, LOGPIXELSY);
	return ret;
}

/**
* Get display's refresh rate.
*/
UN_UINT CSysDisplay::GetRefreshRate()
{
	UN_ASSERT(g_HDC != UN_NULL);
	UN_UINT ret = ::GetDeviceCaps(g_HDC, VREFRESH);
	return ret;
}
