#include "MotionStateMachineApp.h"

MotionStateMachineApp::MotionStateMachineApp()
{
}

MotionStateMachineApp::~MotionStateMachineApp()
{
}

// 初期化.
IZ_BOOL MotionStateMachineApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    initModel(allocator, device);

    initStateMachine(allocator);

    m_Timeline.Init(
        m_Anm[0]->GetAnimationTime(),
        0.0f);
    m_Timeline.EnableLoop(IZ_TRUE);
    m_Timeline.AutoReverse(IZ_FALSE);
    m_Timeline.Start();
    m_Timeline.Reset();

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
        ReleaseInternal();
    }

    return IZ_TRUE;
}

IZ_BOOL MotionStateMachineApp::initModel(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device)
{
    IZ_BOOL result = IZ_FALSE;

    static const char* imgPath = "data/UnityChanImg.img";
    static const char* mshPath = "data/unitychan.msh";
    static const char* sklPath = "data/unitychan.skl";
    static const char* shdPath = "data/SkinShader.shd";

    // Texture
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open(imgPath));

        m_Img = izanagi::CImage::CreateImage(
            allocator,
            device,
            &in);

        VGOTO(result = (m_Img != IZ_NULL), __EXIT__);
    }

    // Mesh
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open(mshPath));

        m_Msh = izanagi::CMesh::CreateMesh(
            allocator,
            device,
            &in);

        VGOTO(result = (m_Msh != IZ_NULL), __EXIT__);
    }

    // Skelton
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open(sklPath));

        m_Skl = izanagi::CSkeleton::CreateSkeleton(
            allocator,
            &in);

        VGOTO(result = (m_Skl != IZ_NULL), __EXIT__);
    }

    // Shader
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open(shdPath));

        m_Shd = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
            allocator,
            device,
            &in);

        VGOTO(result = (m_Shd != IZ_NULL), __EXIT__);

        // ジョイントマトリクスを描画時に設定するためのハンドラ
        m_MdlRenderHandler = izanagi::IMshRenderHandler::CreateMshRenderHandler<izanagi::sample::CSampleMdlRenderHandler>(allocator);
        m_MdlRenderHandler->SetShader(m_Shd);
        VGOTO(result = (m_MdlRenderHandler != IZ_NULL), __EXIT__);
    }

    // Model
    {
        m_Mdl = izanagi::CModel::CreateModel(
            allocator,
            m_Msh,
            m_Skl,
            m_MdlRenderHandler);

        VGOTO(result = (m_Mdl != IZ_NULL), __EXIT__);
    }

    // レンダーグラフ
    m_RenderGraph = izanagi::CRenderGraph::CreateRenderGraph(allocator, 5);
    VGOTO(result = (m_RenderGraph != IZ_NULL), __EXIT__);

    // レンダラ
    m_Renderer = izanagi::CSceneRenderer::CreateSceneRenderer(allocator);
    VGOTO(result = (m_Renderer != IZ_NULL), __EXIT__);

    CreateMaterial(allocator);

__EXIT__:

    return result;
}

IZ_BOOL MotionStateMachineApp::CreateMaterial(izanagi::IMemoryAllocator* allocator)
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

    static const IZ_UINT IMG_IDX = 0;

    izanagi::CFileInputStream in;

    IZ_ASSERT(COUNTOF(mtrls) <= COUNTOF(m_Mtrl));

    for (IZ_UINT i = 0; i < COUNTOF(mtrls); i++) {
        VRETURN(in.Open(mtrls[i]));

        m_Mtrl[i] = izanagi::CMaterial::CreateMaterial(allocator, &in);
        VRETURN(m_Mtrl[i] != IZ_NULL);

        in.Finalize();
    }

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

    // 基本的には事前にマテリアル名とシェーダ名を一致させておくべきだが
    // シェーダ名を強制的に変更することもできる
    // シェーダ名を見てマテリアルをバインディングする
    m_Shd->SetName("Phong");

    for (IZ_UINT i = 0; i < COUNTOF(m_Mtrl); i++) {
        // マテリアルに対応するテクスチャとシェーダを設定
        m_Mtrl[i]->SetTexture(mtrlTexName[i], m_Img->GetTexture(i));
        m_Mtrl[i]->SetShader(m_Shd);

        // メッシュにマテリアルを設定
        m_Mdl->GetMesh()->SetMaterial(0, m_Mtrl[i]);
    }

    return IZ_TRUE;
}

