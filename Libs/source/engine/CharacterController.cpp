#include "engine/CharacterController.h"
#include "engine/CharacterMesh.h"

namespace izanagi {
namespace engine {
    CharacterController* CharacterController::create(IMemoryAllocator* allocator)
    {
        auto buf = ALLOC(allocator, sizeof(CharacterController));
        VRETURN_NULL(buf);

        CharacterController* character = new(buf)CharacterController;
        {
            character->m_Allocator = allocator;
            character->AddRef();
        }

        return character;
    }

    CharacterController::CharacterController()
        : m_position(0, 0, 0, 1)
    {
    }

    CharacterController::~CharacterController()
    {
        SAFE_RELEASE(m_mesh);
    }

    void CharacterController::move(const math::SVector3& dir)
    {
        math::CVector4 moveDir(dir.x, dir.y, dir.z, 0.0f);
        m_position += moveDir;
    }

    void CharacterController::move(const math::SVector3& dir, IZ_FLOAT value)
    {
        math::CVector3 moveDir(dir);
        moveDir.Normalize();
        moveDir *= value;

        move(moveDir);
    }

    void CharacterController::setMesh(Mesh* mesh)
    {
        SAFE_REPLACE(m_mesh, mesh);
    }

    Mesh* CharacterController::getMesh()
    {
        return m_mesh;
    }

    math::CMatrix44 CharacterController::getL2W() const
    {
        math::CMatrix44 mtxL2W;

        math::SQuat::MatrixFromQuat(mtxL2W, m_rotate);
        mtxL2W.Trans(m_position.x, m_position.y, m_position.z);

        return std::move(mtxL2W);
    }

    math::CVector4& CharacterController::position()     
    {
        return m_position;
    }

    math::CQuat& CharacterController::rotation()     
    {
        return m_rotate;
    }

    IZ_BOOL CharacterController::registerToRenderGraph(
        const izanagi::CCamera& camera,
        izanagi::CRenderGraph& renderGraph)
    {
        auto mdl = m_mesh->getMdl();

        auto ret = renderGraph.Register(
            camera,
            m_position,
            mdl);
        IZ_ASSERT(ret);

        return ret;
    }
}   // namespace engine
}   // namespace izanagi
