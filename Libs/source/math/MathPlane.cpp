#include "math/MathPlane.h"
#include "math/MathRay.h"
#include "math/MathCVector.h"

namespace izanagi {
	// ���ʏ�Ń��C�ƌ�������_���擾
	IZ_BOOL CPlane::GetCrossPoint(
		CPlane& plane,
		const SRay& ray,
		SVector& refPtr)
	{
		return plane.GetCrossPoint(ray, refPtr);
	}

	CPlane::CPlane()
	{
		a = b = c = d = padding = 0.0f;
	}

	CPlane::CPlane(const SVector& normal, const SVector& ptr)
	{
		Set(normal, ptr);
	}

	CPlane::CPlane(const SPlane& rhs)
	{
		*this = rhs;
	}

	const CPlane& CPlane::operator=(const SPlane& rhs)
	{
		SVector::Copy(nml, rhs.nml);
		d = rhs.d;
		return *this;
	}

	// ���ʂ�ݒ�.
	void CPlane::Set(const SVector& normal, const SVector& ptr)
	{
		SVector::Copy(nml, normal);

		nml.w = 0.0f;
		SVector::Normalize(nml, nml);

		d = -(a * ptr.x + b * ptr.y + c * ptr.z);
	}

	// ���_����̋������擾.
	IZ_FLOAT CPlane::GetDistance() const
	{
		return ::fabs(d);
	}

	// ���ʏ�ɑ��݂���_���ǂ���.
	IZ_BOOL CPlane::IsOnPlane(const SVector& ptr)
	{
		IZ_FLOAT tmp = a * ptr.x + b * ptr.y + c * ptr.z;
		return CMath::IsNearyEqualZero(tmp + d);
	}

	// 4x4�s��ɂ��ϊ�.
	void CPlane::Transform(const SMatrix& mtx)
	{
		// TODO
		// Matrix�ɃX�P�[�����܂܂�Ă����ꍇ
		// ��xNormalize����H

		SMatrix::ApplyXYZ(nml, nml, mtx);
	}

	// ���ʂ�����킷�x�N�g�����擾
	void CPlane::GetPlaneVector(SVector& plane)
	{
		plane.Set(a, b, c, d);
	}

	// ���C�ƌ�������_���擾
	IZ_BOOL CPlane::GetCrossPoint(
		const SRay& ray,
		SVector& refPtr)
	{
		// NOTE
		// Plane : L = <N, D>
		// Ray   : P(t) = Q + Vt
		//
		// N�EP(t) + D = 0
		// <=> N�EQ + (N�EV)t + D = 0
		// <=> t = -(N�EQ + D) / N�EV
		// <=> t = -L�EQ / L�EV
		//
		// Q = (Qx, Qy, Qz, 1) => �ʒu�x�N�g��
		// V = (Vx, Vy, Vz, 0) => �����x�N�g��
		// L = (Nx, Ny, Nz, D)
		// �Ȃ̂ŁA
		// N�EQ + D = Nx * Qx + Ny * Qy + Nz * Qz + D * 1 = L�EQ
		// N�EV + 0 = Nx * Vx + Ny * Vy + Nz * Vz + D * 0 = L�EV

		CVector plane(a, b, c, d);

		// L�EV
		IZ_FLOAT d = SVector::Dot(plane, ray.v);
		if (d >= 0.0f) {

			// NOTE
			//   | /       |
			//   |/        |
			//---+----  ---+---
			//            /
			//           /
			//  NG        OK
			//
			// �ʂ̖@���ƂȂ��p��90�x��葽����Ζʂƌ����
			// => ���ς̒l���}�C�i�X�ł���Ζʂƌ����

			return IZ_FALSE;
		}

		// L�EQ
		IZ_FLOAT t = SVector::Dot(plane, ray.p);

		// t = -L�EQ / L�EV 
		t = -t / d;

		refPtr.x = ray.p.x + t * ray.v.x;
		refPtr.y = ray.p.y + t * ray.v.y;
		refPtr.z = ray.p.z + t * ray.v.z;
		refPtr.w = 1.0f;

		return IZ_TRUE;
	}

	// ���C�ƌ������邩�ǂ���
	IZ_BOOL CPlane::IsCross(const SRay& ray)
	{
		CVector plane(a, b, c, d);

		IZ_FLOAT d = SVector::Dot(plane, ray.v);

		// ���C�ƕ��ʂ̖@���̓��p��90�x�ȏ�
		// => ���C�ƕ��ʂ͌����Ȃ�
		return (d > 0.0f);
	}
}	// namespace izanagi
