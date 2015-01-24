#if !defined(__ENVIRONMENT_H__)
#define __ENVIRONMENT_H__

#include "izSampleKit.h"

/** Manage environment parameters.
 */
class Environment {
private:
    static Environment s_Instance;

public:
    static Environment& Instance();

private:
    Environment() {}
    ~Environment() {}

public:
    void SetGlobalLightParam(
        const izanagi::CCamera& camera,
        izanagi::shader::CShaderBasic* shader);

    void SetLocalLightParam(
        const izanagi::CCamera& camera,
        const izanagi::math::SMatrix44& mtxL2W,
        izanagi::shader::CShaderBasic* shader);

private:
    izanagi::SParallelLightParam m_ParallelLight;
};

#endif    // #if !defined(__ENVIRONMENT_H__)