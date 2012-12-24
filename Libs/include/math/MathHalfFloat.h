#if !defined(__IZANAGI_MATH_MATH_HALF_FLOAT_H__)
#define __IZANAGI_MATH_MATH_HALF_FLOAT_H__

#include "izStd.h"

namespace izanagi
{
namespace math
{
    // NOTE
    // 単精度浮動小数
    // +-----------+-----------+-----------+
    // | 符号部(s) | 指数部(e) | 仮数部(m) |
    // +-----------+-----------+-----------+
    //      31      30       23 22         0
    // 指数部 : 23bit / 指数部 : 8bit / 符号部 : 1bit
    // 
    // ((-1)^s) * (2 ^ (e - 127)) * (1 + m / (2 ^ 10))
    //
    // 半精度浮動小数
    // +-----------+-----------+-----------+
    // | 符号部(s) | 指数部(e) | 仮数部(m) |
    // +-----------+-----------+-----------+
    //      15      14       10 9          0
    // 指数部 : 10bit / 指数部 : 5bit / 符号部 : 1bit
    // 
    // ((-1)^s) * (2 ^ (e - 15)) * (1 + m / (2 ^ 10))

    /** 半精度浮動小数
     */
    class CFloat16
    {
    private:
        enum
        {
            HALF_FLOAT_EXP_MAX = 0x1f,
            HALF_FLOAT_EXP_MAX_SHIFT = HALF_FLOAT_EXP_MAX << 10,

            HALF_FLOAT_FRACTION_MAX = ((1 << 10) - 1),

            FLOAT_EXP_MAX = 0xff,
            FLOAT_EXP_MAX_SHIFT = FLOAT_EXP_MAX << 23,

            FLOAT_FRACTION_MAX = ((1 << 23) - 1),
        };

    public:
        /** 
         */
        static IZ_UINT16 Float32ToFloat16(IZ_FLOAT f)
        {
            IZ_UINT u = *(IZ_UINT*)&f;

            IZ_INT sign = (u >> 31);                    // 符号部
            IZ_INT expo = (u >> 23) & FLOAT_EXP_MAX;    // 指数部
            IZ_INT frac = u & FLOAT_FRACTION_MAX;       // 仮数部

            expo = expo - 127;
            expo = expo + 15;

            IZ_UINT16 ret = 0;

            if (expo < 0)
            {
                // 負の値になるので表現できない

                // TODO
                // とりあえず、-0.0 or 0.0 にする
                ret = (sign << 15);
            }
            else if (expo > 31)
            {
                // 指数部 5bit に入りきらない
                
                if ((frac != 0) && (expo == FLOAT_EXP_MAX))
                {
                    // NaN
                    ret = (sign << 15)
                            | HALF_FLOAT_EXP_MAX_SHIFT
                            | HALF_FLOAT_FRACTION_MAX;
                }
                else
                {
                    // とりあえず無限大にする
                    ret = (sign << 15)
                            | HALF_FLOAT_EXP_MAX_SHIFT;
                }
            }
            else
            {
                ret = (sign << 15)
                        | ((expo << 10) & 0x7c00)
                        | (frac >> 13);
            }

            return ret;
        }

        /**
         */
        static IZ_FLOAT Float16ToFloat32(const CFloat16& f16)
        {
            IZ_INT sign = f16.m_Sign;
            IZ_INT expo = f16.m_Exp;
            IZ_INT frac = f16.m_Frac;

            IZ_UINT u = 0;

            if (expo == HALF_FLOAT_EXP_MAX)
            {
                // Inifinity
                expo = FLOAT_EXP_MAX;

                if (frac != 0)
                {
                    // NaN
                    // すべてのビットを１にする
                    frac = FLOAT_FRACTION_MAX;
                }
            }
            else if (expo == 0)
            {
                if (frac > 0)
                {
                    // TODO
                    // 非正規化数

                    // とりあえず非正規数になるようにしておく
                    frac = 1;
                }
                else
                {
                    // 0.0
                    frac = 0;
                }
            }
            else
            {
                expo = expo - 15;
                expo = expo + 127;

                frac <<= 13;
            }

            u = (sign << 31) | ((expo & FLOAT_EXP_MAX) << 23) | frac;

            IZ_FLOAT f = *(IZ_FLOAT*)&u;

            return f;
        }

    public:
        CFloat16()
        {
            m_Val = 0;
        }

        CFloat16(IZ_UINT16 u)
        {
            m_Val = u;
        }

        CFloat16(IZ_FLOAT f)
        {
            m_Val = Float32ToFloat16(f);
        }

        CFloat16(const CFloat16& rhs)
        {
            m_Val = rhs.m_Val;
        }

        ~CFloat16() {}

    public:
        operator IZ_FLOAT()
        {
            IZ_FLOAT ret = Float16ToFloat32(*this);
            return ret;
        }

        operator IZ_UINT16()
        {
            return m_Val;
        }

        const CFloat16& operator=(const CFloat16& rhs)
        {
            m_Val = rhs.m_Val;
            return *this;
        }

        const CFloat16& operator=(IZ_FLOAT rhs)
        {
            SetFloat(rhs);
            return *this;
        }

        IZ_BOOL operator==(const CFloat16& rhs)
        {
            return (m_Val == rhs.m_Val);
        }

        IZ_BOOL operator!=(const CFloat16& rhs)
        {
            return !(*this == rhs);
        }

        IZ_UINT16 GetVal() const { return m_Val; }

        void SetFloat(IZ_FLOAT value)
        {
            m_Val = Float32ToFloat16(value);
        }

    private:
        union {
            IZ_UINT16 m_Val;
            struct {
                IZ_UINT16 m_Frac    : 10;   // 仮数部
                IZ_UINT16 m_Exp     : 5;    // 指数部
                IZ_UINT16 m_Sign    : 1;    // 符号部
            };
        };
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_MATH_MATH_HALF_FLOAT_H__)
