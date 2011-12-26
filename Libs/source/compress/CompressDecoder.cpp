#include "compress/Compressor.h"
#include "compress/CMPFormat.h"
#include "std/allocator/MemoryAllocator.h"
#include "math/MathUtil.h"

#include "io/stream/InputStream.h"
#include "io/stream/MemoryInputStream.h"
#include "io/stream/OutputStream.h"
#include "io/stream/MemoryOutputStream.h"

#include "compress/CompressSlideDictionaryDecoder.h"
#include "compress/CompressRangeCoderDecoder.h"

using namespace izanagi;

#ifndef IZ_ENABLE_COMPRESS_ENCODER
CCompressor CCompressor::s_cInstance;
#endif

IZ_BOOL CCompressor::Decode(
	IInputStream* pIn,
	IOutputStream* pOut,
	IMemoryAllocator* pAllocator)
{
	IZ_ASSERT(pIn != IZ_NULL);
	IZ_ASSERT(pOut != IZ_NULL);
	IZ_ASSERT(pAllocator != IZ_NULL);

	IZ_BOOL ret = IZ_FALSE;
	IZ_BYTE* pBuf = IZ_NULL;
	S_CMP_HEADER sHeader;
	CMemoryInputStream cTmpIn;

	// Read CMP file's header.
	VGOTO(
		IZ_INPUT_READ(pIn, &sHeader, 0, sizeof(sHeader)),
		__EXIT__);

	// Allocate temorary buffer.
	pBuf = reinterpret_cast<IZ_BYTE*>(ALLOC_ZERO(pAllocator, sHeader.sizeBlock));
	VGOTO(pBuf != IZ_NULL, __EXIT__);

	// Temporary input.
	cTmpIn.Init(pBuf, sHeader.sizeBlock);

	for (IZ_UINT i = 0; i < sHeader.numBlock; ++i) {
		// Read block's header.
		S_CMP_BLOCK_HEADER sBlockHeader;
		VGOTO(
			IZ_INPUT_READ(pIn, &sBlockHeader, 0, sizeof(sBlockHeader)),
			__EXIT__);

		// Check block size.
		VGOTO(
			sBlockHeader.sizeCompressed < sHeader.sizeBlock,
			__EXIT__);

		// Read block data.
		VGOTO(
			IZ_INPUT_READ(pIn, pBuf, 0, sBlockHeader.sizeCompressed),
			__EXIT__);

		// Decode
		// TODO
	}

__EXIT__:
	FREE(pAllocator, pBuf);
	return ret;
}
