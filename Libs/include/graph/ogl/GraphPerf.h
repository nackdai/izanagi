#if !defined(__IZANAGI_GRAPH_PERF_H__)
#define __IZANAGI_GRAPH_PERF_H__

#include "izDefs.h"
#include "izD3DDefs.h"

namespace izanagi {
	class CGraphPerf {
	private:
		CGraphPerf();
		~CGraphPerf();

		CGraphPerf(const CGraphPerf& rhs);
		const CGraphPerf& operator=(const CGraphPerf& rhs);

	public:
		static inline IZ_INT BeginEvent(
			IZ_COLOR color,
#if 1
			IZ_PCSTR pszName);
#else
			IZ_PCWSTR pszName);
#endif

		static inline IZ_INT EndEvent();
	};

	// inline ***************************

	/**
	* イベント開始
	*/
	IZ_INT CGraphPerf::BeginEvent(
		IZ_COLOR color,
#if 1
		IZ_PCSTR pszName)
#else
		IZ_PCWSTR pszName)
#endif
	{
#ifdef __IZ_DEBUG__
#if 1
		static const IZ_UINT BUF_SIZE = 32;
		static wchar_t buf[BUF_SIZE];

		size_t len = strlen(pszName);
		IZ_ASSERT(len <= BUF_SIZE);

		mbstowcs_s(&len, buf, pszName, BUF_SIZE);
		IZ_ASSERT(len >= 0);

		IZ_INT ret = D3DPERF_BeginEvent(color, buf);
#else
		IZ_INT ret = D3DPERF_BeginEvent(color, pszName);
#endif
		//IZ_ASSERT(ret >= 0);
		return ret;
#else	// #ifdef __IZ_DEBUG__
		UNUSED_ALWAYS(color);
		UNUSED_ALWAYS(pszName);
		return 0;
#endif	// #ifdef __IZ_DEBUG__
	}

	/**
	* イベント終了
	*/
	IZ_INT CGraphPerf::EndEvent()
	{
#ifdef __IZ_DEBUG__
		IZ_INT ret = D3DPERF_EndEvent();
		//IZ_ASSERT(ret >= 0);
		return ret;
#else	// #ifdef __IZ_DEBUG__
		return 0;
#endif	// #ifdef __IZ_DEBUG__
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_PERF_H__)
