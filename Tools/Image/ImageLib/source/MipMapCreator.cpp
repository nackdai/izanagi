#include "MipMapCreator.h"
#include "ImageLibUtility.h"

using namespace izanagi;
using namespace tool;

CMipMapCreator CMipMapCreator::s_cInstance;

namespace {
	template <class _T>
	struct MipMapFunc {
		void operator()(
			_T* pSrc, 
			_T* pDst,
			IZ_UINT nSrcWidth)
		{
			IZ_FLOAT r = 0.0f;
			IZ_FLOAT g = 0.0f;
			IZ_FLOAT b = 0.0f;
			IZ_FLOAT a = 0.0f;

			IZ_UINT nPos = 0;
			IZ_FLOAT a_array[4] = {0.0f, 0.0f, 0.0f, 0.0f};

			for (IZ_UINT y = 0; y < 2; y++) {
				_T* src = pSrc + nSrcWidth * y;

				for (IZ_UINT x = 0; x < 2; x++) {
					r += src->r;
					g += src->g;
					b += src->b;
					a += src->a;

					a_array[nPos++] = src->a;

					src++;
				}
			}

			IZ_FLOAT fDiv = (a > 0.0f ? 1.0f / a : 1.0f);

			pDst->r = static_cast<IZ_BYTE>(r * a_array[0] * fDiv);
			pDst->g = static_cast<IZ_BYTE>(g * a_array[1] * fDiv);
			pDst->b = static_cast<IZ_BYTE>(b * a_array[2] * fDiv);
			pDst->a = static_cast<IZ_BYTE>(a * 0.25f);
		}
	};

	// A8
	template <>
	struct MipMapFunc<S_A8_VAL> {
		void operator()(
			S_A8_VAL* pSrc, 
			S_A8_VAL* pDst,
			IZ_UINT nSrcWidth)
		{
			IZ_UINT a = 0;

			for (IZ_UINT y = 0; y < 2; y++) {
				S_A8_VAL* src = pSrc + nSrcWidth * y;

				for (IZ_UINT x = 0; x < 2; x++) {
					a += src->a;
					src++;
				}
			}

			pDst->a = a >> 2;
		}
	};

	// R32F
	template <>
	struct MipMapFunc<S_R32F_VAL> {
		void operator()(
			S_R32F_VAL* pSrc,
			S_R32F_VAL* pDst,
			IZ_UINT nSrcWidth)
		{
			IZ_FLOAT r = 0;

			for (IZ_UINT y = 0; y < 2; y++) {
				S_R32F_VAL* src = pSrc + nSrcWidth * y;

				for (IZ_UINT x = 0; x < 2; x++) {
					r += src->r;
					src++;
				}
			}

			pDst->r = r * 0.25f;
		}
	};
}	// namespace

namespace {
	template <typename _T, template <typename _T> class _Func>
	void _CreateMipMap(
		void* pSrc,
		void* pDst,
		IZ_UINT nDstWidth,
		IZ_UINT nDstHeight,
		_Func<_T>& func)
	{
		IZ_UINT nSrcWidth = nDstWidth << 1;

		for (IZ_UINT y = 0; y < nDstHeight; y++) {
			_T* src = reinterpret_cast<_T*>(pSrc) + nDstWidth * (y << 1);
			_T* dst = reinterpret_cast<_T*>(pDst) + nDstWidth * y;

			for (IZ_UINT x = 0; x < nDstWidth; x++) {
				func(src, dst, nSrcWidth);

				src += 2;
				dst++;
			}
		}
	}
}	// namespace

/**
* コンバート
*/
void CMipMapCreator::Convert(
	void* pSrc,
	void* pDst,
	IZ_UINT nWidth,
	IZ_UINT nHeight,
	graph::E_GRAPH_PIXEL_FMT nFmt)
{
	if (CImageUtil::IsDXT(nFmt)) {
		// TODO
		// DXTは特殊・・・
	}
	else if (nFmt == graph::E_GRAPH_PIXEL_FMT_A8) {
		// A8も特殊・・・
		_CreateMipMap(
			pSrc,
			pDst,
			nWidth, nHeight,
			MipMapFunc<S_A8_VAL>());
	}
	else if (nFmt == graph::E_GRAPH_PIXEL_FMT_R32F) {
		// R32Fも特殊・・・
		_CreateMipMap(
			pSrc,
			pDst,
			nWidth, nHeight,
			MipMapFunc<S_R32F_VAL>());
	}
	else if (nFmt == graph::E_GRAPH_PIXEL_FMT_RGBA16F) {
		// RGBA16Fも特殊・・・
	}
	else if (nFmt == graph::E_GRAPH_PIXEL_FMT_RGBA32F) {
		// RGBA32Fも特殊・・・
		_CreateMipMap(
			pSrc,
			pDst,
			nWidth, nHeight,
			MipMapFunc< S_PIXEL_VAL<IZ_FLOAT> >());
	}
	else {
		_CreateMipMap(
			pSrc,
			pDst,
			nWidth, nHeight,
			MipMapFunc< S_PIXEL_VAL<IZ_BYTE> >());
	}
}

/**
* 作成可能MIPMAPレベルを計算
*/
IZ_UINT CMipMapCreator::ComputeMaxMipLevel(
	IZ_UINT nWidth, 
	IZ_UINT nHeight, 
	graph::E_GRAPH_PIXEL_FMT nFmt)
{
	IZ_UINT ret = 1;

	IZ_UINT nMinWidth = (CImageUtil::IsDXT(nFmt) ? 4 : 1);
	IZ_UINT nMinHeight = (CImageUtil::IsDXT(nFmt) ? 4 : 1);

	for (IZ_UINT i = 1; ; i++) {
		IZ_UINT nW = nWidth >> i;
		IZ_UINT nH = nHeight >> i;

		if ((nW < nMinWidth) || (nH < nMinHeight)) {
			ret = i;
			break;
		}
	}

	return ret;
}
