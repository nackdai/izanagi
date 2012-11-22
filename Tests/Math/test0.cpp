#include "test0.h"
#include "izMath.h"

void test0()
{
	izanagi::SQuat quat;
	izanagi::SQuat::SetQuatFromRadAxis(quat, IZ_DEG2RAD(45.0f), 0.0f, 0.0f, 1.0f);

	izanagi::SMatrix mtx;
	izanagi::SQuat::MatrixFromQuat(mtx, quat);

	izanagi::SQuat quat1;
	izanagi::SQuat::QuatFromMatrix(quat1, mtx);
}

void test1()
{
	static izanagi::SVector tbl[] = {
		{0.0f, 0.0f, 1.0f, -34.3486f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 44.145f},
		{1.0f, 0.0f, 0.0f, -180.0f},
	};

	izanagi::SMatrix mtx;
	izanagi::SMatrix::SetUnit(mtx);

	for (size_t i = 0; i < COUNTOF(tbl); i++) {
		izanagi::SQuat quat;
		izanagi::SQuat::SetQuatFromRadAxis(
			quat,
			IZ_DEG2RAD(tbl[i].w), tbl[i].x, tbl[i].y, tbl[i].z);

		izanagi::SMatrix tmp;
		izanagi::SQuat::MatrixFromQuat(tmp, quat);

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

		izanagi::SMatrix::Mul(mtx, mtx, tmp);
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

	izanagi::SQuat quatEnd;
	izanagi::SQuat::QuatFromMatrix(quatEnd, mtx);

	IZ_PRINTF("quat: %f, %f, %f, %f\n", quatEnd.x, quatEnd.y, quatEnd.z, quatEnd.w); 
}