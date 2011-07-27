#if !defined(__IZANAGI__MATH_VECTOR_H__)
#define __IZANAGI__MATH_VECTOR_H__

#include <math.h>
#include <memory.h>
#include "izDefs.h"
#include "MathUtil.h"

namespace izanagi {

	struct SVector {
		union {
			IZ_FLOAT v[4];
			struct {
				IZ_FLOAT x, y, z, w;
			};
			struct {
				IZ_FLOAT r, g, b, a;
			};
		};

		void Set(IZ_FLOAT _x, IZ_FLOAT _y, IZ_FLOAT _z, IZ_FLOAT _w = 1.0f)
		{
			x = _x; y = _y; z = _z; w = _w;
		}

		void Set(const SVector& rhs)
		{
			x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
		}

		// ベクトルを設定する
		static void Set(SVector& vec, IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z, IZ_FLOAT w)
		{
			vec.x = x;
			vec.y = y;
			vec.z = z;
			vec.w = w;
		}

		static void SetXYZ(SVector& vec, IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
		{
			vec.x = x;
			vec.y = y;
			vec.z = z;
		}

		// 全成分０のベクトルを設定する
		static void SetZero(SVector& vec)
		{
			Set(vec, 0.0f, 0.0f, 0.0f, 0.0f);
		}

		static void SetDefaultZero(SVector& vec)
		{
			Set(vec, 0.0f, 0.0f, 0.0f, 1.0f);
		}

		// ベクトルをコピー
		static void Copy(SVector& dst, const SVector& src)
		{
			memcpy(&dst, &src, sizeof(SVector));
		}

		// ベクトルをコピー(XYZ成分のみ)
		static void CopyXYZ(SVector& dst, const SVector& src)
		{
			dst.x = src.x;
			dst.y = src.y;
			dst.z = src.z;
		}

		// ２つのベクトルをスワップする。
		static void Swap(SVector& v1, SVector& v2)
		{
			SVector v;
			Copy(v, v1);
			Copy(v1, v2);
			Copy(v2, v);
		}

		// ベクトルを加算する
		// dst = src1 + src2
		static void Add(SVector& dst, const SVector& src1, const SVector& src2)
		{
#if defined(__USE_D3D_MATH__)
			D3DXVec4Add(
				reinterpret_cast<D3DXVECTOR4*>(&dst),
				reinterpret_cast<const D3DXVECTOR4*>(&src1),
				reinterpret_cast<const D3DXVECTOR4*>(&src2));
#else	// #if defined(__USE_D3D_MATH__)
			dst.x = src1.x + src2.x;
			dst.y = src1.y + src2.y;
			dst.z = src1.z + src2.z;
			dst.w = src1.w + src2.w;
#endif	// #if defined(__USE_D3D_MATH__)
		}

		// ベクトルを加算する(XYZ成分のみ)
		static void AddXYZ(SVector& dst, const SVector& src1, const SVector& src2)
		{
#if defined(__USE_D3D_MATH__)
			D3DXVec3Add(
				reinterpret_cast<D3DXVECTOR3*>(&dst),
				reinterpret_cast<const D3DXVECTOR3*>(&src1),
				reinterpret_cast<const D3DXVECTOR3*>(&src2));
			dst.w = src1.w;
#else	// #if defined(__USE_D3D_MATH__)
			dst.x = src1.x + src2.x;
			dst.y = src1.y + src2.y;
			dst.z = src1.z + src2.z;
			dst.w = src1.w;
#endif	// #if defined(__USE_D3D_MATH__)
		}

		// ベクトルを減算する
		// dst = src1 - src2
		static void Sub(SVector& dst, const SVector& src1, const SVector& src2)
		{
#if defined(__USE_D3D_MATH__)
			D3DXVec4Subtract(
				reinterpret_cast<D3DXVECTOR4*>(&dst),
				reinterpret_cast<const D3DXVECTOR4*>(&src1),
				reinterpret_cast<const D3DXVECTOR4*>(&src2));
#else	// #if defined(__USE_D3D_MATH__)
			dst.x = src1.x - src2.x;
			dst.y = src1.y - src2.y;
			dst.z = src1.z - src2.z;
			dst.w = src1.w - src2.w;
#endif	// #if defined(__USE_D3D_MATH__)
		}

		// ベクトルを減算する(XYZ成分のみ)
		static void SubXYZ(SVector& dst, const SVector& src1, const SVector& src2)
		{
#if defined(__USE_D3D_MATH__)
			D3DXVec3Subtract(
				reinterpret_cast<D3DXVECTOR3*>(&dst),
				reinterpret_cast<const D3DXVECTOR3*>(&src1),
				reinterpret_cast<const D3DXVECTOR3*>(&src2));
			dst.w = src1.w;
#else	// #if defined(__USE_D3D_MATH__)
			dst.x = src1.x - src2.x;
			dst.y = src1.y - src2.y;
			dst.z = src1.z - src2.z;
			dst.w = src1.w;
#endif	// #if defined(__USE_D3D_MATH__)
		}

		// ベクトルを乗算する
		// dst = src1 * src2
		static void Mul(SVector& dst, const SVector& src1, const SVector& src2)
		{
			dst.x = src1.x * src2.x;
			dst.y = src1.y * src2.y;
			dst.z = src1.z * src2.z;
			dst.w = src1.w * src2.w;
		}

		// ベクトルをスケーリングする
		// dst = fQ * src
		static void Scale(SVector& dst, const SVector& src, IZ_FLOAT fQ)
		{
#if defined(__USE_D3D_MATH__)
			D3DXVec4Scale(
				reinterpret_cast<D3DXVECTOR4*>(&dst),
				reinterpret_cast<const D3DXVECTOR4*>(&src),
				fQ);
#else	// #if defined(__USE_D3D_MATH__)
			dst.x = fQ * src.x;
			dst.y = fQ * src.y;
			dst.z = fQ * src.z;
			dst.w = fQ * src.w;
#endif	// #if defined(__USE_D3D_MATH__)
		}

		// ベクトルのＸＹＺ成分のみをスケーリングする
		// Ｗ成分はそのまま
		// dst = (src.x x fQ, src.y x fQ, src.z x fQ, src.w)
		static void ScaleXYZ(SVector& dst, const SVector& src, IZ_FLOAT fQ)
		{
#if defined(__USE_D3D_MATH__)
			D3DXVec3Scale(
				reinterpret_cast<D3DXVECTOR3*>(&dst),
				reinterpret_cast<const D3DXVECTOR3*>(&src),
				fQ);
			dst.w = src.w;
#else	// #if defined(__USE_D3D_MATH__)
			dst.x = fQ * src.x;
			dst.y = fQ * src.y;
			dst.z = fQ * src.z;
			dst.w = src.w;
#endif	// #if defined(__USE_D3D_MATH__)
		}

		// ベクトルをスケーリング（除算）する
		// dst = (src.x / fQ, src.y / fQ, src.z / fQ, src.w / fQ)
		static void Div(SVector& dst, const SVector& src, IZ_FLOAT fQ)
		{
#if defined(__USE_D3D_MATH__)
			D3DXVec4Scale(
				reinterpret_cast<D3DXVECTOR4*>(&dst),
				reinterpret_cast<const D3DXVECTOR4*>(&src),
				1.0f / fQ);
#else	// #if defined(__USE_D3D_MATH__)
			dst.x = src.x / fQ;
			dst.y = src.y / fQ;
			dst.z = src.z / fQ;
			dst.w = src.w / fQ;
#endif	// #if defined(__USE_D3D_MATH__)
		}

		// ベクトルのＸＹＺ成分のみをスケーリング(除算)する
		// Ｗ成分はそのまま
		// dst = (src.x / fQ, src.y / fQ, src.z / fQ, src.w)
		static void DivXYZ(SVector& dst, const SVector& src, IZ_FLOAT fQ)
		{
#if defined(__USE_D3D_MATH__)
			D3DXVec3Scale(
				reinterpret_cast<D3DXVECTOR3*>(&dst),
				reinterpret_cast<const D3DXVECTOR3*>(&src),
				1.0f / fQ);
			dst.w = src.w;
#else	// #if defined(__USE_D3D_MATH__)
			dst.x = src.x / fQ;
			dst.y = src.y / fQ;
			dst.z = src.z / fQ;
			dst.w = src.w;
#endif	// #if defined(__USE_D3D_MATH__)
		}

		// ベクトルの内積を計算する
		static IZ_FLOAT Dot(const SVector& vec1, const SVector& vec2)
		{
#if defined(__USE_D3D_MATH__)
			IZ_FLOAT ret = D3DXVec3Dot(
							reinterpret_cast<const D3DXVECTOR3*>(&vec1),
							reinterpret_cast<const D3DXVECTOR3*>(&vec2));
			return ret;
#else	// #if defined(__USE_D3D_MATH__)
			IZ_FLOAT ret = vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
			return ret;
#endif	// #if defined(__USE_D3D_MATH__)
		}

		// ベクトルの外積を計算する
		static void Cross(SVector& dst, const SVector& src1, const SVector& src2)
		{
#if defined(__USE_D3D_MATH__)
			D3DXVec3Cross(
				reinterpret_cast<D3DXVECTOR3*>(&dst),
				reinterpret_cast<const D3DXVECTOR3*>(&src1),
				reinterpret_cast<const D3DXVECTOR3*>(&src2));
#else	// #if defined(__USE_D3D_MATH__)
			dst.x = src1.y * src2.z - src1.z * src2.y;
			dst.y = src1.z * src2.x - src1.x * src2.z;
			dst.z = src1.x * src2.y - src1.y * src2.x;
			dst.w = src1.w;
#endif	// #if defined(__USE_D3D_MATH__)
		}

		// ベクトルの大きさ(絶対値)を計算する
		static IZ_FLOAT Length(const SVector& vec)
		{
#if defined(__USE_D3D_MATH__)
			IZ_FLOAT ret = D3DXVec3Length(reinterpret_cast<const D3DXVECTOR3*>(&vec));
			return ret;
#else	// #if defined(__USE_D3D_MATH__)
			IZ_FLOAT ret = sqrtf(Dot(vec, vec));
			return ret;
#endif	// #if defined(__USE_D3D_MATH__)
		}

		// 位置ベクトル間の距離を計算する
		static IZ_FLOAT Length2(const SVector& vec1, const SVector& vec2)
		{
			SVector v;
			Sub(v, vec1, vec2);
			return Length(v);
		}

		// ベクトルを正規化する
		static void Normalize(SVector& dst, const SVector& src)
		{
#if defined(__USE_D3D_MATH__)
			D3DXVec3Normalize(
				reinterpret_cast<D3DXVECTOR3*>(&dst),
				reinterpret_cast<const D3DXVECTOR3*>(&src));
			dst.w = 0.0f;
#else	// #if defined(__USE_D3D_MATH__)
			SVector::DivXYZ(dst, src, Length(src));
			dst.w = 0.0f;
#endif	// #if defined(__USE_D3D_MATH__)
		}

		// Ｘ軸に対してベクトルを回転する
		static void RotX(SVector& dst, const SVector& src, IZ_FLOAT fTheta)
		{
			IZ_FLOAT c, s;
			CMath::GetSinCosF(fTheta, &s, &c);

			Set(
				dst,
				src.v[0],
				src.v[1] * c - src.v[2] * s,
				src.v[1] * s + src.v[2] * c,
				src.v[3]);
		}

		// Ｙ軸に対してベクトルを回転する
		static void RotY(SVector& dst, const SVector& src, IZ_FLOAT fTheta)
		{
			IZ_FLOAT c, s;
			CMath::GetSinCosF(fTheta, &s, &c);

			Set(
				dst,
				src.v[0] * c + src.v[2] * s,
				src.v[1],
				src.v[2] * c - src.v[0] * s,
				src.v[3]);
		}

		// Ｚ軸に対してベクトルを回転する
		static void RotZ(SVector& dst, const SVector& src, IZ_FLOAT fTheta)
		{
			IZ_FLOAT c, s;
			CMath::GetSinCosF(fTheta, &s, &c);

			Set(
				dst,
				src.v[0] * c - src.v[1] * s,
				src.v[0] * s + src.v[1] * c,
				src.v[2],
				src.v[3]);	
		}

		// ベクトルの線形補間
		static void Lerp(SVector& dst, const SVector& src0, const SVector& src1, IZ_FLOAT f)
		{
#if defined(__USE_D3D_MATH__)
			D3DXVec4Lerp(
				reinterpret_cast<D3DXVECTOR4*>(&dst),
				reinterpret_cast<const D3DXVECTOR4*>(&src0),
				reinterpret_cast<const D3DXVECTOR4*>(&src1),
				f);
#else	// #if defined(__USE_D3D_MATH__)
			IZ_FLOAT ff = 1.0f - f;
			dst.x = ff * src0.x + f * src1.x;
			dst.y = ff * src0.y + f * src1.y;
			dst.z = ff * src0.z + f * src1.z;
			dst.w = ff * src0.w + f * src1.w;
#endif	// #if defined(__USE_D3D_MATH__)
		}
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI__MATH_VECTOR_H__)
