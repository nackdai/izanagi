#if !defined(__SLIDE_DICTIONARY_DECODER_H__)
#define __SLIDE_DICTIONARY_DECODER_H__

#include "SlideDictionaryBase.h"
#include "izStd.h"
#include "ToolkitIoDefs.h"

namespace izanagi {
	/**
	*/
	class CSlideDictionaryDecoder : public CSlideDictionaryBase {
	public:
		CSlideDictionaryDecoder() {}
		~CSlideDictionaryDecoder() {}

		NO_COPIABLE(CSlideDictionaryDecoder);

	public:
		// 初期化
		IZ_BOOL Init(
			IMemoryAllocator* pAllocator,
			IZ_UINT nDictionarySize);

		// 復号化
		IZ_BOOL Decode(
			IInputStream* pInput,
			izanagi_tk::IOutputStream* pOutput);
	};
}	// namespace izanagi

#endif	// #if !defined(__SLIDE_DICTIONARY_DECODER_H__)
