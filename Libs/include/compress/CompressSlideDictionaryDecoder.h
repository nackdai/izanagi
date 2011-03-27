#if !defined(__IZANAGI_COMPRESS_SLIDE_DICTIONARY_DECODER_H__)
#define __IZANAGI_COMPRESS_SLIDE_DICTIONARY_DECODER_H__

#include "CompressSlideDictionaryBase.h"
#include "izStd.h"
#include "izIo.h"

namespace izanagi {
	/**
	*/
	class CCompressSlideDictionaryDecoder : public CSlideDictionaryBase {
	public:
		CCompressSlideDictionaryDecoder() {}
		~CCompressSlideDictionaryDecoder() {}

		NO_COPIABLE(CCompressSlideDictionaryDecoder);

	public:
		// 初期化
		IZ_BOOL Init(
			IMemoryAllocator* pAllocator,
			IZ_UINT nDictionarySize);

		// 復号化
		IZ_BOOL Decode(
			IInputStream* pInput,
			IOutputStream* pOutput);
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_COMPRESS_SLIDE_DICTIONARY_DECODER_H__)
