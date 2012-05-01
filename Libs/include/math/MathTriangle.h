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

		/** 4x4行列による変換.
		 */
		void Transform(const SMatrix& mtx);

		/** 三角形上に存在する点かどうか.
		 */
		IZ_BOOL IsOnTriangle(const SVector& ptr) const;

		/** レイと交差する点を取得.
		 */
		IZ_BOOL GetCrossPoint(
			const SRay& ray,
			SVector& refPtr) const;

		/** レイと交差するかどうか.
		 */
		IZ_BOOL IsCross(const SRay& ray);

		/** レイと交差する点を裏表の両面について取得.
		 */
		IZ_BOOL GetBilateralCrossPoint(
			const SRay& ray,
			SVector& refPtr) const;

		/** 裏表の両面についてレイと交差するかどうか.
		 */
		IZ_BOOL IsBilateralCross(const SRay& ray) const;

		/** 三角形の基準点からのベクトルに対する割合を取得.
		 */
		IZ_BOOL GetFraction(
			const SVector& point,
			IZ_FLOAT f[2]) const;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_MATH_TRIANGLE_H__)
