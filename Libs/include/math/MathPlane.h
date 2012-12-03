#if !defined(__IZANAGI_MATH_PLANE_H__)
#define __IZANAGI_MATH_PLANE_H__

#include "MathVector.h"
#include "MathMatrix.h"

namespace izanagi
{
namespace math
{
	struct SRay;

	/**
	 */
	struct SPlane {
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

		SVector pt;
	};

	/**
	 */
	class CPlane : public SPlane {
		friend class CRectangle;

	private:
		/** 平面上でレイと交差する点を取得
		 */
		static IZ_BOOL GetCrossPoint(
			const CPlane& plane,
			const SRay& ray,
			SVector& refPtr);

	public:
		CPlane();
		CPlane(const SVector& normal, const SVector& ptr);
		CPlane(const SPlane& rhs);

		~CPlane() {}

	private:
		CPlane(IZ_FLOAT _a, IZ_FLOAT _b, IZ_FLOAT _c, IZ_FLOAT _d);

	public:
		/** 平面をコピー.
		 */
		const CPlane& operator=(const SPlane& rhs);

		/** 平面を設定.
		 */
		void Set(const SVector& normal, const SVector& ptr);

		/** 平面を設定.
		 */
		void Set(const SVector& normal, IZ_FLOAT _d);

		/** 原点からの距離を取得.
		 */
		IZ_FLOAT GetDistance() const;

		/** 平面上に存在する点かどうか.
		 */
		IZ_BOOL IsOnPlane(const SVector& ptr);

		/** 4x4行列による変換.
		 */
		void Transform(const SMatrix& mtx);

		/** 平面をあらわすベクトルを取得.
		 */
		void GetPlaneVector(SVector& plane);

		/** レイと交差する点を取得.
		 */
		IZ_BOOL GetCrossPoint(
			const SRay& ray,
			SVector& refPtr) const;

		/** 線分と交差する点を取得.
		 */
		IZ_BOOL GetCrossPoint(
			const SVector& from,
			const SVector& to,
			SVector& refPtr) const;

		/** レイと交差するかどうか.
		 */
		IZ_BOOL IsCross(const SRay& ray) const;

		/** 面の正側（法線の向き側）に点があるかどうか.
		 */
		IZ_BOOL IsPositive(const SVector& ptr) const;
	};
}   // namespace math
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_MATH_PLANE_H__)
