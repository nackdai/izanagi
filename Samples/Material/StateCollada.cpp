#include "StateCollada.h"
#include "izGraph.h"
#include "izSystem.h"
#include "StateManager.h"

CStateCollada::CStateCollada(izanagi::sample::CSampleApp* app)
: CStateBase(app)
{
}

CStateCollada::~CStateCollada()
{
    Destroy();
}

// 開始
IZ_BOOL CStateCollada::Enter(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::CValue& arg)
{
    IZ_BOOL result = InitObject(
        allocator,
        device,
        30.0f,
        "data/ModelImage.img",
        "data/Seymour.msh",
        "data/Seymour.skl",
        "data/SkinShader.shd");

    return result;
}

// ステートから抜ける（終了）.
IZ_BOOL CStateCollada::Leave()
{
    ReleaseObject();
    return IZ_TRUE;
}

IZ_BOOL CStateCollada::Render(izanagi::graph::CGraphicsDevice* device)
{
    IZ_BOOL ret = CStateBase::Render(device);

    RenderName(device, "Collada");

    return ret;
}

IZ_BOOL CStateCollada::CreateMaterial(izanagi::IMemoryAllocator* allocator)
{
    static const char* mtrls[] = {
        "data/Seymour_0.mtrl",
        "data/Seymour_1.mtrl",
        "data/Seymour_2.mtrl",
        "data/Seymour_3.mtrl",
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
    m_Shd->SetName("DefaultShader");

    for (IZ_UINT i = 0; i < COUNTOF(m_Mtrl); i++) {
        // マテリアルに対応するテクスチャとシェーダを設定
        m_Mtrl[i]->SetTexture("boy_10.tga", m_Img->GetTexture(IMG_IDX));
        m_Mtrl[i]->SetShader(m_Shd);

        // メッシュにマテリアルを設定
        m_Mdl->GetMesh()->SetMaterial(0, m_Mtrl[i]);
    }

    return IZ_TRUE;
}
