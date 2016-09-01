#include "PlayerCharacter.h"

PlayerCharacter::PlayerCharacter()
{
    QuatToRotateRight = izanagi::math::CQuat::AngleAxis(
        IZ_DEG2RAD(90.0f),
        izanagi::math::CVector3::yup());
}

PlayerCharacter::~PlayerCharacter()
{
}

// 初期化.
IZ_BOOL PlayerCharacter::init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    initModel(allocator, device);

    initAnimation(allocator);

    m_ctrl->rotation().InitAngleAxis(0.0f, izanagi::math::CVector3::yup());

    m_camera = &camera;

    updateDirection(1.0f);

__EXIT__:
    if (!result) {
        release();
    }

    return IZ_TRUE;
}

IZ_BOOL PlayerCharacter::initModel(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device)
{
    IZ_BOOL result = IZ_FALSE;

    static const char* imgPath = "data/UnityChanImg.img";
    static const char* mshPath = "data/unitychan.msh";
    static const char* sklPath = "data/unitychan.skl";
    static const char* shdPath = "data/SkinShader.shd";

    izanagi::CImage* img = nullptr;
    izanagi::shader::CShaderBasic* shd = nullptr;
    izanagi::sample::CSampleMdlRenderHandler* renderHandler = nullptr;

    // Texture
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open(imgPath));

        img = izanagi::CImage::CreateImage(
            allocator,
            device,
            &in);

        VGOTO(result = (img != IZ_NULL), __EXIT__);
    }

    // Shader
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open(shdPath));

        shd = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
            allocator,
            device,
            &in);

        VGOTO(result = (shd != IZ_NULL), __EXIT__);

        // ジョイントマトリクスを描画時に設定するためのハンドラ
        renderHandler = izanagi::IMshRenderHandler::CreateMshRenderHandler<izanagi::sample::CSampleMdlRenderHandler>(allocator);
        renderHandler->SetShader(shd);
        VGOTO(result = (renderHandler != IZ_NULL), __EXIT__);
    }

    m_mesh = izanagi::engine::Mesh::create(allocator);
    VGOTO(result = (m_mesh != IZ_NULL), __EXIT__);

    result = m_mesh->loadModel(
        device,
        mshPath,
        sklPath,
        renderHandler);
    VGOTO(result = (m_mesh != IZ_NULL), __EXIT__);

    m_ctrl = izanagi::engine::CharacterController::create(allocator);
    VGOTO(result = (m_ctrl != IZ_NULL), __EXIT__);

    m_ctrl->setMesh(m_mesh);

    result = createMaterial(
        allocator,
        device,
        img,
        shd);

__EXIT__:
    SAFE_RELEASE(img);
    SAFE_RELEASE(shd);
    SAFE_RELEASE(renderHandler);

    if (!result) {
        release();
    }

    return result;
}

IZ_BOOL PlayerCharacter::createMaterial(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::CImage* img,
    izanagi::shader::CShaderBasic* shd)
{
    static const char* mtrls[] = {
        "data/unitychan_0.mtrl",
        "data/unitychan_1.mtrl",
        "data/unitychan_2.mtrl",
        "data/unitychan_3.mtrl",
        "data/unitychan_4.mtrl",
        "data/unitychan_5.mtrl",
        "data/unitychan_6.mtrl",
        "data/unitychan_7.mtrl",
        "data/unitychan_8.mtrl",
    };

    auto result = m_mesh->loadMaterials(
        device,
        COUNTOF(mtrls), mtrls);
    VRETURN(result);

    static const char* mtrlTexName[] = {
        "face_00.tga",
        "eyeline_00.tga",
        "eye_iris_L_00.tga",
        "eye_iris_R_00.tga",
        "body_01.tga",
        "eyeline_00.tga",
        "hair_01.tga",
        "skin_01.tga",
        "cheek_00.tga",
    };

    IZ_ASSERT(COUNTOF(mtrlTexName) == img->GetTexNum());

    // 基本的には事前にマテリアル名とシェーダ名を一致させておくべきだが
    // シェーダ名を強制的に変更することもできる
    // シェーダ名を見てマテリアルをバインディングする
    shd->SetName("Phong");

    for (IZ_UINT i = 0; i < COUNTOF(mtrlTexName); i++) {
        result = m_mesh->bindMaterial(
            i,
            izanagi::CKey(mtrlTexName[i]),
            img->GetTexture(i),
            shd);
        IZ_ASSERT(result);
    }

    return IZ_TRUE;
}

