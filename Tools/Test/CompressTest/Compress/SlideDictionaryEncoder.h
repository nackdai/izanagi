#if !defined(__SLIDE_DICTIONARY_ENCODER_H__)
#define __SLIDE_DICTIONARY_ENCODER_H__

#include "SlideDictionaryBase.h"
#include "izStd.h"
#include "ToolkitIoDefs.h"

namespace izanagi {
	/**
	*/
	class CSlideDictionaryEncoder : public CSlideDictionaryBase {
	public:
		struct SDesc {
			IZ_UINT org_size;			// 元のサイズ
			IZ_UINT compressed_size;	// 圧縮後のサイズ
		};

	public:
		CSlideDictionaryEncoder() {}
		~CSlideDictionaryEncoder() {}

		NO_COPIABLE(CSlideDictionaryEncoder);

	public:
		// 初期化
		IZ_BOOL Init(
			IMemoryAllocator* pAllocator,
			IZ_UINT nDictionarySize,
			IZ_UINT nMaxMatchLen);

		// 圧縮
		IZ_BOOL Encode(
			SDesc* pDesc,
			IInputStream* pInput,
			izanagi_tk::IOutputStream* pOutput);

	private:
		// 処理位置を次の位置に移動
		IZ_BOOL Next(IInputStream* pInput);

		// データ読み込み
		inline IZ_UINT ReadData(IInputStream* pInput);

		struct SMatchInfo {
			IZ_UINT16 len;		// 一致文字列長
			IZ_UINT16 dist;		// 一致文字列までの相対距離
		};

		// 辞書内を探す
		void Search(SMatchInfo& refMatchInfo);
	};
}	// namespace izanagi

#endif	// #if !defined(__SLIDE_DICTIONARY_ENCODER_H__)
