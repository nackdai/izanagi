#if !defined(__RANGE_CODER_ENCODER_H__)
#define __RANGE_CODER_ENCODER_H__

#include "RangeCoderBase.h"
#include "izStd.h"

namespace izanagi {
	/**
	*/
	class CRangeCoderEncoder : public CRangeCoderBase {
	public:
		struct SDesc {
			IZ_UINT org_size;			// 元のサイズ
			IZ_UINT compressed_size;	// 圧縮後のサイズ
		};

	public:
		CRangeCoderEncoder()
		{
			m_pAllocator = IZ_NULL;

			m_pBuffer = IZ_NULL;
			m_nBufferSize = 0;

			m_bIsFirstOutByte = IZ_TRUE;
		}

		~CRangeCoderEncoder() { Clear(); }

		NO_COPIABLE(CRangeCoderEncoder);

	public:
		// 初期化
		IZ_BOOL Init(
			IMemoryAllocator* pAllocator,
			IZ_UINT nBufferSize);

		void Clear();

		// 符号化
		IZ_BOOL Encode(
			SDesc* pDesc,
			IInputStream* pInput,
			izanagi_tk::IOutputStream* pOutput);

		// フラッシュ
		IZ_BOOL Flush(izanagi_tk::IOutputStream* pOutput);

	private:
		// 符号化
		IZ_BOOL Encode(
			IZ_UINT nTotal,
			IZ_UINT nLow,
			IZ_UINT nRange,
			izanagi_tk::IOutputStream* pOutput);

		// Low と Range の正規化
		IZ_BOOL Normalize(izanagi_tk::IOutputStream* pOutput);

		IZ_BOOL IsInitialized()
		{
			return ((m_pBuffer != IZ_NULL) && (m_nBufferSize > 0));
		}

	private:
		IMemoryAllocator* m_pAllocator;

		IZ_BYTE* m_pBuffer;
		IZ_UINT m_nBufferSize;

		IZ_BOOL m_bIsFirstOutByte;
	};
}	// namespace izanagi

#endif	// #if !defined(__RANGE_CODER_ENCODER_H__)
