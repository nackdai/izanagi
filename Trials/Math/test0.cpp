#include "test0.h"
#include "izMath.h"

void test0()
{
	izanagi::math::SQuat quat;
	izanagi::math::SQuat::SetQuatFromRadAxis(quat, IZ_DEG2RAD(45.0f), 0.0f, 0.0f, 1.0f);

	izanagi::math::SMatrix mtx;
	izanagi::math::SQuat::MatrixFromQuat(mtx, quat);

	izanagi::math::SQuat quat1;
	izanagi::math::SQuat::QuatFromMatrix(quat1, mtx);
}

void test1()
{
	static izanagi::math::SVector tbl[] = {
		{0.0f, 0.0f, 1.0f, -34.3486f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 44.145f},
		{1.0f, 0.0f, 0.0f, -180.0f},
	};

	izanagi::math::SMatrix mtx;
	izanagi::math::SMatrix::SetUnit(mtx);

	for (size_t i = 0; i < COUNTOF(tbl); i++) {
		izanagi::math::SQuat quat;
		izanagi::math::SQuat::SetQuatFromRadAxis(
			quat,
			IZ_DEG2RAD(tbl[i].w), tbl[i].x, tbl[i].y, tbl[i].z);

		izanagi::math::SMatrix tmp;
		izanagi::math::SQuat::MatrixFromQuat(tmp, quat);

		IZ_PRINTF("-----------\n");
		for (IZ_UINT n = 0; n < 16; n++) {
			IZ_PRINTF("%f", tmp.a[n]);
			if ((n % 4) == 3) {
				IZ_PRINTF("\n");
			}
			else {
				IZ_PRINTF(", ");
			}
		}

		izanagi::math::SMatrix::Mul(mtx, mtx, tmp);
	}

	IZ_PRINTF("============\n");
	for (IZ_UINT n = 0; n < 16; n++) {
		IZ_PRINTF("%f", mtx.a[n]);
		if ((n % 4) == 3) {
			IZ_PRINTF("\n");
		}
		else {
			IZ_PRINTF(", ");
		}
	}

	izanagi::math::SQuat quatEnd;
	izanagi::math::SQuat::QuatFromMatrix(quatEnd, mtx);

	IZ_PRINTF("quat: %f, %f, %f, %f\n", quatEnd.x, quatEnd.y, quatEnd.z, quatEnd.w); 
}