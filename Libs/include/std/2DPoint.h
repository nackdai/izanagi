#if !defined(__IZANAGI_STD_2D_POINT_H__)
#define __IZANAGI_STD_2D_POINT_H__

#include "izDefs.h"

namespace izanagi {
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
		IZ_BOOL operator!=(const S2DPoint<_T>& pt)
		{
			IZ_BOOL ret = ((x != pt.x) && (y != pt.y));
			return ret;
		}

		IZ_BOOL operator==(const S2DPoint<_T>& pt)
		{
			IZ_BOOL ret = !(*this != pt);
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
	typedef S2DPoint<IZ_INT> SIntPoint;
	typedef C2DPoint<IZ_INT> CIntPoint;
	typedef S2DPoint<IZ_FLOAT> SFloatPoint;
	typedef C2DPoint<IZ_FLOAT> CFloatPoint;
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_2D_POINT_H__)
