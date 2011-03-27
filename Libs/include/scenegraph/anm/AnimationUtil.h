#if !defined(__IZANAGI_SCENEGRAPH_ANIMATION_UTIL_H__)
#define __IZANAGI_SCENEGRAPH_ANIMATION_UTIL_H__

#include "ANMFormat.h"

namespace izanagi {
	class CAnimationUtil {
	private:
		CAnimationUtil();
		~CAnimationUtil();

	public:
		static IZ_BOOL IsScalarInterp(IZ_UINT type);

		static IZ_FLOAT ComputeInterp(
			E_ANM_INTERP_TYPE nInterp,
			IZ_FLOAT fTime,
			IZ_UINT nKeyNum,
			IZ_UINT nPos,
			S_ANM_KEY** const pKeys);

		static void ComputeInterp(
			SVector& vRef,
			E_ANM_INTERP_TYPE nInterp,
			IZ_FLOAT fTime,
			IZ_UINT nKeyNum,
			IZ_UINT nPos,
			S_ANM_KEY** const pKeys);

	private:
		static IZ_FLOAT ComputeLinear(
			IZ_FLOAT fTime,
			IZ_UINT nKeyNum,
			IZ_UINT nPos,
			S_ANM_KEY** const pKeys);

		static IZ_FLOAT ComputeBezier(
			IZ_FLOAT fTime,
			IZ_UINT nKeyNum,
			IZ_UINT nPos,
			S_ANM_KEY** const pKeys);

		static IZ_FLOAT ComputeHermite(
			IZ_FLOAT fTime,
			IZ_UINT nKeyNum,
			IZ_UINT nPos,
			S_ANM_KEY** const pKeys);

		static void ComputeSlerp(
			SVector& vRef,
			IZ_FLOAT fTime,
			IZ_UINT nKeyNum,
			IZ_UINT nPos,
			S_ANM_KEY** const pKeys);

	private:
		static IZ_FLOAT ComputeNomralizedTime(
			IZ_FLOAT fTime,
			IZ_INT& nPrev,
			IZ_INT& nNext,
			IZ_UINT nKeyNum,
			S_ANM_KEY** const pKeys);
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_ANIMATION_UTIL_H__)
