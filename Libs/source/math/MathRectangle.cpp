#include "math/MathRectangle.h"
#include "math/MathRay.h"
#include "math/MathPlane.h"
#include "math/MathCVector.h"

namespace izanagi {
	CRectangle::CRectangle()
	{
		a = b = c = d = padding = 0.0f;

		pt.Set(0.0f, 0.0f, 0.0f);
		v[0].dir.Set(0.0f, 0.0f, 0.0f, 0.0f);
		v[1].dir.Set(0.0f, 0.0f, 0.0f, 0.0f);
	}

	CRectangle::CRectangle(
		const SVector& point,
		IZ_FLOAT lengthX,
		IZ_FLOAT lengthZ)
	{
		Set(
			point,
			lengthX, lengthZ);
	}

	CRectangle::CRectangle(const CRectangle& rhs)
	{
		*this = rhs;
	}

	const CRectangle& CRectangle::operator=(const CRectangle& rhs)
	{
		SVector::Copy(pt, rhs.pt);

		v[0].length = rhs.v[0].length;
		SVector::Copy(v[0].dir, rhs.v[0].dir);

		v[1].length = rhs.v[1].length;
		SVector::Copy(v[1].dir, rhs.v[1].dir);

		SVector::Copy(nml, rhs.nml);

		d = rhs.d;

		return *this;
	}

	static const izanagi::CVector DirX(1.0f, 0.0f,  0.0f, 0.0f);
	static const izanagi::CVector DirZ(0.0f, 0.0f, -1.0f, 0.0f);

	// ��`��ݒ�.
	void CRectangle::Set(
		const SVector& point,
		IZ_FLOAT lengthX,
		IZ_FLOAT lengthZ)
	{
		SVector::Copy(pt, point);

		v[0].length = lengthX;
		v[0].dir.Set(DirX);

		v[1].length = lengthZ;
		v[1].dir.Set(DirZ);

		SVector::Cross(nml, v[0].dir, v[1].dir);
		SVector::Normalize(nml, nml);

		d = -(a * pt.x + b * pt.y + c * pt.z);
	}

	// ���_����̋������擾.
	IZ_FLOAT CRectangle::GetDistance() const
	{
		return ::fabs(d);
	}

	// ��`��ɑ��݂���_���ǂ���.
	IZ_BOOL CRectangle::IsOnRectangle(const SVector& ptr) const
	{
		SVector p;
		SVector::Sub(p, ptr, pt);

		IZ_FLOAT dot1 = SVector::Dot(p, v[0].dir);
		IZ_FLOAT dot2 = SVector::Dot(p, v[1].dir);

		IZ_FLOAT length1 = v[0].length;
		IZ_FLOAT length2 = v[1].length;

#if 0
		return IS_IN_BOUND(dot1 / length1, 0.0f, 1.0f) && IS_IN_BOUND(dot2 / length2, 0.0f, 1.0f);
#else
		// pt�͋�`�̒��S�_�Ȃ̂�
		// ��`���\������_�͓�̃x�N�g���� -0.5 - 0.5 �͈̔͂ɑ��݂���
		return IS_IN_BOUND(dot1 / length1, -0.5f, 0.5f)
			&& IS_IN_BOUND(dot2 / length2, -0.5f, 0.5f);
#endif
	}

	// 4x4�s��ɂ��ϊ�.
	void CRectangle::Transform(const SMatrix& mtx)
	{
		CVector p(0.0f, 0.0f, 0.0f);
		SMatrix::Apply(p, p, mtx);

		SVector::AddXYZ(pt, pt, p);

		for (IZ_UINT i = 0; i < COUNTOF(v); i++)
		{
			SVector tmp;
			SVector::Scale(tmp, v[i].dir, v[i].length);

			SMatrix::ApplyXYZ(v[i].dir, tmp, mtx);

			v[i].length = SVector::Length(v[i].dir);
			SVector::Normalize(v[i].dir, v[i].dir);
		}

		SVector::Cross(nml, v[0].dir, v[1].dir);
		SVector::Normalize(nml, nml);

		// �v�Z��Z���ۂ߂�
		nml.x = izanagi::CMath::IsNearyEqualZero(nml.x) ? 0.0f : nml.x;
		nml.y = izanagi::CMath::IsNearyEqualZero(nml.y) ? 0.0f : nml.y;
		nml.z = izanagi::CMath::IsNearyEqualZero(nml.z) ? 0.0f : nml.z;
		SVector::Normalize(nml, nml);

		d = -(a * pt.x + b * pt.y + c * pt.z);
	}

	// ���C�ƌ�������_���擾
	IZ_BOOL CRectangle::GetCrossPoint(
		const SRay& ray,
		SVector& refPtr) const
	{
		IZ_BOOL isCross = GetCrossPoint(
			CPlane::GetCrossPoint,
			ray,
			refPtr);

		return isCross;
	}

	// ���C�ƌ������邩�ǂ���
	IZ_BOOL CRectangle::IsCross(const SRay& ray)
	{
		CVector ptr;

		IZ_BOOL isCross = GetCrossPoint(ray, ptr);

		return isCross;
	}

	// ���ʂ��擾.
	void CRectangle::GetPlane(SPlane& plane) const
	{
		plane.pt.Set(
			pt.x,
			pt.y,
			pt.z);

		plane.nml.Set(a, b, c, 0.0f);
		plane.d = d;
	}

	// ���C�ƌ�������_���擾
	IZ_BOOL CRectangle::GetCrossPoint(
		CRectangle::GetCrossPointFunc func,
		const SRay& ray,
		SVector& refPtr) const
	{
		CVector ptr;

		IZ_BOOL isCross = (*func)(
			CPlane(nml, pt),
			ray, ptr);

		if (isCross) {
			isCross = IsOnRectangle(ptr);
			if (isCross) {
				SVector::Copy(refPtr, ptr);
			}
		}

		return isCross;
	}

	// X�����̃x�N�g�����擾.
	const SVector& CRectangle::GetX() const
	{
		return v[0].dir;
	}

	// Y�����̃x�N�g�����擾.
	const SVector& CRectangle::GetY() const
	{
		return v[1].dir;
	}

	// �����擾.
	IZ_FLOAT CRectangle::GetWidth() const
	{
		return v[0].length;
	}

	// �������擾.
	IZ_FLOAT CRectangle::GetHeight() const
	{
		return v[1].length;
	}
}	// namespace izanagi
