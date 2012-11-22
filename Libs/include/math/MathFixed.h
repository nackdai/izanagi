#if !defined(__IZANAGI_MATH_MATH_FIXED_H__)
#define __IZANAGI_MATH_MATH_FIXED_H__

#include "izDefs.h"

namespace izanagi {
	// NOTE
	// 単精度浮動小数
	// +-----------+-----------+-----------+
	// | 符号部(s) | 指数部(e) | 仮数部(m) |
	// +-----------+-----------+-----------+
	//      31      30       23 22         0
	// 
	// ((-1)^s) * (2 ^ (e - 127)) * (1 + m / (2 ^ 23))

	/**
	* 固定小数
	*/
	template <
		typename _T,
		IZ_UINT INT_BIT,
		IZ_UINT DECIMAL_BIT
	>
	class CFixedPoint {

		enum {
			BIT_CNT = COUNT_BIT(_T),
			SIGN_SHIFT = BIT_CNT - 1,
			DEC_MASK = (1 << DECIMAL_BIT) - 1,
			INT_MASK = (1 << INT_BIT) - 1,
		};

		static _T FloatToFixed(
			IZ_UINT nInt,
			IZ_UINT nSign,
			IZ_INT nExp,
			IZ_UINT nFraction)
		{
			nExp -= 127;

			IZ_UINT32 nDecimal = 0;

			if (nExp > 0) {
				// 精度は23bit
				nDecimal = (nFraction << nExp) & 0x7fffff;
			}
			else if (nExp < 0) {
				nExp = -nExp;
				nDecimal = (nFraction >> nExp) | (0x01 << 22);
			}
			else {
				nDecimal = nFraction;
			}

			nDecimal = (nDecimal >> (23 - DECIMAL_BIT)) & DEC_MASK;

			_T ret = (nSign << SIGN_SHIFT) | ((nInt & INT_MASK) << DECIMAL_BIT) | nDecimal;

			return ret;
		}

		static _T FloatToFixed(IZ_FLOAT f)
		{
			IZ_UINT nVal = *(IZ_UINT*)&f;

			// TODO
			// Check whether float value is normal.

			IZ_UINT nInt = (IZ_UINT)CMath::Absf(f);
			IZ_UINT nSign = nVal >> 31;
			IZ_INT nExp = (nVal >> 23) & 0xff;
			IZ_UINT nFraction = nVal & 0x7fffff;

			_T ret = FloatToFixed(
						nInt,
						nSign,
						nExp,
						nFraction);

			return ret;
		}

		// １が立っているビットの最上位位置を取得
		static IZ_INT GetValidBitTop(IZ_UINT nVal)
		{
			IZ_INT ret = 0;
			IZ_UINT nMask = 0x80000000;

			for (ret = COUNT_BIT(IZ_UINT32) - 1; ret >= 0; ret--) {
				if ((nVal & nMask) > 0) {
					break;
				}
				nMask >>= 1; 
			}

			return ret;
		}

		static IZ_FLOAT FixedToFloat(
			IZ_UINT nSign,
			IZ_UINT nInt,
			IZ_UINT nDecimal)
		{
			static union {
				IZ_UINT32 val;
				struct {
					IZ_UINT32 frac	: 23;
					IZ_UINT32 exp	: 8;
					IZ_UINT32 sign	: 1;
				};
			} sTmp;

			sTmp.sign = nSign;

			IZ_INT nDecBitTop = GetValidBitTop(nDecimal);
			IZ_ASSERT(nDecBitTop >= 0);

			IZ_UINT64 nFracTmp = 0;

			if (nInt > 0) {
				// Compute exponent.
				IZ_INT nExp = GetValidBitTop(nInt);
				IZ_ASSERT(nExp >= 0);

				sTmp.exp = nExp + 127;

				// Compute fraction.
				nFracTmp = (nInt << (nDecBitTop + 1)) | nDecimal;
				nFracTmp &= (1 << (nExp + nDecBitTop + 1)) - 1;
			}
			else if (nDecimal > 0) {
				// Compute exponent.
				IZ_INT nExp = nDecBitTop;

				sTmp.exp = (nExp < 0 ? 0 : -nExp) + 127;

				// Compute fraction.
				nFracTmp = nDecimal & ((1 << nDecBitTop) - 1);
				nFracTmp = nFracTmp << (23 - nDecBitTop);
			}

			sTmp.frac = static_cast<IZ_UINT32>(nFracTmp);

			IZ_FLOAT ret = *(IZ_FLOAT*)&sTmp.val;
			return ret;
		}

