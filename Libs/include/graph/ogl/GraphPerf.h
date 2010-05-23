#if !defined(__URANUS_GRAPH_PERF_H__)
#define __URANUS_GRAPH_PERF_H__

#include "unDefs.h"
#include "unD3DDefs.h"

namespace uranus {
	class CGraphPerf {
	private:
		CGraphPerf();
		~CGraphPerf();

		CGraphPerf(const CGraphPerf& rhs);
		const CGraphPerf& operator=(const CGraphPerf& rhs);

	public:
		static inline UN_INT BeginEvent(
			UN_COLOR color,
#if 1
			UN_PCSTR pszName);
#else
			UN_PCWSTR pszName);
#endif

		static inline UN_INT EndEvent();
	};

	// inline ***************************

	/**
	* イベント開始
	*/
	UN_INT CGraphPerf::BeginEvent(
		UN_COLOR color,
#if 1
		UN_PCSTR pszName)
#else
		UN_PCWSTR pszName)
#endif
	{
#ifdef __UN_DEBUG__
#if 1
		static const UN_UINT BUF_SIZE = 32;
		static wchar_t buf[BUF_SIZE];

		size_t len = strlen(pszName);
		UN_ASSERT(len <= BUF_SIZE);

		mbstowcs_s(&len, buf, pszName, BUF_SIZE);
		UN_ASSERT(len >= 0);

		UN_INT ret = D3DPERF_BeginEvent(color, buf);
#else
		UN_INT ret = D3DPERF_BeginEvent(color, pszName);
#endif
		//UN_ASSERT(ret >= 0);
		return ret;
#else	// #ifdef __UN_DEBUG__
		UNUSED_ALWAYS(color);
		UNUSED_ALWAYS(pszName);
		return 0;
#endif	// #ifdef __UN_DEBUG__
	}

	/**
	* イベント終了
	*/
	UN_INT CGraphPerf::EndEvent()
	{
#ifdef __UN_DEBUG__
		UN_INT ret = D3DPERF_EndEvent();
		//UN_ASSERT(ret >= 0);
		return ret;
#else	// #ifdef __UN_DEBUG__
		return 0;
#endif	// #ifdef __UN_DEBUG__
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_PERF_H__)
