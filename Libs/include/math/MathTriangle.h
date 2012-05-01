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

		/** 4x4�s��ɂ��ϊ�.
		 */
		void Transform(const SMatrix& mtx);

		/** �O�p�`��ɑ��݂���_���ǂ���.
		 */
		IZ_BOOL IsOnTriangle(const SVector& ptr) const;

		/** ���C�ƌ�������_���擾.
		 */
		IZ_BOOL GetCrossPoint(
			const SRay& ray,
			SVector& refPtr) const;

		/** ���C�ƌ������邩�ǂ���.
		 */
		IZ_BOOL IsCross(const SRay& ray);

		/** ���C�ƌ�������_�𗠕\�̗��ʂɂ��Ď擾.
		 */
		IZ_BOOL GetBilateralCrossPoint(
			const SRay& ray,
			SVector& refPtr) const;

		/** ���\�̗��ʂɂ��ă��C�ƌ������邩�ǂ���.
		 */
		IZ_BOOL IsBilateralCross(const SRay& ray) const;

		/** �O�p�`�̊�_����̃x�N�g���ɑ΂��銄�����擾.
		 */
		IZ_BOOL GetFraction(
			const SVector& point,
			IZ_FLOAT f[2]) const;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_MATH_TRIANGLE_H__)
