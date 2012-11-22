#if !defined(__RANGE_CODER_DECODER_H__)
#define __RANGE_CODER_DECODER_H__

#include "RangeCoderBase.h"
#include "ToolkitIoDefs.h"
#include "izStd.h"

namespace izanagi {
	/**
	*/
	class CRangeCoderDecoder : public CRangeCoderBase {
	public:
		struct SDesc {
			IZ_UINT compressed_size;	// 復号化前のサイズ
			IZ_UINT decode_size;		// 復号化後のサイズ
		};

	public:
		CRangeCoderDecoder() {}
		~CRangeCoderDecoder() {}

		NO_COPIABLE(CRangeCoderDecoder);

	public:
		// 復号化
		IZ_BOOL Decode(
			SDesc* pDesc,
			IInputStream* pInput,
			IZ_UINT nTotal,
			izanagi_tk::IOutputStream* pOutput);

	protected:
		// シンボルの出現位置を計算する
		IZ_UINT ComputeSimbolPos(
			IInputStream* pInput,
			IZ_UINT nTotal);

		// Low と Range の正規化
		IZ_BOOL Normalize(IInputStream* pInput);

		// 復号化
		void Decode(
			IZ_UINT nLow,
			IZ_UINT nRange,
			IZ_UINT nTotal);
	};
}	// namespace izanagi

#endif	// #if !defined(__RANGE_CODER_DECODER_H__)
