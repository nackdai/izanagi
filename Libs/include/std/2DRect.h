#if !defined(__URANUS_STD_2D_RECT_H__)
#define __URANUS_STD_2D_RECT_H__

#include "unDefs.h"
#include "2DPoint.h"

namespace uranus {
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
		// ������W��Ԃ�
		S2DPoint<_T>& TopLeft()
		{
			return left_top;
		}
		const S2DPoint<_T>& TopLeft() const
		{
			return left_top;
		}

		// �E�����W��Ԃ�
		S2DPoint<_T>& BottomRight()
		{
			return right_bottom;
		}
		const S2DPoint<_T>& BottomRight() const
		{
			return right_bottom;
		}

		// ���S���W��Ԃ�
		S2DPoint<_T> CenterPoint() const
		{
			S2DPoint<_T> ret;
			ret.x = (left + right) / 2;
			ret.y = (top + bottom) / 2;

			return ret;
		}

		// �R�s�[���̎l�p�`�̐��@���R�s�[
		void CopyRect(const S2DRect<_T>& rc)
		{
			left = rc.left;
			top = rc.top;
			right = rc.right;
			bottom = rc.bottom;
		}

		// ���@��ݒ�
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

		// ��̎l�p�`�ɐݒ�(���ׂĂ̍��W��0�ɂ���)
		void SetRectEmpty()
		{
			left = top = right = bottom = (_T)0;
		}

		// ���ƍ���������
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

		// ���ƍ����𑝉�
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

		// �w�肳�ꂽ x ���W�Ɉړ�
		void MoveToX(_T x)
		{
			_T width = Width();
			left = x;
			right = left + width;
		}

		// �w�肳�ꂽ y ���W
		void MoveToY(_T y);

		// �w�肳�ꂽ x ���W�� y ���W�Ɉړ�
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

		// �肳�ꂽ�I�t�Z�b�g�ňړ�
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

		// �����擾
		_T Width() const
		{
			return (right - left);
		}

		// �������擾
		_T Height() const
		{
			return (bottom - top);
		}

		// �󂩂ǂ����𒲂ׂ�(�����ƕ��̂ǂ��炩�܂��͂��̗����� 0 �̂Ƃ��ɋ�ɂȂ�)
		UN_BOOL IsRectEmpty() const
		{
			return ((left == right) || (top == bottom));
		}

		// �����o�ϐ� top�Abottom�Aleft�Aright �����ׂ� 0 ���ǂ����𒲂ׂ�
		UN_BOOL IsRectNull() const
		{
			return ((left == 0) && (top == 0) && (right == 0) && (bottom == 0));
		}

		// �w�肳�ꂽ�l�p�`�Ɠ��������ǂ���
		UN_BOOL EqualRect(const S2DRect<_T>& rc) const
		{
			return ((left == rc.left)
					&& (top == rc.top)
					&& (right == rc.right)
					&& (bottom == rc.bottom));
		}

		// �w�肳�ꂽ�_����`���ɂ��邩�ǂ����𒲂ׂ�
		UN_BOOL PtInRect(_T x, _T y) const
		{
			return (((left <= x) && (x < right))
					&& ((top <= y) && (y < bottom)));
		}
		UN_BOOL PtInRect(const S2DPoint<_T>& pt) const
		{
			return PtInRect(pt.x, pt.y);
		}

	public:
		UN_BOOL operator!=(const S2DRect<_T>& rhs) const
		{
			return !EqualRect(rhs);
		}

		UN_BOOL operator==(const S2DRect<_T>& rhs) const
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
	typedef S2DRect<UN_INT> SIntRect;
	typedef C2DRect<UN_INT> CIntRect;
	typedef S2DRect<UN_FLOAT> SFloatRect;
	typedef C2DRect<UN_FLOAT> CFloatRect;
}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_2D_RECT_H__)
