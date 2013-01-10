#if !defined(__IZANAGI_SCENEGRAPH_ANIMATION_BLENDER_H__)
#define __IZANAGI_SCENEGRAPH_ANIMATION_BLENDER_H__

#include "AnimationInterface.h"

namespace izanagi {
    /** モーションブレンディングのベースとなるインターフェースクラス
     */
    class IAnimationBlender : public CAnimationCachable {
    public:
        template <class _T>
        static _T* CreateAnmBlender(IMemoryAllocator* allocator)
        {
            void* buf = ALLOC_ZERO(allocator, sizeof(_T));
            VRETURN_NULL(buf != IZ_NULL);

            IZ_BOOL result = IZ_TRUE;

            _T* instance = new(buf) _T;
            {
                instance->AddRef();
                instance->m_Allocator = allocator;

                result = instance->Init();
            }

            if (!result) {
                SAFE_RELEASE(instance);
            }

            return instance;
        }

    protected:
        IAnimationBlender() {}
        virtual ~IAnimationBlender() {}

        NO_COPIABLE(IAnimationBlender);

        IZ_DEFINE_INTERNAL_RELEASE();

    protected:
        PURE_VIRTUAL(IZ_BOOL Init());

    public:
        PURE_VIRTUAL(IZ_FLOAT GetMinAnimationTime() const);
        PURE_VIRTUAL(IZ_FLOAT GetMaxAnimationTime() const);

    protected:
        IMemoryAllocator* m_Allocator;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_ANIMATION_BLENDER_H__)