void PlayerCharacter::initAnimation(izanagi::IMemoryAllocator* allocator)
{
    m_anm = izanagi::engine::AnimationStateMachine::create(allocator);
    IZ_ASSERT(m_anm);

    static const struct {
        const char* type;
        const char* path;
    } anmInfo[] = {
        { "idle", "data/unitychan_WAIT02.anm" },
        { "move", "data/unitychan_RUN00_F.anm" },
    };

    for (IZ_UINT i = 0; i < COUNTOF(anmInfo); i++) {
        izanagi::CFileInputStream in;
        auto res = in.Open(anmInfo[i].path);
        IZ_ASSERT(res);

        auto anm = izanagi::CAnimation::CreateAnimation(
            allocator,
            &in);
        IZ_ASSERT(anm);

        auto node = m_anm->addNode(anmInfo[i].type, anm);
        IZ_ASSERT(node);

        SAFE_RELEASE(anm);
    }

    m_anm->addBehaviour(nullptr, "idle");

    auto speed = m_anm->addConditionValue("speed");

    {
        auto behaviour = m_anm->addBehaviour("idle", "move");
        behaviour->setTransitionTime(0.1f);
        behaviour->setName("idle -> move");

        auto cond = behaviour->addCondition(
            allocator,
            speed,
            izanagi::engine::StateMachineCondition::Type::Float,
            izanagi::engine::StateMachineCondition::Cmp::Greater,
            izanagi::CValue(0.0f));
    }

    {
        auto behaviour = m_anm->addBehaviour("move", "idle");
        behaviour->setTransitionTime(0.1f);
        behaviour->setName("move -> idle");

        auto cond = behaviour->addCondition(
            allocator,
            speed,
            izanagi::engine::StateMachineCondition::Type::Float,
            izanagi::engine::StateMachineCondition::Cmp::LessEqual,
            izanagi::CValue(0.0f));
    }
}

// 解放.
void PlayerCharacter::release()
{
    SAFE_RELEASE(m_anm);

    SAFE_RELEASE(m_mesh);
    SAFE_RELEASE(m_ctrl);
}

// 更新.
void PlayerCharacter::update(
    izanagi::graph::CGraphicsDevice* device,
    const izanagi::CCamera& camera,
    IZ_FLOAT elapsed)
{
    izanagi::math::CVector3 dir(0, 0, 0);
    auto value = GetTargetDirection(m_forward, m_right, dir);

    MoveForward(dir, value);

    auto mdl = m_mesh->getMdl();

    m_anm->update(
        elapsed,
        mdl->GetSkeleton());

    // TODO
    mdl->Update();
}

void PlayerCharacter::prepareToRender(
    const izanagi::CCamera& camera,
    izanagi::CRenderGraph& renderGraph)
{
    // レンダーグラフに登録
    m_ctrl->registerToRenderGraph(
        camera,
        renderGraph);
}

void PlayerCharacter::updateDirection(IZ_FLOAT value)
{
    const auto mtxL2W = m_ctrl->getL2W();
    m_dir.Set(mtxL2W.m[2][0], mtxL2W.m[2][1], mtxL2W.m[2][2]);
    m_dir = m_dir * (value < 0.0f ? -1.0f : 1.0f);
}

IZ_FLOAT PlayerCharacter::GetTargetDirection(
    IZ_FLOAT forward, IZ_FLOAT right,
    izanagi::math::CVector3& dir)
{
    IZ_ASSERT(m_camera);

    const auto& camPos = m_camera->GetParam().pos;
    const auto& pos = m_ctrl->position();

    // Compute forward direction.
    auto dirForward = pos - camPos;
    dirForward.y = 0.0f;
    dirForward.Normalize();

    // Compute right direction.
    izanagi::math::CVector4 dirRight = QuatToRotateRight * dirForward;

    auto f = izanagi::math::CMath::Absf(forward);
    auto r = izanagi::math::CMath::Absf(right);

    auto total = 1.0f / (f + r);

    // Compute normalized magnitude.
    forward = forward * total;
    right = right * total;

    // Compute direction.
    auto tmp = dirForward * forward + dirRight * right;
    dir.Set(tmp.x, tmp.y, tmp.z);

    // Return total magnitude.
    return (f + r) * 0.5f;
}

