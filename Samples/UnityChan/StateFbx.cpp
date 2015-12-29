#include "StateFbx.h"
#include "izGraph.h"
#include "izSystem.h"
#include "izSampleKit.h"

static IZ_INT posAnm = 0;

static const char* anmFileName[] = {
    "data/unitychan_ARpose1.anm",
    "data/unitychan_ARpose2.anm",
    "data/unitychan_DAMAGED00.anm",
    "data/unitychan_DAMAGED01.anm",
    "data/unitychan_HANDUP00_R.anm",
    "data/unitychan_JUMP00.anm",
    "data/unitychan_JUMP00B.anm",
    "data/unitychan_JUMP01.anm",
    "data/unitychan_JUMP01B.anm",
    "data/unitychan_LOSE00.anm",
    "data/unitychan_REFLESH00.anm",
    "data/unitychan_RUN00_F.anm",
    "data/unitychan_RUN00_L.anm",
    "data/unitychan_RUN00_R.anm",
    "data/unitychan_SLIDE00.anm",
    "data/unitychan_UMATOBI00.anm",
    "data/unitychan_WAIT00.anm",
    "data/unitychan_WAIT01.anm",
    "data/unitychan_WAIT02.anm",
    "data/unitychan_WAIT03.anm",
    "data/unitychan_WAIT04.anm",
    "data/unitychan_WALK00_B.anm",
    "data/unitychan_WALK00_F.anm",
    "data/unitychan_WALK00_L.anm",
    "data/unitychan_WALK00_R.anm",
    "data/unitychan_WIN00.anm",
};

CStateFbx::CStateFbx(izanagi::sample::CSampleApp* app)
: CStateBase(app)
{
}

CStateFbx::~CStateFbx()
{
    Destroy();
}

// 開始
IZ_BOOL CStateFbx::Enter(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::CValue& arg)
{
    m_Allocator = allocator;

    IZ_BOOL result = InitObject(
        allocator,
        device,
        30.0f,
        "data/UnityChanImg.img",
        "data/unitychan.msh",
        "data/unitychan.skl",
        "data/SkinShader.shd");

    // Animation.
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open(anmFileName[posAnm]));

        m_Anm = izanagi::CAnimation::CreateAnimation(
            allocator,
            &in);

        result = (m_Anm != IZ_NULL);
        IZ_ASSERT(result);
    }

    // Timeline
    {
        m_Timeline.Init(
            m_Anm->GetAnimationTime(),
            0.0f);
        m_Timeline.EnableLoop(IZ_TRUE);
        m_Timeline.AutoReverse(IZ_FALSE);
        m_Timeline.Start();
        m_Timeline.Reset();
    }

    return result;
}

// ステートから抜ける（終了）.
IZ_BOOL CStateFbx::Leave()
{
    ReleaseObject();

    SAFE_RELEASE(m_Anm);

    return IZ_TRUE;
}

void CStateFbx::OnUpdate(
    IZ_FLOAT time,
    izanagi::graph::CGraphicsDevice* device)
{
    // 時間更新
    IZ_FLOAT fElapsed = m_App->GetTimer(0).GetTime();
    fElapsed /= 1000.0f;

    m_Timeline.Advance(fElapsed);
    //m_Timeline.Advance(0.5f);
    IZ_FLOAT t = m_Timeline.GetTime();

    // アニメーション適用
    m_Mdl->ApplyAnimation(t, m_Anm);

    m_Mdl->Update();
}

IZ_BOOL CStateFbx::Render(izanagi::graph::CGraphicsDevice* device)
{
    IZ_BOOL ret = CStateBase::Render(device);

    RenderName(device, anmFileName[posAnm]);

    return ret;
}

IZ_BOOL CStateFbx::CreateMaterial(izanagi::IMemoryAllocator* allocator)
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

IZ_BOOL CStateFbx::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    if (key == izanagi::sys::E_KEYBOARD_BUTTON_UP) {
        posAnm++;
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_DOWN) {
        posAnm--;
    }

    posAnm = izanagi::math::CMath::Clamp<IZ_INT>(posAnm, 0, COUNTOF(anmFileName) - 1);

    SAFE_RELEASE(m_Anm);

    izanagi::CFileInputStream in;
    VRETURN(in.Open(anmFileName[posAnm]));

    m_Anm = izanagi::CAnimation::CreateAnimation(
        m_Allocator,
        &in);

    IZ_ASSERT(m_Anm != IZ_NULL);

    // Reset Timeline
    {
        m_Timeline.Init(
            m_Anm->GetAnimationTime(),
            0.0f);
        m_Timeline.EnableLoop(IZ_TRUE);
        m_Timeline.AutoReverse(IZ_FALSE);
        m_Timeline.Start();
        m_Timeline.Reset();
    }

    return IZ_TRUE;
}
