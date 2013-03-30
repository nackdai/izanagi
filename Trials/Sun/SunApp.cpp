#include "SunApp.h"
#include "Time.h"
#include "Ephemeris.h"
#include "Sun.h"

CSunApp::CSunApp()
{    
    m_Shader = IZ_NULL;
    m_Axis = IZ_NULL;
}

CSunApp::~CSunApp()
{
}

// 初期化.
IZ_BOOL CSunApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    // シェーダ
    {
        izanagi::CFileInputStream in;
        VGOTO(result = in.Open("data/BasicShader.shd"), __EXIT__);

        m_Shader = izanagi::CShaderBasic::CreateShader<izanagi::CShaderBasic>(
                    allocator,
                    device,
                    &in);
        VGOTO(result = (m_Shader != IZ_NULL), __EXIT__);
    }

    {
        IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
                    | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR;

        m_Sphere = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
            allocator,
            device,
            flag,
            IZ_COLOR_RGBA(0xff, 0, 0, 0xff),
            1.0f,
            10, 10);

        m_Axis = izanagi::CDebugMeshAxis::CreateDebugMeshAxisDefault(
            allocator,
            device);

        m_Plane = izanagi::CDebugMeshRectangle::CreateDebugMeshRectangle(
            allocator,
            device,
            flag,
            0xffffffff,
            1, 1, 100.0f, 100.0f);
    }

    // カメラ
    camera.Init(
        izanagi::math::CVector(0.0f, 10.0f, 0.0f, 1.0f),
        izanagi::math::CVector(10.0f, 10.0f, 0.0f, 1.0f),
        izanagi::math::CVector(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        500.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)SCREEN_WIDTH / SCREEN_HEIGHT);
    camera.Update();

    SUniversalTime day;
    {
        day.year = 1976;
        day.month = 7;
        day.day = 27;
        day.hour = 12;
        day.minute = 0;
        day.second = 0;
    }

    SLongTime jd;
    CTime::GetJDByUT(day, jd);

    SPolarCoord p;
    {
        p.radius = 1.0f;
        p.latitude = -4.070f;
        //p.latitude = 0.0f;
        p.longitude = 281.0f;
        //p.longitude = IZ_DEG2RAD(90.0f);

        //p.latitude = IZ_DEG2RAD(45.0f);
        //p.longitude = IZ_DEG2RAD(45.0f);
    }

    izanagi::math::SMatrix mtx;
    CEphemeris::ConvertPolarToMatrix(p, mtx);

    izanagi::math::SVector v0;
    v0.x = 1.0f;
    v0.y = 0.0f;
    v0.z = 0.0f;
    v0.w = 1.0f;
    izanagi::math::SMatrix::ApplyXYZ(v0, v0, mtx);

    izanagi::math::SVector v;
    CEphemeris::ConvertPolarToRectangular(p, v);
    CEphemeris::ConvertRectangularToPolar(v, p);

    IZ_FLOAT lat = p.latitude;
    IZ_FLOAT log = p.longitude;

    SPolarCoord eliptic;
    {
        eliptic.radius = 1.0f;
        eliptic.latitude = 18.927f;
        eliptic.longitude = 281.608f;

        //eliptic.latitude = IZ_DEG2RAD(90.0f);
        //eliptic.longitude = IZ_DEG2RAD(0.0f);
    }

    izanagi::math::SVector equatorial;
    CEphemeris::ConvertElipticToEquatorial(eliptic, equatorial);

    SPolarCoord polar;
    CEphemeris::ConvertRectangularToPolar(equatorial, polar);

    lat = polar.latitude;
    log = polar.longitude;

    SUniversalTime ut;
    {
        ut.year = 2012;
        ut.month = 5;
        ut.day = 31;
        ut.hour = 2;
        ut.minute = 57;
        ut.second = 31;
    }
    SUniversalTime lst;
    CTime::GetLSTByUT(ut, 135.0f, lst);

    {
        ut.year = 2013;
        ut.month = 3;
        ut.day = 28;
        ut.hour = 14;
        ut.minute = 43;
        ut.second = 0;
    }
    CEphemeris::GetHourAngle(ut, 135.0f, 37.5);

__EXIT__:
    if (!result) {
        ReleaseInternal();
    }

    return IZ_TRUE;
}

