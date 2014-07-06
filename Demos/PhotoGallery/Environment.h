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

private:
    izanagi::SPointLightParam m_PointLight;
};

#endif    // #if !defined(__ENVIRONMENT_H__)