void PlayerCharacter::MoveForward(
    izanagi::math::CVector3& dir,
    IZ_FLOAT value)
{
    // Set character speed to animation controller.
    m_anm->setConditionValue("speed", izanagi::math::CMath::Absf(value));

    if (value == 0.0f)
    {
        // If value is 0.0f, character don't move.
        m_state = State::Idle;
        m_slerp = 0.0f;
    }
    else
    {
        if (m_state == State::Idle)
        {
            if (!m_dir.Equals(dir) || value < 0.0f)
            {
                // If the character direction is different from the main camera direction, the character will rotate.
                m_state = State::Rotate;
            }
            else
            {
                // If the character direction equal to the main camera direction, the character will move to the direction.
                m_dir = dir;
                m_state = State::Move;
            }
        }

        if ((m_state & State::Move) > 0)
        {
            if (!m_dir.Equals(dir))
            {
                // Update parameters to move character with rotation.

                // 移動と回転を同時に行う.
                m_state |= State::Rotate;

                // Reset current rotation.
                m_slerp = 0.0f;

                auto angle = IZ_RAD2DEG(::atan2f(dir.z, dir.x));
                angle -= 90.0f;

                if (value < 0.0f)
                {
                    // Rotate to opposite.
                    angle -= 180.0f;
                }

                // Compute target rotation.
                m_toQuat = izanagi::math::CQuat::AngleAxis(
                    IZ_DEG2RAD(-angle),
                    izanagi::math::CVector3::yup());

                // Get current rotation.
                m_fromQuat = m_ctrl->rotation();
            }

            if ((m_state & State::Rotate) > 0)
            {
                // Advance slerp.
                m_slerp += 0.01f;

                auto rotation = izanagi::math::SQuat::Slerp(m_fromQuat, m_toQuat, m_slerp);
                m_ctrl->rotation() = rotation;

                if (m_slerp >= 1.0f)
                {
                    // Finish slerp.

                    m_slerp = 0.0f;
                    m_state = State::Move;
                }

                updateDirection(value);
            }

            //dir = m_dir * 0.01f;
            dir = m_dir;

            m_ctrl->move(dir);

            izanagi::math::CVector4 camPos(m_camera->GetParam().pos);
            camPos += dir;
            m_camera->SetPos(camPos);

            izanagi::math::CVector4 camAt(m_camera->GetParam().ref);
            camAt += dir;
            m_camera->SetAt(camAt);

            //Camera.main.transform.position = dir;
        }
        else if (m_state == State::Rotate)
        {
            if (m_slerp == 0.0f)
            {
                // Initialize parameter for slerp.

                auto angle = IZ_RAD2DEG(::atan2f(dir.z, dir.x));
                angle -= 90.0f;

                if (value < 0.0f)
                {
                    // Rotate to opposite.
                    angle -= 180.0f;
                }

                // Compute target rotation.
                m_toQuat = izanagi::math::CQuat::AngleAxis(
                    IZ_DEG2RAD(-angle),
                    izanagi::math::CVector3::yup());

                // Get current rotation.
                m_fromQuat = m_ctrl->rotation();
            }

            // Advance slerp.
            m_slerp += 0.1f;

            auto rotation = izanagi::math::SQuat::Slerp(m_fromQuat, m_toQuat, m_slerp);
            m_ctrl->rotation() = rotation;

            if (m_slerp >= 1.0f)
            {
                // Finish slerp.

                m_slerp = 0.0f;
                m_state = State::Move;
            }

            updateDirection(value);
        }
    }
}

namespace {
    inline void _SetShaderParam(
        izanagi::shader::IShader* shader,
        const char* name,
        const void* value,
        IZ_UINT bytes)
    {
        izanagi::shader::IZ_SHADER_HANDLE handle = shader->GetParameterByName(name);
        IZ_ASSERT(handle != 0);

        shader->SetParamValue(
            handle,
            value,
            bytes);
    }
}

// 描画.
void PlayerCharacter::render(
    izanagi::graph::CGraphicsDevice* device,
    const izanagi::CCamera& camera,
    izanagi::CRenderGraph* renderGraph,
    izanagi::CSceneRenderer* renderer)
{
#if 0
    m_Shd->Begin(device, 0, IZ_TRUE);

    m_Shd->BeginPass(0);

    izanagi::sample::CSampleCamera& camera = GetCamera();

    const izanagi::math::SMatrix44& mtxW2C = camera.GetParam().mtxW2C;
    _SetShaderParam(m_Shd, "g_mW2C", &mtxW2C, sizeof(mtxW2C));

    m_Shd->CommitChanges(device);

    device->SetTexture(0, m_Img->GetTexture(0));

    m_Mdl->Render(device);

    m_Shd->EndPass();

    m_Shd->End(device);
#else
    auto mtrl = m_mesh->getMaterial(0);
    auto shd = mtrl->GetShader();

    auto mtxL2W = m_ctrl->getL2W();
    _SetShaderParam(shd, "g_mL2W", &mtxL2W, sizeof(mtxL2W));

    const izanagi::math::SMatrix44& mtxW2C = camera.GetParam().mtxW2C;
    _SetShaderParam(shd, "g_mW2C", &mtxW2C, sizeof(mtxW2C));

    auto handler = m_mesh->getMshRenderHandler();

    // 描画
    renderGraph->Render(
        device,
        renderer,
        handler);
#endif
}

IZ_BOOL PlayerCharacter::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    if (key == izanagi::sys::E_KEYBOARD_BUTTON_UP)
    {
        m_forward = 1.0f;
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_DOWN)
    {
        m_forward = -1.0f;
    }

    if (key == izanagi::sys::E_KEYBOARD_BUTTON_RIGHT)
    {
        m_right = 1.0f;
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_LEFT)
    {
        m_right = -1.0f;
    }

    return IZ_TRUE;
}

void PlayerCharacter::OnKeyUp(izanagi::sys::E_KEYBOARD_BUTTON key)
{
#if 1
    if (key == izanagi::sys::E_KEYBOARD_BUTTON_UP)
    {
        m_forward = 0.0f;
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_DOWN)
    {
        m_forward = 0.0f;
    }
#endif

#if 1
    if (key == izanagi::sys::E_KEYBOARD_BUTTON_RIGHT)
    {
        m_right = 0.0f;
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_LEFT)
    {
        m_right = 0.0f;
    }
#endif
}