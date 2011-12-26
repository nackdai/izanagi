	#if !defined(__IZANAGI_MATH_MATH_HALF_FLOAT_H__)
#define __IZANAGI_MATH_MATH_HALF_FLOAT_H__

namespace izanagi {
	// NOTE
	// 半精度浮動小数
	// +-----------+-----------+-----------+
	// | 符号部(s) | 指数部(e) | 仮数部(m) |
	// +-----------+-----------+-----------+
	//      15      14       10 9          0
	// 
	// ((-1)^s) * (2 ^ (e - 15)) * (1 + m / (2 ^ 10))

	/**
	* 半精度浮動小数
	*/
	class CHalfFloat {
	public:
		enum {
			ZERO = 0x0000,	// 0.0 -> 0 00000 0000000000
			ONE  = 0x3c00,	// 1.0 -> 0 01111 0000000000

			PLUS_INFINITE  = 0x7c00,	// 正の無限大 -> 0 11111 0000000000
			MINUS_INFINITE = 0xfc00,	// 負の無限大 -> 1 11111 0000000000

			NaN_0 = 0x7c18,	// Not a Number -> 0 11111 0000011000
			Nan_1 = 0xffff,	// Not a Number -> 1 11111 1111111111

			UN_NORMALIZED = 0x000f,	// 非正規化数 -> 0 00000 0000001111
		};

		static IZ_UINT16 FloatToHalfFloat(IZ_FLOAT f)
		{
			IZ_UINT nF = *(IZ_UINT*)&f;
			IZ_UINT nSign = (x >> 31);			// 符号部
			IZ_UINT nFrac = x & 0x007fffff;		// 仮数部
			IZ_UINT nExp = (x >> 23) & 0xff;	// 指数部

			// TODO
			return 0;
		}

	public:
		CHalfFloat(IZ_UINT16 n);
		CHalfFloat(UN_FLOAT f);
		CHalfFloat(const CHalfFloat& rhs);

		~CHalfFloat();

	public:
		operator IZ_FLOAT();

		IZ_UINT16 GetVal() const { return m_Val; }

	private:
		union {
			IZ_UINT16 m_Val;
			struct {
				IZ_UINT16 m_Frac	: 10	// 仮数部
				IZ_UINT16 m_Exp		: 5;	// 指数部
				IZ_UINT16 m_Sign	: 1;	// 符号部
			};
		};
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_MATH_MATH_HALF_FLOAT_H__)