	public:
		CFixedPoint(IZ_FLOAT f)
		{
			m_Val = FloatToFixed(f);
		}
		CFixedPoint(_T n)
		{
			m_Val = n;
		}
		CFixedPoint(const CFixedPoint& rhs)
		{
			m_Val = rhs.m_Val;
		}

		~CFixedPoint() {}

	public:
		const CFixedPoint& operator=(IZ_FLOAT rhs)
		{
			m_Val = _FloatToFixed<_T>(rhs);
			return *this;
		}
		const CFixedPoint& operator=(_T rhs)
		{
			m_Val = rhs;
			return *this;
		}
		const CFixedPoint& operator=(const CFixedPoint& rhs)
		{
			m_Val = rhs.m_Val;
			return *this;
		}

		IZ_BOOL operator==(_T rhs)
		{
			return (m_Val == rhs);
		}
		IZ_BOOL operator==(const CFixedPoint& rhs)
		{
			return (m_Val == rhs.m_Val);
		}

		IZ_BOOL operator!=(_T rhs)
		{
			return !(*this == rhs);
		}
		IZ_BOOL operator!=(const CFixedPoint& rhs)
		{
			return !(*this == rhs);
		}

		// TODO
		/*
		const CFixedPoint& operator+=(_T rhs)
		{
			return *this += CFixedPoint(rhs);
		}
		const CFixedPoint& operator+=(const CFixedPoint& rhs)
		{
			// TODO
			return *this;
		}

		const CFixedPoint& operator-=(_T rhs)
		{
			return *this -= CFixedPoint(rhs);
		}
		const CFixedPoint& operator-=(const CFixedPoint& rhs)
		{
			// TODO
			return *this;
		}

		const CFixedPoint& operator*=(_T rhs)
		{
			return *this *= CFixedPoint(rhs);
		}
		const CFixedPoint& operator*=(const CFixedPoint& rhs)
		{
			// TODO
			return *this;
		}

		const CFixedPoint& operator/=(_T rhs)
		{
			return *this /= CFixedPoint(rhs);
		}
		const CFixedPoint& operator/=(const CFixedPoint& rhs)
		{
			// TODO
			return *this;
		}
		*/

		// TODO
		/*
		const CFixedPoint& operator+(_T a);
		const CFixedPoint& operator+(const CFixedPoint& a);

		const CFixedPoint& operator-(_T a);
		const CFixedPoint& operator-(const CFixedPoint& a);

		const CFixedPoint& operator*(_T a);
		const CFixedPoint& operator*(const CFixedPoint& a);

		const CFixedPoint& operator/(_T a);
		const CFixedPoint& operator/(const CFixedPoint& a);
		*/

		operator IZ_FLOAT()
		{
			IZ_FLOAT ret = GetValAsFloat();
			return ret;
		}

		operator IZ_UINT32() { return m_Val; }
		operator IZ_UINT64() { return m_Val; }

		IZ_FLOAT GetValAsFloat() const
		{
			IZ_FLOAT ret = FixedToFloat(
							m_Sign,
							m_Int,
							m_Dec);
			return ret;
		}
		_T GetVal() const { m_Val; }

	private:
		union {
			_T m_Val;
			struct {
				_T m_Dec	: DECIMAL_BIT;	// 小数部
				_T m_Int	: INT_BIT;		// 整数部
				_T m_Sign	: 1;			// 符号部
			};
		};

		IZ_C_ASSERT(COUNT_BIT(_T) == 1 + INT_BIT + DECIMAL_BIT);
	};

	typedef CFixedPoint<IZ_UINT16, 7, 8>	CFixed16;
	typedef CFixedPoint<IZ_UINT32, 15, 16>	CFixed32;
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_MATH_MATH_FIXED_H__)
