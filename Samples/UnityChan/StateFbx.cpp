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
        "data/UnityChanImg.img",
        "data/unitychan.msh",
        "data/unitychan.skl",
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

    RenderName(device, "UnityChan");

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
