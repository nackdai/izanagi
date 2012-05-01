#if !defined(__IZANAGI_MATH_RECTANGLE_H__)
#define __IZANAGI_MATH_RECTANGLE_H__

#include "MathVector.h"
#include "MathMatrix.h"

namespace izanagi {
	struct SRay;
	struct SPlane;
	class CPlane;

	struct SRectangle {
		SVector pt;
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

	class CRectangle : public SRectangle {
	public:
		CRectangle();
		CRectangle(
			const SVector& point,
			const SVector& dir1,
			const SVector& dir2);
		CRectangle(
			const SVector& point,
			const SVector& dir1, IZ_FLOAT length1,
			const SVector& dir2, IZ_FLOAT length2);
		CRectangle(const CRectangle& rhs);

		~CRectangle() {}

	public:
		const CRectangle& operator=(const CRectangle& rhs);

		/** 矩形を設定.
		 */
		void Set(
			const SVector& point,
			const SVector& dir1,
			const SVector& dir2);

		/** 矩形を設定.
		 */
		void Set(
			const SVector& point,
			const SVector& dir1, IZ_FLOAT length1,
			const SVector& dir2, IZ_FLOAT length2);

		/** 原点からの距離を取得.
		 */
		IZ_FLOAT GetDistance() const;

		/** 矩形上に存在する点かどうか.
		 */
		IZ_BOOL IsOnRectangle(const SVector& ptr) const;

		/** 4x4行列による変換.
		 */
		void Transform(const SMatrix& mtx);

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

		/** 平面を取得.
		 */
		void GetPlane(SPlane& plane) const;

	private:
		typedef IZ_BOOL (*GetCrossPointFunc)(const CPlane& plane, const SRay& ray, SVector& refPtr);

		// レイと交差する点を取得
		IZ_BOOL GetCrossPoint(
			GetCrossPointFunc func,
			const SRay& ray,
			SVector& refPtr) const;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_MATH_RECTANGLE_H__)
