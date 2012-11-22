#if !defined(__IZANAGI_COMPRESS_COMPRESSOR_H__)
#define __IZANAGI_COMPRESS_COMPRESSOR_H__

#include "izDefs.h"
#include "izIo.h"

namespace izanagi {
	class IMemoryAllocator;

	/**
	*/
	class CCompressor {
		class CEncoderOutputStreamProxy;
		friend class CEncoderOutputStreamProxy;

		static CCompressor s_cInstance;

	public:
		static CCompressor& GetInstance() { return s_cInstance; }

	private:
		CCompressor()
		{
			m_pInBuf = IZ_NULL;
			m_pOutBuf = IZ_NULL;

			m_nOrgSize = 0;
			m_nCmpSize = 0;
		}
		~CCompressor() {}

		NO_COPIABLE(CCompressor);

	public:
		IZ_BOOL Encode(
			IInputStream* pIn,
			IOutputStream* pOut,
			IMemoryAllocator* pAllocator,
			IZ_UINT nBlockSize,
			IZ_FLOAT fThreshold);

		IZ_FLOAT GetCompressRatio() const;

		IZ_BOOL Decode(
			IInputStream* pIn,
			IOutputStream* pOut,
			IMemoryAllocator* pAllocator);

	private:
		inline IZ_BOOL EncodeBlock(
			IInputStream* pIn,
			IOutputStream* pOut,
			IZ_UINT nBlockSize,
			IZ_FLOAT fThreshold,
			void* pSlideDicEncoderBuf);

	private:
		IZ_BYTE* m_pInBuf;
		CMemoryInputStream m_cInTmp;

		IZ_BYTE* m_pOutBuf;
		CMemoryOutputStream m_cOutTmp;

		IZ_UINT m_nOrgSize;
		IZ_UINT m_nCmpSize;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_COMPRESS_COMPRESSOR_H__)
