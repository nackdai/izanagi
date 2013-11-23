#if !defined(__IZANAGI_STD_2D_RECT_H__)
#define __IZANAGI_STD_2D_RECT_H__

#include "izDefs.h"
#include "2DPoint.h"

namespace izanagi {
#if 0
    /**
    */
    template <typename _T>
    struct S2DRect {
        union {
            struct {
                _T left, top, right, bottom;
            };
            struct {
                S2DPoint<_T> left_top;
                S2DPoint<_T> right_bottom;
            };
        };
    };

    /**
    */
    template <typename _T>
    class C2DRect : public S2DRect<_T> {
    public:
        C2DRect()
        {
            left = top = right = bottom = (_T)0;
        }

        ~C2DRect() {}

        C2DRect(_T l, _T t, _T r, _T b)
        {
            left = l;
            top = t;
            right = r;
            bottom = b;
        }
        C2DRect(const S2DRect<_T>& rhs)
        {
            left = rhs.left;
            top = rhs.top;
            right = rhs.right;
            bottom = rhs.bottom;
        }

    public:
        // 左上座標を返す
        S2DPoint<_T>& TopLeft()
        {
            return left_top;
        }
        const S2DPoint<_T>& TopLeft() const
        {
            return left_top;
        }

        // 右下座標を返す
        S2DPoint<_T>& BottomRight()
        {
            return right_bottom;
        }
        const S2DPoint<_T>& BottomRight() const
        {
            return right_bottom;
        }

        // 中心座標を返す
        S2DPoint<_T> CenterPoint() const
        {
            S2DPoint<_T> ret;
            ret.x = (left + right) / 2;
            ret.y = (top + bottom) / 2;

            return ret;
        }

        // コピー元の四角形の寸法をコピー
        void CopyRect(const S2DRect<_T>& rc)
        {
            left = rc.left;
            top = rc.top;
            right = rc.right;
            bottom = rc.bottom;
        }

        // 寸法を設定
        void SetRect(_T l, _T t, _T r, _T b)
        {
            left = l;
            top = t;
            right = r;
            bottom = b;
        }

        void SetRect(
            const S2DPoint<_T>& ptTopLeft,
            const S2DPoint<_T>& ptRightBottom)
        {
            left = ptTopLeft.x;
            top = ptTopLeft.y;
            right = ptRightBottom.x;
            bottom = ptRightBottom.y;
        }

        // 空の四角形に設定(すべての座標を0にする)
        void SetRectEmpty()
        {
            left = top = right = bottom = (_T)0;
        }

        // 幅と高さを減少
        void DeflateRect(_T x, _T y)
        {
            left += x;
            top += y;
            right -= x;
            bottom -= y;
        }
        void DeflateRect(_T l, _T t, _T r, _T b)
        {
            left += l;
            top += t;
            right -= r;
            bottom -= b;
        }
        void DeflateRect(const S2DRect<_T>& rc)
        {
            left += rc.left;
            top += rc.top;
            right -= rc.right;
            bottom -= rc.bottom;
        }

        // 幅と高さを増加
        void InflateRect(_T x, _T y)
        {
            left -= x;
            top -= y;
            right += x;
            bottom += y;
        }
        void InflateRect(_T l, _T t, _T r, _T b)
        {
            left -= l;
            top -= t;
            right += r;
            bottom += b;
        }
        void InflateRect(const S2DRect<_T>& rc)
        {
            left -= rc.left;
            top -= rc.top;
            right += rc.right;
            bottom += rc.b
        }

        // 指定された x 座標に移動
        void MoveToX(_T x)
        {
            _T width = Width();
            left = x;
            right = left + width;
        }

        // 指定された y 座標
        void MoveToY(_T y);

        // 指定された x 座標と y 座標に移動
        void MoveToXY(_T x, _T y)
        {
            _T height = Height();
            top = y;
            bottom = top + height;
        }
        void MoveToXY(const S2DPoint<_T>& pt)
        {
            MoveToX(x);
            MoveToY(y);
        }

        // 指定されたオフセットで移動
        void OffsetRect(_T x, _T y)
        {
            left += x;
            top += y;
            right += x;
            bottom += y;
        }
        void OffsetRect(const S2DPoint<_T>& pt)
        {
            OffsetRect(pt.x, pt.y);
        }

        // 幅を取得
        _T Width() const
        {
            return (right - left);
        }

        // 高さを取得
        _T Height() const
        {
            return (bottom - top);
        }

        void SetWidth(_T width)
        {
            right = left + width;
        }

        void SetHeight(_T height)
        {
            bottom = top + height;
        }

        // 空かどうかを調べる(高さと幅のどちらかまたはその両方が 0 のときに空になる)
        IZ_BOOL IsRectEmpty() const
        {
            return ((left == right) || (top == bottom));
        }

