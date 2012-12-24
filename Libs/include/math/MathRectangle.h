#if !defined(__IZANAGI_MATH_RECTANGLE_H__)
#define __IZANAGI_MATH_RECTANGLE_H__

#include "MathVector.h"
#include "MathMatrix.h"

namespace izanagi
{
namespace math
{
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
            IZ_FLOAT lengthX,
            IZ_FLOAT lengthZ);
        CRectangle(const CRectangle& rhs);

        ~CRectangle() {}

    public:
        const CRectangle& operator=(const CRectangle& rhs);

        /** 矩形を設定.
         */
        void Set(
            const SVector& point,
            IZ_FLOAT lengthX,
            IZ_FLOAT lengthZ);

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

        /** 平面を取得.
         */
        void GetPlane(SPlane& plane) const;

        /** X方向のベクトルを取得.
         */
        const SVector& GetX() const;

        /** Y方向のベクトルを取得.
         */
        const SVector& GetY() const;

        /** 幅を取得.
         */
        IZ_FLOAT GetWidth() const;

        /** 高さを取得.
         */
        IZ_FLOAT GetHeight() const;

    private:
        typedef IZ_BOOL (*GetCrossPointFunc)(const CPlane& plane, const SRay& ray, SVector& refPtr);

        // レイと交差する点を取得
        IZ_BOOL GetCrossPoint(
            GetCrossPointFunc func,
            const SRay& ray,
            SVector& refPtr) const;
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_MATH_RECTANGLE_H__)
