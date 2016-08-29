#include "PlayerCharacter.h"

PlayerCharacter::PlayerCharacter()
{
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

    // カメラ
    camera.Init(
        izanagi::math::CVector4(0.0f, 5.0f, 30.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 5.0f, 0.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        500.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)device->GetBackBufferWidth() / device->GetBackBufferHeight());
    camera.Update();

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

    // レンダーグラフ
    m_RenderGraph = izanagi::CRenderGraph::CreateRenderGraph(allocator, 5);
    VGOTO(result = (m_RenderGraph != IZ_NULL), __EXIT__);

    // レンダラ
    m_Renderer = izanagi::CSceneRenderer::CreateSceneRenderer(allocator);
    VGOTO(result = (m_Renderer != IZ_NULL), __EXIT__);

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

    SAFE_RELEASE(m_RenderGraph);
    SAFE_RELEASE(m_Renderer);

    SAFE_RELEASE(m_mesh);
    SAFE_RELEASE(m_ctrl);
}

// 更新.
void PlayerCharacter::update(
    izanagi::graph::CGraphicsDevice* device,
    const izanagi::CCamera& camera,

    IZ_FLOAT elapsed)
{
    m_speed = izanagi::math::CMath::Clamp(m_speed, 0.0f, 1.0f);

    m_anm->setConditionValue("speed", m_speed);

    auto mdl = m_mesh->getMdl();

    m_anm->update(
        elapsed, 
        mdl->GetSkeleton());

    // TODO
    mdl->Update();

    // レンダーグラフに登録
    m_RenderGraph->BeginRegister();
    {
        m_ctrl->registerToRenderGraph(
            camera,
            *m_RenderGraph);
    }
    m_RenderGraph->EndRegister();
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
    const izanagi::CCamera& camera)
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

    const izanagi::math::SMatrix44& mtxW2C = camera.GetParam().mtxW2C;
    _SetShaderParam(shd, "g_mW2C", &mtxW2C, sizeof(mtxW2C));

    auto handler = m_mesh->getMshRenderHandler();

    // 描画
    m_RenderGraph->Render(
        device,
        m_Renderer,
        handler);
#endif
}

IZ_BOOL PlayerCharacter::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    if (key == izanagi::sys::E_KEYBOARD_BUTTON_UP) {
        m_speed += 0.01f;
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_DOWN) {
        m_speed -= 0.01f;
    }

    return IZ_TRUE;
}