        // メンバ変数 top、bottom、left、right がすべて 0 かどうかを調べる
        IZ_BOOL IsRectNull() const
        {
            return ((left == 0) && (top == 0) && (right == 0) && (bottom == 0));
        }

        // 指定された四角形と等しいかどうか
        IZ_BOOL EqualRect(const S2DRect<_T>& rc) const
        {
            return ((left == rc.left)
                    && (top == rc.top)
                    && (right == rc.right)
                    && (bottom == rc.bottom));
        }

        // 指定された点が矩形内にあるかどうかを調べる
        IZ_BOOL PtInRect(_T x, _T y) const
        {
            return (((left <= x) && (x < right))
                    && ((top <= y) && (y < bottom)));
        }
        IZ_BOOL PtInRect(const S2DPoint<_T>& pt) const
        {
            return PtInRect(pt.x, pt.y);
        }

    public:
        IZ_BOOL operator!=(const S2DRect<_T>& rhs) const
        {
            return !EqualRect(rhs);
        }

        IZ_BOOL operator==(const S2DRect<_T>& rhs) const
        {
            return EqualRect(rhs);
        }

        const C2DRect& operator=(const S2DRect<_T>& rhs)
        {
            CopyRect(rhs);
            return *this;
        }
    };

    //////////////////////////////////////////////////////////
    typedef S2DRect<IZ_INT> SIntRect;
    typedef C2DRect<IZ_INT> CIntRect;
    typedef S2DRect<IZ_FLOAT> SFloatRect;
    typedef C2DRect<IZ_FLOAT> CFloatRect;
#else
    struct SIntRect {
        IZ_INT left, top, width, height;
    };

    class CIntRect : public SIntRect {
    public:
        CIntRect()
        {
            left = top = width = height = 0;
        }

        ~CIntRect() {}

        CIntRect(IZ_INT l, IZ_INT t, IZ_INT w, IZ_INT h)
        {
            left = l;
            top = t;
            width = w;
            height = h;
        }
        CIntRect(const SIntRect& rhs)
        {
            CopyRect(rhs);
        }

    public:
        IZ_INT right() const
        {
            return left + width;
        }

        IZ_INT bottom() const
        {
            return top + height;
        }

        // 指定された四角形と等しいかどうか
        IZ_BOOL EqualRect(const SIntRect& rc) const
        {
            return ((left == rc.left)
                    && (top == rc.top)
                    && (width == rc.width)
                    && (height == rc.height));
        }

        // コピー元の四角形の寸法をコピー
        void CopyRect(const SIntRect& rc)
        {
            left = rc.left;
            top = rc.top;
            width = rc.width;
            height = rc.height;
        }

        // 指定されたオフセットで移動
        void OffsetRect(IZ_INT x, IZ_INT y)
        {
            left += x;
            top += y;
        }

        static void InflateRect(
            SIntRect& rc,
            IZ_INT l, IZ_INT t, IZ_INT r, IZ_INT b)
        {
            rc.left -= l;
            rc.top -= t;

            rc.width = rc.width + l + r;
            rc.height = rc.height + t + b;
        }

        static void InflateRect(
            SIntRect& dst,
            const SIntRect& src,
            IZ_INT l, IZ_INT t, IZ_INT r, IZ_INT b)
        {
            dst.left = src.left - l;
            dst.top = src.top - t;

            dst.width = src.width + l + r;
            dst.height = src.height + t + b;
        }

        static IZ_INT right(const SIntRect& rc)
        {
            return rc.left + rc.width;
        }

        static IZ_INT bottom(const SIntRect& rc)
        {
            return rc.top + rc.height;
        }

     public:
        IZ_BOOL operator!=(const SIntRect& rhs) const
        {
            return !EqualRect(rhs);
        }

        IZ_BOOL operator==(const SIntRect& rhs) const
        {
            return EqualRect(rhs);
        }

        const CIntRect& operator=(const SIntRect& rhs)
        {
            CopyRect(rhs);
            return *this;
        }
    };

    struct SFloatRect {
        IZ_FLOAT left, top, right, bottom;
    };

    class CFloatRect : public SFloatRect {
    public:
        CFloatRect()
        {
            left = top = right = bottom = 0.0f;
        }

        ~CFloatRect() {}

        CFloatRect(IZ_FLOAT l, IZ_FLOAT t, IZ_FLOAT w, IZ_FLOAT h)
        {
            left = l;
            top = t;
            right = w;
            bottom = h;
        }
        CFloatRect(const SFloatRect& rhs)
        {
            left = rhs.left;
            top = rhs.top;
            right = rhs.right;
            bottom = rhs.bottom;
        }
    };
#endif
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_2D_RECT_H__)
