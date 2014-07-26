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
    Environment();
    ~Environment();

public:
    /** Set point light parameter to shader.
     */
    void SetPointLightParam(izanagi::shader::CShaderBasic* shader);
    
    /** Set parallel light parameter to shader.
     */
    void SetParallelLightParam(
        const izanagi::CCamera& camera,
        const izanagi::math::SMatrix& mtxL2W,
        izanagi::shader::CShaderBasic* shader);

    /** Set ambient light parameter to shader.
     */
    void SetAmbientLightParam(izanagi::shader::CShaderBasic* shader);

    /** Get parallel light direction.
     */
    izanagi::math::SVector4& GetDir()
    {
        return m_ParallelLight.vDir;
    }

private:
    izanagi::SPointLightParam m_PointLight;
    izanagi::SAmbientLightParam m_Ambient;
    izanagi::SParallelLightParam m_ParallelLight;
};

#endif    // #if !defined(__ENVIRONMENT_H__)