// 解放.
void CSunApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Shader);
    SAFE_RELEASE(m_Sphere);
    SAFE_RELEASE(m_Axis);
    SAFE_RELEASE(m_Plane);
}

IZ_FLOAT angle = 0.0f;
IZ_FLOAT radius = 100.0f;

izanagi::math::SMatrix g_Eliptic;
izanagi::math::SMatrix g_Horizon;

// 更新.
void CSunApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::CCamera& camera = GetCamera();
    camera.Update();

    SPolarCoord polarEliptic;

    SUniversalTime day;
    {
        day.year = 2013;
        day.month = 7;
        day.day = 18;
        day.hour = 12;
        day.minute = 0;
        day.second = 0;
    }

    izanagi::math::SMatrix trans;
    izanagi::math::SMatrix::GetTrans(
        trans,
        radius, 0.0f, 0.0f);

    // Sun
    {
        SUniversalTime veDay;
        {
            veDay.year = 2013;
            veDay.month = 3;
            veDay.day = 21;
            veDay.hour = 12;
            veDay.minute = 0;
            veDay.second = 0;
        }
        IZ_FLOAT longitude = CSun::GetElipticLongitudeByUniversalTime(day, veDay);

        {
            polarEliptic.radius = 1.0f;
            polarEliptic.latitude = 0.0f;
            polarEliptic.longitude = longitude;
        }

        izanagi::math::SVector eq;
        CEphemeris::ConvertElipticToEquatorial(polarEliptic, eq);

        SPolarCoord polar;
        CEphemeris::ConvertRectangularToPolar(eq, polar);

        izanagi::math::SVector v;
        CEphemeris::ConvertPolarToRectangular(polar, v);

        //IZ_PRINTF("%f\n", v.y);

        izanagi::math::SMatrix rot;
        CEphemeris::ConvertPolarToMatrix(polar, rot);

        izanagi::math::SMatrix::Mul(g_Eliptic, trans, rot);
    }

    // Horizon
    {
        SPolarCoord polar;
        {
            polar.longitude = IZ_DEG2RAD(135);
            polar.latitude = IZ_DEG2RAD(35);
        }

        izanagi::math::SVector horizontal;

        CEphemeris::ConvertElipticToHorizontal(
            polarEliptic,
            day,
            polar,
            horizontal);

        CEphemeris::ConvertRectangularToPolar(horizontal, polar);

        izanagi::math::SMatrix rot;
        CEphemeris::ConvertPolarToMatrix(polar, rot);

        izanagi::math::SMatrix::Mul(g_Horizon, trans, rot);
    }
}

namespace {
    inline void _SetShaderParam(
        izanagi::CShaderBasic* shader,
        const char* name,
        const void* value,
        IZ_UINT bytes)
    {
        izanagi::IZ_SHADER_HANDLE handle = shader->GetParameterByName(name);
        IZ_ASSERT(handle != 0);

        shader->SetParamValue(
            handle,
            value,
            bytes);
    }
}

// 描画.
void CSunApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::sample::CSampleCamera& camera = GetCamera();

    m_Shader->Begin(0, IZ_FALSE);

    if (m_Shader->BeginPass(0))
    {
        // パラメータ設定
        _SetShaderParam(
            m_Shader,
            "g_mL2W",
            (void*)&g_Eliptic,
            sizeof(g_Eliptic));

        _SetShaderParam(
            m_Shader,
            "g_mW2C",
            (void*)&camera.GetParam().mtxW2C,
            sizeof(camera.GetParam().mtxW2C));

        // シェーダ設定
        m_Shader->CommitChanges();

        m_Sphere->Draw();

        _SetShaderParam(
            m_Shader,
            "g_mL2W",
            (void*)&izanagi::math::SMatrix::GetUnit(),
            sizeof(izanagi::math::SMatrix));

        m_Shader->CommitChanges();

        m_Plane->Draw();

        _SetShaderParam(
            m_Shader,
            "g_mL2W",
            (void*)&izanagi::math::SMatrix::GetUnit(),
            sizeof(izanagi::math::SMatrix));

        m_Shader->CommitChanges();

        m_Axis->Draw();

        m_Shader->EndPass();
    }

    m_Shader->End();
}
