#include "StateFbx.h"
#include "izGraph.h"
#include "izSystem.h"
#include "StateManager.h"

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
    IZ_BOOL result = InitObject(
        allocator,
        device,
        30.0f,
        "data/ModelImage.img",
        "data/SeymourFbx.msh",
        "data/SeymourFbx.skl",
        "data/SkinShader.shd");

    return result;
}

// ステートから抜ける（終了）.
IZ_BOOL CStateFbx::Leave()
{
    ReleaseObject();
    return IZ_TRUE;
}

IZ_BOOL CStateFbx::Render(izanagi::graph::CGraphicsDevice* device)
{
    IZ_BOOL ret = CStateBase::Render(device);

    RenderName(device, "Fbx");

    return ret;
}

IZ_BOOL CStateFbx::CreateMaterial(izanagi::IMemoryAllocator* allocator)
{
    static const char* mtrls[] = {
        "data/SeymourFbx_0.mtrl",
        "data/SeymourFbx_1.mtrl",
        "data/SeymourFbx_2.mtrl",
        "data/SeymourFbx_3.mtrl",
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

    // 基本的には事前にマテリアル名とシェーダ名を一致させておくべきだが
    // シェーダ名を強制的に変更することもできる
    // シェーダ名を見てマテリアルをバインディングする
    m_Shd->SetName("Phong");

    for (IZ_UINT i = 0; i < COUNTOF(m_Mtrl); i++) {
        // マテリアルに対応するテクスチャとシェーダを設定
        m_Mtrl[i]->SetTexture("boy_10.dds", m_Img->GetTexture(IMG_IDX));
        m_Mtrl[i]->SetShader(m_Shd);

        // メッシュにマテリアルを設定
        m_Mdl->GetMesh()->SetMaterial(0, m_Mtrl[i]);
    }

    return IZ_TRUE;
}
