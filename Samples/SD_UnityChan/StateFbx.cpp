#include "StateFbx.h"
#include "izGraph.h"
#include "izSystem.h"
#include "izSampleKit.h"

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
        "data/SD_UnityChanImg.img",
        "data/SD_unitychan.msh",
        "data/SD_unitychan.skl",
        "data/SkinShader.shd");

#if 1
    // Animation.
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/SD_unitychan_motion.anm"));

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
#endif

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
#if 1
    if (m_enableStepAnm) {
        if (m_step) {
            m_Timeline.Advance(16.67f / 1000.0f);
        }
        m_step = IZ_FALSE;
    }
    else {
        // 時間更新
        IZ_FLOAT fElapsed = m_App->GetTimer(0).GetTime();
        fElapsed /= 1000.0f;
        m_Timeline.Advance(fElapsed);
    }

    //m_Timeline.Advance(0.5f);
    IZ_FLOAT t = m_Timeline.GetTime();

    // アニメーション適用
    m_Mdl->ApplyAnimation(t, m_Anm);
#endif

    m_Mdl->Update();
}

IZ_BOOL CStateFbx::Render(izanagi::graph::CGraphicsDevice* device)
{
    IZ_BOOL ret = CStateBase::Render(device);

#if 0
    static char tmp[128];

    IZ_FLOAT t = m_Timeline.GetTime();
    IZ_SPRINTF(tmp, sizeof(tmp), "(%s) [%.3f]", anmFileName[posAnm], t);

    RenderName(device, tmp);
#endif

    return ret;
}

IZ_BOOL CStateFbx::CreateMaterial(izanagi::IMemoryAllocator* allocator)
{
    static const char* mtrls[] = {
        "data/SD_unitychan_0.mtrl",
        "data/SD_unitychan_1.mtrl",
        "data/SD_unitychan_2.mtrl",
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
        "utc_all2.tga",
    };
    
    // 基本的には事前にマテリアル名とシェーダ名を一致させておくべきだが
    // シェーダ名を強制的に変更することもできる
    // シェーダ名を見てマテリアルをバインディングする
    m_Shd->SetName("Phong");

    for (IZ_UINT i = 0; i < COUNTOF(m_Mtrl); i++) {
        // マテリアルに対応するテクスチャとシェーダを設定
        m_Mtrl[i]->SetTexture(mtrlTexName[0], m_Img->GetTexture(0));
        m_Mtrl[i]->SetShader(m_Shd);

        // メッシュにマテリアルを設定
        m_Mdl->GetMesh()->SetMaterial(0, m_Mtrl[i]);
    }

    return IZ_TRUE;
}

IZ_BOOL CStateFbx::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    if (key == izanagi::sys::E_KEYBOARD_BUTTON_SPACE) {
        m_enableStepAnm = !m_enableStepAnm;
    }

    if (m_enableStepAnm) {
        if (key == izanagi::sys::E_KEYBOARD_BUTTON_RETURN) {
            m_step = IZ_TRUE;
        }
    }

    return IZ_TRUE;
}
