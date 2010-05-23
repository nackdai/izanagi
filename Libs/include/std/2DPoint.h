#if !defined(__URANUS_STD_2D_POINT_H__)
#define __URANUS_STD_2D_POINT_H__

#include "unDefs.h"

namespace uranus {
	/**
	*/
	template <typename _T>
	struct S2DPoint {
		_T x, y;
	};

	/**
	*/
	template <typename _T>
	class C2DPoint : public S2DPoint<_T> {
	public:
		C2DPoint()
		{
			x = y = (_T)0;
		}

		~C2DPoint() {}

		C2DPoint(_T _x, _T _y)
		{
			x = _x;
			y = _y;
		}
		C2DPoint(const S2DPoint<_T>& rhs)
		{
			x = rhs.x;
			y = rhs.y;
		}

	public:
		void Offset(const S2DPoint<_T>& pt)
		{
			x += pt.x;
			y += pt.y;
		}

		void Offset(_T x, _T y)
		{
			x += _x;
			y += _y;
		}

	public:
		UN_BOOL operator!=(const S2DPoint<_T>& pt)
		{
			UN_BOOL ret = ((x != pt.x) && (y != pt.y));
			return ret;
		}

		UN_BOOL operator==(const S2DPoint<_T>& pt)
		{
			UN_BOOL ret = !(*this != pt);
			return ret;
		}

		const C2DPoint& operator=(const S2DPoint<_T>& pt)
		{
			x = pt.x;
			y = pt.y;
			return *this;
		}

		C2DPoint operator+(const S2DPoint<_T>& pt)
		{
			C2DPoint tmp(x + pt.x, y + pt.x);
			return tmp;
		}

		C2DPoint operator-(const S2DPoint<_T>& pt)
		{
			C2DPoint tmp(x - pt.x, y - pt.x);
			return tmp;
		}

		void operator+=(const S2DPoint<_T>& pt)
		{
			x += pt.x;
			y += pt.y;
		}

		void operator-=(const S2DPoint<_T>& pt)
		{
			x -= pt.x;
			y -= pt.y;
		}
	};

	//////////////////////////////////////////////////////////
	typedef S2DPoint<UN_INT> SIntPoint;
	typedef C2DPoint<UN_INT> CIntPoint;
	typedef S2DPoint<UN_FLOAT> SFloatPoint;
	typedef C2DPoint<UN_FLOAT> CFloatPoint;
}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_2D_POINT_H__)
