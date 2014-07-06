#if !defined(__ENVIRONMENT_H__)
#define __ENVIRONMENT_H__

#include "izSampleKit.h"

class Environment {
private:
    static Environment s_Instance;

public:
    static Environment& Instance();

private:
    Environment();
    ~Environment();

public:
    void SetPointLightParam(izanagi::shader::CShaderBasic* shader);
    
    void SetParallelLightParam(
        const izanagi::CCamera& camera,
        const izanagi::math::SMatrix& mtxL2W,
        izanagi::shader::CShaderBasic* shader);

    void SetAmbientLightParam(izanagi::shader::CShaderBasic* shader);

    izanagi::math::SVector& GetDir()
    {
        return m_ParallelLight.vDir;
    }

private:
    izanagi::SPointLightParam m_PointLight;
    izanagi::SAmbientLightParam m_Ambient;
    izanagi::SParallelLightParam m_ParallelLight;
};

#endif    // #if !defined(__ENVIRONMENT_H__)