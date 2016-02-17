#if !defined(__IZANAGI_COLLISION_RECTANGLE_H__)
#define __IZANAGI_COLLISION_RECTANGLE_H__

#include "izMath.h"

namespace izanagi
{
namespace col
{
    struct SRay;
    struct SPlane;
    class CPlane;

    /** 矩形パラメータ構造体
     */
    struct SRectangle {
        math::SVector4 pt;     ///< 左上原点
        math::SVector4 v[2];   ///< 原点から伸びる矩形を形成する２ベクトル

        union {
            math::SVector4 nml;    ///< 法線
            struct {
                IZ_FLOAT a;
                IZ_FLOAT b;
                IZ_FLOAT c;
                IZ_FLOAT padding;
            };
        };
        IZ_FLOAT d;     ///< 原点からの距離
    };

    /** 矩形クラス
     */
    class CRectangle : public SRectangle {
    public:
        CRectangle();
        CRectangle(
            const math::SVector4& point,
            const math::SVector4& v0,
            const math::SVector4& v1);
        CRectangle(const CRectangle& rhs);

        ~CRectangle() {}

    public:
        const CRectangle& operator=(const CRectangle& rhs);

        /** 矩形を設定.
         */
        void Set(
            const math::SVector4& point,
            const math::SVector4& v0,
            const math::SVector4& v1);

        /** 原点からの距離を取得.
         */
        IZ_FLOAT GetDistance() const;

        /** 矩形上に存在する点かどうか.
         */
        IZ_BOOL IsOnRectangle(const math::SVector4& ptr) const;

        /** 4x4行列による変換.
         */
        void Transform(const math::SMatrix44& mtx);

        void Transform(
            CRectangle& dst,
            const math::SMatrix44& mtx);

        /** レイと交差する点を取得.
         */
        IZ_BOOL GetIntersectPoint(
            const SRay& ray,
            math::SVector4& refPtr,
            IZ_FLOAT* retRayCoefficient = IZ_NULL) const;

        /** レイと交差するかどうか.
         */
        IZ_BOOL IsIntersect(
            const SRay& ray,
            IZ_FLOAT* retRayCoefficient = IZ_NULL);

        /** 平面を取得.
         */
        void GetPlane(SPlane& plane) const;

    private:
        typedef IZ_BOOL (*GetCrossPointFunc)(const CPlane& plane, const SRay& ray, math::SVector4& refPtr, IZ_FLOAT* retRayCoefficient);

        // レイと交差する点を取得
        IZ_BOOL GetIntersectPoint(
            GetCrossPointFunc func,
            const SRay& ray,
            math::SVector4& refPtr,
            IZ_FLOAT* retRayCoefficient) const;
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COLLISION_RECTANGLE_H__)
