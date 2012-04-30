#if !defined(__IZANAGI_MATH_TRIANGLE_H__)
#define __IZANAGI_MATH_TRIANGLE_H__

#include "MathVector.h"
#include "MathMatrix.h"

namespace izanagi {
	struct SRay;

	struct STriangle {
		SVector pt[3];
	};

	struct STrianglePlane : public STriangle {
		struct {
			SVector dir;
			IZ_FLOAT length;
		} v[2];
		union {
			SVector nml;
			struct {
				IZ_FLOAT a;
				IZ_FLOAT b;
				IZ_FLOAT c;
				IZ_FLOAT padding;
			};
		};
		IZ_FLOAT d;
	};

	class CTriangle : public STrianglePlane {
	public:
		CTriangle();
		CTriangle(const SVector point[3]);
		CTriangle(const CTriangle& rhs);

		~CTriangle() {}

	public:
		const CTriangle& operator=(const CTriangle& rhs);

		void Set(const SVector point[3]);

		/** 4x4çsóÒÇ…ÇÊÇÈïœä∑.
		 */
		void Transform(const SMatrix& mtx);
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_MATH_TRIANGLE_H__)
