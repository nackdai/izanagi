#if !defined(__IZANAGI_SCENEGRAPH_ANIMATION_UTIL_H__)
#define __IZANAGI_SCENEGRAPH_ANIMATION_UTIL_H__

#include "ANMFormat.h"

namespace izanagi {
    /**
     */
    class CAnimationUtil {
        friend class CAnimation;

    private:
        CAnimationUtil();
        ~CAnimationUtil();

    public:
        static IZ_BOOL IsScalarInterp(IZ_UINT type);

        static IZ_BOOL IsUserCustomInterp(IZ_UINT type);

        static IZ_FLOAT ComputeInterp(
            E_ANM_INTERP_TYPE nInterp,
            IZ_FLOAT fTime,
            IZ_UINT nKeyNum,
            IZ_UINT nPos,
            const S_ANM_KEY** pKeys);

        static void ComputeInterp(
            math::SVector& vRef,
            E_ANM_INTERP_TYPE nInterp,
            IZ_FLOAT fTime,
            IZ_UINT nKeyNum,
            IZ_UINT nPos,
            const S_ANM_KEY** pKeys);

    private:
        static IZ_FLOAT ComputeLinear(
            IZ_FLOAT fTime,
            IZ_UINT nKeyNum,
            IZ_UINT nPos,
            const S_ANM_KEY** pKeys);

        static IZ_FLOAT ComputeBezier(
            IZ_FLOAT fTime,
            IZ_UINT nKeyNum,
            IZ_UINT nPos,
            const S_ANM_KEY** pKeys);

        static IZ_FLOAT ComputeHermite(
            IZ_FLOAT fTime,
            IZ_UINT nKeyNum,
            IZ_UINT nPos,
            const S_ANM_KEY** pKeys);

        static void ComputeSlerp(
            math::SVector& vRef,
            IZ_FLOAT fTime,
            IZ_UINT nKeyNum,
            IZ_UINT nPos,
            const S_ANM_KEY** pKeys);

        static void ComputeBezierSlerp(
            math::SVector& vRef,
            IZ_FLOAT fTime,
            IZ_UINT nKeyNum,
            IZ_UINT nPos,
            const S_ANM_KEY** pKeys);

    public:
        static IZ_FLOAT ComputeNomralizedTime(
            IZ_FLOAT fTime,
            IZ_INT& nPrev,
            IZ_INT& nNext,
            IZ_UINT nKeyNum,
            const S_ANM_KEY** pKeys);

    private:
        static FuncInterpScalar s_UserFuncInterpScalar;
        static FuncInterpVector s_UserFuncInterpVector;

        // スカラー値の補間処理を設定.
        static void SetUserFuncInterpScalar(FuncInterpScalar func);

        // ベクターの補間処理を設定.
        static void SetUserFuncInterpVector(FuncInterpVector func);

    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_ANIMATION_UTIL_H__)
