#include "Scene.h"
#include "Ring.h"
#include "Utility.h"
#include "ItemManager.h"
#include "Configure.h"
#include "Environment.h"

Scene Scene::instance;

Scene& Scene::Instance()
{
    return instance;
}

Scene::Scene()
{
    m_Ring = IZ_NULL;
    m_BevelShader = IZ_NULL;
    m_BasicShader = IZ_NULL;
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

    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/BasicShader.shd"));

        m_BasicShader = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
                    allocator,
                    device,
                    &in);
        VRETURN(m_BasicShader != IZ_NULL);
    }

    return IZ_TRUE;
}

void Scene::Terminate()
{
    SAFE_RELEASE(m_Ring);
    SAFE_RELEASE(m_BevelShader);
    SAFE_RELEASE(m_BasicShader);
}

void Scene::Update(
    IZ_FLOAT time,
    izanagi::graph::CGraphicsDevice* device,
    const izanagi::CCamera& camera)
{
    ItemManager::Instance().Update(
        time,
        device,
        camera);
}

void Scene::Render(
    const izanagi::CCamera& camera,
    izanagi::graph::CGraphicsDevice* device)
{
    RenderByBasicShader(camera, device);
    RenderByBevelShader(camera, device);
}

void Scene::RenderByBevelShader(
    const izanagi::CCamera& camera,
    izanagi::graph::CGraphicsDevice* device)
{
    const izanagi::SCameraParam& param = camera.GetParam();

    m_BevelShader->Begin(device, 0, IZ_FALSE);
    {
        if (m_BevelShader->BeginPass(0)) {
            // パラメータ設定
            Utility::SetShaderParam(
                m_BevelShader,
                "g_mW2C",
                (void*)&param.mtxW2C,
                sizeof(param.mtxW2C));

            Environment::Instance().SetGlobalLightParam(
                camera,
                m_BevelShader);

#if 0
            m_BevelShader->CommitChanges(device);

            m_Ring->Draw(device);
#else
            ItemManager::Instance().RenderBox(device, m_BevelShader, camera);
            RenderRing(camera, device);
#endif
        }
    }
    m_BevelShader->End(device);
}

void Scene::RenderRing(
    const izanagi::CCamera& camera,
    izanagi::graph::CGraphicsDevice* device)
{
    izanagi::math::SMatrix44 mtxL2W;
    izanagi::math::SMatrix44::SetUnit(mtxL2W);

    IZ_FLOAT ringCenter = Configure::ItemHeight * 0.5f + Configure::RING_HEIGHT * 0.5f;
    
    {
        izanagi::math::SMatrix44::GetTrans(
            mtxL2W,
            0.0f,
            ringCenter + 0.5f,
            0.0f);

        Utility::SetShaderParam(
            m_BevelShader,
            "g_mL2W",
            (void*)&mtxL2W,
            sizeof(mtxL2W));

        Environment::Instance().SetLocalLightParam(
            camera,
            mtxL2W,
            m_BevelShader);

        m_BevelShader->CommitChanges(device);
        m_Ring->Draw(device);
    }

    {
        izanagi::math::SMatrix44::GetTrans(
            mtxL2W,
            0.0f,
            -(ringCenter + 0.5f),
            0.0f);

        Utility::SetShaderParam(
            m_BevelShader,
            "g_mL2W",
            (void*)&mtxL2W,
            sizeof(mtxL2W));

        Environment::Instance().SetLocalLightParam(
            camera,
            mtxL2W,
            m_BevelShader);

        m_BevelShader->CommitChanges(device);
        m_Ring->Draw(device);
    }
}

void Scene::RenderByBasicShader(
    const izanagi::CCamera& camera,
    izanagi::graph::CGraphicsDevice* device)
{
    const izanagi::SCameraParam& param = camera.GetParam();

    m_BasicShader->Begin(device, 0, IZ_FALSE);
    {
        if (m_BasicShader->BeginPass(0)) {
            Utility::SetShaderParam(
                m_BasicShader,
                "g_mW2C",
                (void*)&param.mtxW2C,
                sizeof(param.mtxW2C));

            ItemManager::Instance().RenderBoard(device, m_BasicShader);
        }
    }
    m_BasicShader->End(device);
}
