#if !defined(__IZANAGI_STD_STD_UTIL_H__)
#define __IZANAGI_STD_STD_UTIL_H__

#include "izDefs.h"

namespace izanagi {
	class CStdUtil {
	public:
		/**
		*/
		template <typename _T>
		static IZ_UINT64 GetPtrDistance(_T* p0, _T* p1)
		{
#if 0
			IZ_UINT64 nPos_0 = (IZ_UINT64)p0;
			IZ_UINT64 nPos_1 = (IZ_UINT64)p1;
#else
			IZ_UINT64 nPos_0 = ConvPtrToInt<IZ_UINT64>(p0);
			IZ_UINT64 nPos_1 = ConvPtrToInt<IZ_UINT64>(p1);
#endif

#if 1
			IZ_UINT64 nMax = IZ_MAX(nPos_0, nPos_1);
			IZ_UINT64 nMin = IZ_MIN(nPos_0, nPos_1);

			return (nMax - nMin);
#else
			IZ_ASSERT(nPos_0 <= nPos_1);
			return (nPos_1 - nPos_0);
#endif
		}

		/**
		*/
		template <typename _T>
		static IZ_UINT GetPtrAdvanced(_T* p0, _T* p1)
		{
			IZ_UINT64 nDiff = GetPtrDistance(p0, p1);
			IZ_UINT ret = static_cast<IZ_UINT>(nDiff / sizeof(_T));
			return ret;
		}

		template <typename _T>
		static void FillZero(_T* p, IZ_UINT num)
		{
			FILL_ZERO(p, sizeof(_T) * num);
		}

		template <typename _T>
		static _T ConvPtrToInt(void* p)
		{
			IZ_C_ASSERT(sizeof(_T) >= sizeof(void*));

			union {
				_T val;
				void* ptr;
			} conv;

			conv.ptr = p;
			return conv.val;
		}

		template <typename _T>
		static _T ConvPtrToInt(const void* p)
		{
			IZ_C_ASSERT(sizeof(_T) >= sizeof(const void*));

			union {
				_T val;
				const void* ptr;
			} conv;

			conv.ptr = p;
			return conv.val;
		}

		template <typename _T, typename _U>
		static _T* ConvIntToPtr(const _U& n)
		{
			IZ_C_ASSERT(sizeof(_U) >= sizeof(void*));

			union {
				_U val;
				_T* ptr;
			} conv;

			conv.val = n;
			return conv.ptr;
		}

		static IZ_UINT ComputeCRC32(IZ_BYTE* pData, IZ_UINT nSize)
		{
			// NOTE
			// EthernetのFCS(Frame Check Sequence)

			static const IZ_UINT CRCPOLY2 = 0xEDB88320UL;

			IZ_UINT r = 0xFFFFFFFFUL;

			for (IZ_UINT i = 0; i < nSize; ++i) {
				r ^= pData[i];

				for (IZ_UINT n = 0; n < CHAR_BIT; ++n) {
					if (r & 1) {
						r = (r >> 1) ^ CRCPOLY2;
					}
					else {
						r >>= 1;
					}
				}
			}

			return r ^ 0xFFFFFFFFUL;
		}

		static void Swap(IZ_UINT num, IZ_BYTE ch[4])
		{
			switch (num) {
			case 2:
				{
					IZ_BYTE tmp = ch[0];
					ch[0] = ch[1];
					ch[1] = tmp;
				}
				break;
			case 3:
				{
					IZ_BYTE tmp = ch[0];
					ch[0] = ch[2];
					ch[2] = tmp;
				}
				break;
			case 4:
				{
					IZ_BYTE tmp = ch[0];
					ch[0] = ch[3];
					ch[3] = tmp;

					tmp = ch[1];
					ch[1] = ch[2];
					ch[2] = tmp;
				}
				break;
			default:
				IZ_ASSERT(IZ_FALSE);
				break;
			}
		}
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_STD_UTIL_H__)
