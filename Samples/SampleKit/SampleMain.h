#if !defined(__IZ_SAMPLE_SAMPLE_MAIN_H__)
#define __IZ_SAMPLE_SAMPLE_MAIN_H__

#include "izStd.h"

namespace izanagi {
namespace sample {
	class CSampleApp;
}
}

IZ_INT SampleMain(
	void* systemData,
	izanagi::sample::CSampleApp* app,
	const char* title,
	IZ_UINT screenWidth, IZ_UINT screenHeight,
	void* allocatorBuf, IZ_UINT bufSize,
	void* graphBuf, IZ_UINT graphBufSize);

#endif	// #if !defined(__IZ_SAMPLE_SAMPLE_MAIN_H__)
