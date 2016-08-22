#if !defined(__IZANAGI_COLLISION_AABB_H__)
#define __IZANAGI_COLLISION_AABB_H__

#include "izMath.h"
#include "collision/ColBoundingVolume.h"
#include "collision/ColSphere.h"

namespace izanagi
{
namespace col
{
    /**
     */
    class AABB : public BoundingVolume {
    public:
        AABB();
        AABB(
            const math::SVector4& minPtr,
            const math::SVector4& maxPtr);
        AABB(
            const math::SVector4& center,
            IZ_FLOAT lengthX,
            IZ_FLOAT lengthY,
            IZ_FLOAT lengthZ);

        virtual ~AABB() {}

        AABB(const AABB& rhs);

        const AABB& operator=(const AABB& rhs);

    public:
        void initialize(
            const math::SVector4& minPtr,
            const math::SVector4& maxPtr);

        void initialize(
            const math::SVector4& center,
            IZ_FLOAT lengthX,
            IZ_FLOAT lengthY,
            IZ_FLOAT lengthZ);

        /** �������a���v�Z.
         */
        virtual IZ_FLOAT computeRadius(const math::SVector4& normal) const override;

        /** �������a���v�Z.
         */
        virtual IZ_FLOAT computeRadius(
            const math::SMatrix44& mtxW2V,
            const math::SVector4& normal) const override;

        /** ���S���W���擾.
         */
        virtual const math::SVector4 getCenter() const override;

        /** Get if the ray is hit.
         */
        virtual IZ_BOOL isHit(const Ray& ray, HitResult& res) override;

        /** �ŏ����W���擾.
         */
        const math::SVector4 getMin() const;

        /** �ő���W���擾.
         */
        const math::SVector4 getMax() const;

        /** �T�C�Y���擾.
         */
        const math::SVector3& getSize() const;

        /** �����̂ɂ���.
         */
        void makeCubic();

        /** AABB�ɊO�ڂ���o�E���f�B���O�X�t�B�A���擾.
         */
        col::Sphere getCircumscribedSphere() const;

        /** AABB�ɓ��ڂ���o�E���f�B���O�X�t�B�A���擾.
         */
        col::Sphere getInscribedSphere() const;

        /** Compute if this aabb can be displayed by world to clip transform matrix.
         */
        IZ_BOOL canDisplay(const math::SMatrix44& mtxW2C);

        /** Get the pre-computed result if the aabb can be displayed. 
         */
        IZ_BOOL canDisplay() const;

        /** Check if the point is contain in the aabb.
         */
        IZ_BOOL isContain(const math::SVector3& point);

        /** Check if the aabb is valid.
         */
        IZ_BOOL isValid();

    protected:
        math::CVector4 m_min;

        math::CVector3 m_size;

        enum Clip {
            PositiveX,
            NegativeX,
            PositiveY,
            NegativeY,
            PositiveZ,
            NegativeZ,
        };

        IZ_UINT m_displayFlag{ 0 };
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COLLISION_AABB_H__)
