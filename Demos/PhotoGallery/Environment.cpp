#include "Environment.h"
#include "Utility.h"
#include "Configure.h"

Environment Environment::s_Instance;

Environment& Environment::Instance()
{
    return s_Instance;
}

Environment::Environment()
{
    m_PointLight.vPos.Set(0.0f, 40.0f, -Configure::InnerRadius * 0.5f);
    m_PointLight.color.Set(1.0f, 1.0f, 1.0f, 1.0f);
    m_PointLight.attn.Set(0.0f, 0.0f, 0.0005f, 0.0f);
}

Environment::~Environment()
{
}

void Environment::SetPointLightParam(izanagi::shader::CShaderBasic* shader)
{
    Utility::SetShaderParam(
        shader,
        "g_PointLightPos",
        (void*)&m_PointLight.vPos,
        sizeof(m_PointLight.vPos));
    Utility::SetShaderParam(
        shader,
        "g_PointLight",
        (void*)&m_PointLight.attn,
        sizeof(m_PointLight.attn));
    Utility::SetShaderParam(
        shader,
        "g_PointLightColor",
        (void*)&m_PointLight.color,
        sizeof(m_PointLight.color));
}
