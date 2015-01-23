#include "Scene.h"
#include "Ring.h"
#include "Utility.h"

Scene Scene::instance;

Scene& Scene::Instance()
{
    return instance;
}

Scene::Scene()
{
    m_Ring = IZ_NULL;
    m_BevelShader = IZ_NULL;
}

Scene::~Scene()
{
}

IZ_BOOL Scene::Init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device)
{
    m_Ring = Ring::Create(allocator, device);
    VRETURN(m_Ring != IZ_NULL);

    // シェーダ
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/BevelShader.shd"));

        m_BevelShader = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
                    allocator,
                    device,
                    &in);
        VRETURN(m_BevelShader != IZ_NULL);
    }

    return IZ_TRUE;
}

void Scene::Terminate()
{
    SAFE_RELEASE(m_Ring);
    SAFE_RELEASE(m_BevelShader);
}

void Scene::Render(
    const izanagi::CCamera& camera,
    izanagi::graph::CGraphicsDevice* device)
{
    izanagi::math::SMatrix44 mtxL2W;
    izanagi::math::SMatrix44::SetUnit(mtxL2W);

    const izanagi::SCameraParam& param = camera.GetParam();

    m_BevelShader->Begin(device, 0, IZ_FALSE);
    {
        if (m_BevelShader->BeginPass(0)) {
            // パラメータ設定
            Utility::SetShaderParam(
                m_BevelShader,
                "g_mL2W",
                (void*)&mtxL2W,
                sizeof(mtxL2W));

            Utility::SetShaderParam(
                m_BevelShader,
                "g_mW2C",
                (void*)&param.mtxW2C,
                sizeof(param.mtxW2C));

            izanagi::SParallelLightParam parallelLight;

            // ライトパラメータ
            {
                // Ambient Light Color
                izanagi::SAmbientLightParam ambient;
                ambient.color.Set(0.0f, 0.0f, 0.0f);

                // Parallel Light Color
                parallelLight.color.Set(1.0f, 1.0f, 1.0f);

                // Parallel Light Direction
                parallelLight.vDir.Set(-1.0f, -1.0f, -1.0f);
                izanagi::math::SVector4::Normalize(parallelLight.vDir, parallelLight.vDir);

                // マテリアル
                izanagi::SMaterialParam mtrl;
                {
                    mtrl.vDiffuse.Set(1.0f, 1.0f, 1.0f, 1.0f);
                    mtrl.vAmbient.Set(1.0f, 1.0f, 1.0f, 1.0f);
                    mtrl.vSpecular.Set(1.0f, 1.0f, 1.0f, 20.0f);
                }

                Utility::SetShaderParam(m_BevelShader, "g_vLitParallelColor", &parallelLight.color, sizeof(parallelLight.color));
                Utility::SetShaderParam(m_BevelShader, "g_vLitAmbientColor", &ambient.color, sizeof(ambient.color));

                Utility::SetShaderParam(m_BevelShader, "g_vMtrlDiffuse", &mtrl.vDiffuse, sizeof(mtrl.vDiffuse));
                Utility::SetShaderParam(m_BevelShader, "g_vMtrlAmbient", &mtrl.vAmbient, sizeof(mtrl.vAmbient));
                Utility::SetShaderParam(m_BevelShader, "g_vMtrlSpecular", &mtrl.vSpecular, sizeof(mtrl.vSpecular));
            }

            {
                // ライトの方向をローカル座標に変換する

                // ライトの方向はワールド座標なので World -> Localマトリクスを計算する
                izanagi::math::SMatrix44 mtxW2L;
                izanagi::math::SMatrix44::Inverse(mtxW2L, mtxL2W);

                // World -> Local
                izanagi::math::SVector4 parallelLightLocalDir;
                izanagi::math::SMatrix44::ApplyXYZ(
                    parallelLightLocalDir,
                    parallelLight.vDir,
                    mtxW2L);

                Utility::SetShaderParam(
                    m_BevelShader,
                    "g_vLitParallelDir",
                    (void*)&parallelLightLocalDir,
                    sizeof(parallelLightLocalDir));

                // L2V = L2W * W2V の逆行列を計算する
                izanagi::math::SMatrix44 mtxV2L;
                izanagi::math::SMatrix44::Mul(mtxV2L, mtxL2W, param.mtxW2V);
                izanagi::math::SMatrix44::Inverse(mtxV2L, mtxV2L);

                // ビュー座標系における視点は常に原点
                izanagi::math::CVector4 eyePos(0.0f, 0.0f, 0.0f, 1.0f);

                // 視点のローカル座標を計算する
                izanagi::math::SMatrix44::Apply(eyePos, eyePos, mtxV2L);

                Utility::SetShaderParam(
                    m_BevelShader,
                    "g_vEye",
                    (void*)&eyePos,
                    sizeof(eyePos));
            }

            m_BevelShader->CommitChanges(device);

            m_Ring->Draw(device);
        }
    }
    m_BevelShader->End(device);
}
