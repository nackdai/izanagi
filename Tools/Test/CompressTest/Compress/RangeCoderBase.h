#if !defined(__RANGE_CODER_DEFS_H__)
#define __RANGE_CODER_DEFS_H__

#include "izDefs.h"
#include "izStd.h"
#include "izIo.h"
#include "ToolkitIoDefs.h"

// NOTE
// 31----------30---------------23--------0
// | 桁上がり用 | 出力対象       | ...    |
// +------------+----------------+--------+

namespace izanagi {
	// RangeCoderのベース
	class CRangeCoderBase {
	protected:
		CRangeCoderBase() {}
		~CRangeCoderBase() {}

		NO_COPIABLE(CRangeCoderBase);

	protected:
		static const IZ_UINT RC_SHIFT = 23;
		static const IZ_UINT RC_TOP = (1 << (RC_SHIFT + 8));
		static const IZ_UINT RC_BOTTOM = (1 << RC_SHIFT);

	protected:
		// 確率表更新
		static void UpdateCount(
			IZ_BYTE nSimbol,
			IZ_UINT* pCountTbl,
			IZ_UINT nTblSize)
		{
			for (IZ_UINT i = nSimbol + 1; i < nTblSize; i++) {
				pCountTbl[i]++;
			}
		}

	protected:
		// RangeCoderパラメータ
		struct {
			IZ_UINT low;
			IZ_UINT range;
			IZ_UINT tmp;
			IZ_BYTE buf;
		} m_RCParam;
	};
}	// namespace izanagi


#endif	// #if !defined(__RANGE_CODER_DEFS_H__)