void MotionStateMachineApp::initAnimation(izanagi::IMemoryAllocator* allocator)
{
    static const char* anm[] = {
        "data/unitychan_WAIT02.anm",
        "data/unitychan_WALK00_F.anm",
        "data/unitychan_LOSE00.anm",
        "data/unitychan_RUN00_F.anm",
        "data/unitychan_UMATOBI00.anm",
    };

    IZ_C_ASSERT(COUNTOF(anm) == State::Num);

    for (IZ_UINT i = 0; i < State::Num; i++) {
        izanagi::CFileInputStream in;
        auto res = in.Open(anm[i]);
        IZ_ASSERT(res);

        m_Anm[i] = izanagi::CAnimation::CreateAnimation(
            allocator,
            &in);
    }
}

void MotionStateMachineApp::initStateMachine(izanagi::IMemoryAllocator* allocator)
{
    initAnimation(allocator);

    izanagi::engine::AnimationStateMachine::StateHandler = [] (const char* name, IZ_FLOAT t, IZ_FLOAT duration) {
        IZ_PRINTF("(%s) [%.3f / %.3f]\n", name, t, duration);
    };

    m_stateMachine = izanagi::engine::AnimationStateMachine::create(allocator);

    auto idle = m_stateMachine->addNode("idle", m_Anm[State::Idle]);
    auto move = m_stateMachine->addNode("move", m_Anm[State::Walk]);
    auto lose = m_stateMachine->addNode("lose", m_Anm[State::Lose], IZ_FALSE);

    m_stateMachine->addBehaviour(nullptr, "idle");

    auto speed = m_stateMachine->addConditionValue("speed");
    auto isLose = m_stateMachine->addConditionValue("isLose");

    {
        auto behaviour = m_stateMachine->addBehaviour("idle", "move");
        behaviour->setTransitionTime(1.0f);
        behaviour->setName("idle -> move");

        auto cond = behaviour->addCondition(
            allocator,
            speed,
            izanagi::engine::StateMachineCondition::Type::Float,
            izanagi::engine::StateMachineCondition::Cmp::GreaterEqual,
            izanagi::CValue(0.5f));
    }

    {
        auto behaviour = m_stateMachine->addBehaviour("move", "idle");
        behaviour->setTransitionTime(1.0f);
        behaviour->setName("move -> idle");

        auto cond = behaviour->addCondition(
            allocator,
            speed,
            izanagi::engine::StateMachineCondition::Type::Float,
            izanagi::engine::StateMachineCondition::Cmp::Less,
            izanagi::CValue(0.5f));
    }

    {
        auto behaviour = m_stateMachine->addBehaviour("idle", "lose");
        behaviour->setTransitionTime(1.0f);
        behaviour->setName("idle -> lose");

        auto cond = behaviour->addCondition(
            allocator,
            isLose,
            izanagi::engine::StateMachineCondition::Type::Bool,
            izanagi::engine::StateMachineCondition::Cmp::Equal,
            izanagi::CValue(IZ_TRUE));

        lose->setExitFunc([&] {
            m_isLose = IZ_FALSE;
            m_stateMachine->setConditionValue("isLose", m_isLose);
        });
    }

    {
        auto behaviour = m_stateMachine->addBehaviour("lose", "idle");
        behaviour->setTransitionTime(1.0f);
        behaviour->setName("lose -> idle");

        auto cond = behaviour->addCondition(
            allocator,
            isLose,
            izanagi::engine::StateMachineCondition::Type::Bool,
            izanagi::engine::StateMachineCondition::Cmp::NotEqual,
            izanagi::CValue(IZ_TRUE));
    }

    {
        initLayer(allocator);

        {
            auto behaviour = m_stateMachine->addBehaviour(move, m_layer);
            behaviour->setTransitionTime(1.0f);
            behaviour->setName("move -> layer");

            auto cond = behaviour->addCondition(
                allocator,
                speed,
                izanagi::engine::StateMachineCondition::Type::Float,
                izanagi::engine::StateMachineCondition::Cmp::Greater,
                izanagi::CValue(0.7f));
        }

        {
            auto behaviour = m_stateMachine->addBehaviour(m_layer, move);
            behaviour->setTransitionTime(1.0f);
            behaviour->setName("layer -> move");
        }
    }
}

