#if !defined(__IZ_SAMPLE_SAMPLE_MAIN_H__)
#define __IZ_SAMPLE_SAMPLE_MAIN_H__

#include "izStd.h"

namespace izanagi {
namespace sample {
	class CSampleApp;

	/**
	 */
	struct SSampleParam {
		izanagi::sample::CSampleApp* app;
		izanagi::IMemoryAllocator* allocator;
		IZ_UINT width;
		IZ_UINT height;
		IZ_BOOL isWindowed;
		IZ_PCSTR title;
		void* platformParam;
	};
}
}

IZ_BOOL SampleMainLoop(izanagi::sample::SSampleParam& params);

#endif	// #if !defined(__IZ_SAMPLE_SAMPLE_MAIN_H__)
