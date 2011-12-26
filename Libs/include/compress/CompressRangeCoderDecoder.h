#if !defined(__IZANAGI_COMPRESS_RANGE_CODER_DECODER_H__)
#define __IZANAGI_COMPRESS_RANGE_CODER_DECODER_H__

#include "CompressRangeCoderBase.h"
#include "izStd.h"
#include "izIo.h"

namespace izanagi {
	/**
	*/
	class CCompressRangeCoderDecoder : public CCompressRangeCoderBase {
	public:
		struct SDesc {
			IZ_UINT compressed_size;	// 復号化前のサイズ
			IZ_UINT decode_size;		// 復号化後のサイズ
		};

	public:
		CCompressRangeCoderDecoder() {}
		~CCompressRangeCoderDecoder() {}

		NO_COPIABLE(CCompressRangeCoderDecoder);

	public:
		// 復号化
		IZ_BOOL Decode(
			SDesc* pDesc,
			IInputStream* pInput,
			IZ_UINT nTotal,
			IOutputStream* pOutput);

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

#endif	// #if !defined(__IZANAGI_COMPRESS_RANGE_CODER_DECODER_H__)