void MotionStateMachineApp::initLayer(izanagi::IMemoryAllocator* allocator)
{
    m_layer = izanagi::engine::AnimationStateMachine::create(allocator);

    auto run = m_layer->addNode("run", m_Anm[State::Run], IZ_FALSE);
    auto umatobi = m_layer->addNode("umatobi", m_Anm[State::Umatobi], IZ_FALSE);

    m_layer->addBehaviour(nullptr, "run");
    m_layer->addBehaviour("umatobi", nullptr);

    m_willUmatobi = m_layer->addConditionValue("willUmatobi");

    auto behaviour = m_layer->addBehaviour("run", "umatobi");
    behaviour->setTransitionTime(0.2f);
    behaviour->setName("run -> umatobi");

    auto cond = behaviour->addCondition(
        allocator,
        m_willUmatobi,
        izanagi::engine::StateMachineCondition::Type::Bool,
        izanagi::engine::StateMachineCondition::Cmp::Equal,
        izanagi::CValue(IZ_TRUE));

    run->setWillAutoReturnToPreviousNode(IZ_FALSE);
    run->setCallbackIfNoneLoopAnimationOver([&] {
        m_willUmatobi->setValue(IZ_TRUE);
    });
    umatobi->setExitFunc([&] {
        m_willUmatobi->setValue(IZ_FALSE);
        m_speed = 0.5f;
    });
}

// 解放.
void MotionStateMachineApp::ReleaseInternal()
{
    SAFE_RELEASE(m_stateMachine);
    SAFE_RELEASE(m_layer);

    SAFE_RELEASE(m_willUmatobi);

    SAFE_RELEASE(m_Img);
    SAFE_RELEASE(m_Mdl);
    SAFE_RELEASE(m_Msh);
    SAFE_RELEASE(m_Skl);
    SAFE_RELEASE(m_Shd);

    for (IZ_UINT i = 0; i < COUNTOF(m_Mtrl); i++) {
        SAFE_RELEASE(m_Mtrl[i]);
    }

    SAFE_RELEASE(m_RenderGraph);
    SAFE_RELEASE(m_Renderer);

    SAFE_RELEASE(m_MdlRenderHandler);

    for (IZ_UINT i = 0; i < COUNTOF(m_Anm); i++) {
        SAFE_RELEASE(m_Anm[i]);
    }
} 

// 更新.
void MotionStateMachineApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();

    m_speed = izanagi::math::CMath::Clamp(m_speed, 0.0f, 1.0f);

    m_stateMachine->setConditionValue("speed", m_speed);
    m_stateMachine->setConditionValue("isLose", m_isLose);

    IZ_FLOAT elapsed = GetTimer(0).GetTime();
    //elapsed /= 1000.0f;
    elapsed = 16.67f / 1000.0f;

    m_Timeline.Advance(elapsed);

    m_stateMachine->update(elapsed, m_Mdl->GetSkeleton());

    m_Mdl->Update();

    // レンダーグラフに登録
    m_RenderGraph->BeginRegister();
    {
        // 位置は原点なので
        m_RenderGraph->Register(
            GetCamera(),
            izanagi::math::CVector4(),
            m_Mdl);
    }
    m_RenderGraph->EndRegister();
}

namespace {
    inline void _SetShaderParam(
        izanagi::shader::CShaderBasic* shader,
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
void MotionStateMachineApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
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
    izanagi::sample::CSampleCamera& camera = GetCamera();

    const izanagi::math::SMatrix44& mtxW2C = camera.GetParam().mtxW2C;
    _SetShaderParam(m_Shd, "g_mW2C", &mtxW2C, sizeof(mtxW2C));

    // 描画
    m_RenderGraph->Render(
        device,
        m_Renderer,
        m_MdlRenderHandler);
#endif

    if (device->Begin2D()) {
        izanagi::CDebugFont* debugFont = GetDebugFont();

        debugFont->Begin(device, 0, izanagi::CDebugFont::FONT_SIZE * 2);

        debugFont->DBPrint(
            device,
            "spped [%.2f]\n",
            m_speed);

        debugFont->End();

        device->End2D();
    }
}

IZ_BOOL MotionStateMachineApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    if (key == izanagi::sys::E_KEYBOARD_BUTTON_UP) {
        m_speed += 0.01f;
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_DOWN) {
        m_speed -= 0.01f;
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_SPACE) {
        m_isLose = !m_isLose;
    }

    return IZ_TRUE;
}
