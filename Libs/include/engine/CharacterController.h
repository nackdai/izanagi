#if !defined(__IZANAGI_ENGINE_CHARACTER_CONTROLLER_H__)
#define __IZANAGI_ENGINE_CHARACTER_CONTROLLER_H__

#include "izStd.h"
#include "izMath.h"
#include "izSceneGraph.h"

namespace izanagi {
namespace engine {
    class Mesh;

    class CharacterController : public CObject {
    public:
        static CharacterController* create(IMemoryAllocator* allocator);

    private:
        CharacterController();
        virtual ~CharacterController();

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        void move(const math::SVector4& dir);
        void move(const math::SVector4& dir, IZ_FLOAT value);

        void setMesh(Mesh* mesh);

        Mesh* getMesh();

        math::CMatrix44 getL2W() const;

        math::CVector4& getPosition();

        math::CQuat& getRotate();

        IZ_BOOL registerToRenderGraph(
            const izanagi::CCamera& camera,
            izanagi::CRenderGraph& renderGraph);

    private:
        izanagi::IMemoryAllocator* m_Allocator{ nullptr };

        Mesh* m_mesh{ nullptr };

        math::CVector4 m_position;
        math::CQuat m_rotate;
    };
}   // namespace engine
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ENGINE_CHARACTER_CONTROLLER_H__)

