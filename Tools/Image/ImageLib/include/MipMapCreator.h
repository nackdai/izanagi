#if !defined(__IZANAGI_IMAGE_LIB_MIP_MAP_CREATOR_H__)
#define __IZANAGI_IMAGE_LIB_MIP_MAP_CREATOR_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"

namespace izanagi {
namespace tool {
	/**
	*/
	class CMipMapCreator {
	private:
		static CMipMapCreator s_cInstance;

	public:
		static CMipMapCreator* GetInstance() { return &s_cInstance; }

	private:
		CMipMapCreator() {}
		~CMipMapCreator() {}

		NO_COPIABLE(CMipMapCreator);

	public:
		// コンバート
		static void Convert(
			void* pSrc,
			void* pDst,
			IZ_UINT nWidth,
			IZ_UINT nHeight,
			graph::E_GRAPH_PIXEL_FMT nFmt);

		// 作成可能MIPMAPレベルを計算
		static IZ_UINT ComputeMaxMipLevel(
			IZ_UINT nWidth, 
			IZ_UINT nHeight, 
			graph::E_GRAPH_PIXEL_FMT nFmt);
	};
}	// namespace tool
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_IMAGE_LIB_MIP_MAP_CREATOR_H__)
