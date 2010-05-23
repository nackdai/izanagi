#if !defined(__URANUS_STD_STD_UTIL_H__)
#define __URANUS_STD_STD_UTIL_H__

#include "unDefs.h"

namespace uranus {
	class CStdUtil {
	public:
		/**
		*/
		template <typename _T>
		static UN_UINT64 GetPtrDiff(_T* p0, _T* p1)
		{
#if 0
			UN_UINT64 nPos_0 = (UN_UINT64)p0;
			UN_UINT64 nPos_1 = (UN_UINT64)p1;
#else
			UN_UINT64 nPos_0 = ConvPtrToInt<UN_UINT64>(p0);
			UN_UINT64 nPos_1 = ConvPtrToInt<UN_UINT64>(p1);
#endif

#if 1
			UN_UINT64 nMax = MAX(nPos_0, nPos_1);
			UN_UINT64 nMin = MIN(nPos_0, nPos_1);

			return (nMax - nMin);
#else
			UN_ASSERT(nPos_0 <= nPos_1);
			return (nPos_1 - nPos_0);
#endif
		}

		/**
		*/
		template <typename _T>
		static UN_UINT GetPtrAdvanced(_T* p0, _T* p1)
		{
			UN_UINT64 nDiff = GetPtrDiff(p0, p1);
			UN_UINT ret = static_cast<UN_UINT>(nDiff / sizeof(_T));
			return ret;
		}

		template <typename _T>
		static void FillZero(_T* p, UN_UINT num)
		{
			FILL_ZERO(p, sizeof(_T) * num);
		}

		template <typename _T>
		static _T ConvPtrToInt(void* p)
		{
			UN_C_ASSERT(sizeof(_T) >= sizeof(void*));

			union {
				_T val;
				void* ptr;
			} conv;

			conv.ptr = p;
			return conv.val;
		}

		template <typename _T, typename _U>
		static _T* ConvIntToPtr(const _U& n)
		{
			UN_C_ASSERT(sizeof(_U) >= sizeof(void*));

			union {
				_U val;
				_T* ptr;
			} conv;

			conv.val = n;
			return conv.ptr;
		}

		static UN_UINT ComputeCRC32(UN_BYTE* pData, UN_UINT nSize)
		{
			// NOTE
			// Ethernet‚ÌFCS(Frame Check Sequence)

			static const UN_UINT CRCPOLY2 = 0xEDB88320UL;

			UN_UINT r = 0xFFFFFFFFUL;

			for (UN_UINT i = 0; i < nSize; i++) {
				r ^= pData[i];

				for (UN_UINT n = 0; n < CHAR_BIT; n++) {
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
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_STD_UTIL_H__)
