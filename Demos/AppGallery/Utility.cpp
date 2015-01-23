#include "Utility.h"

void Utility::SetShaderParam(
    izanagi::shader::CShaderBasic* shader,
    const char* name,
    const void* value,
    IZ_UINT bytes)
{
    izanagi::shader::IZ_SHADER_HANDLE handle = shader->GetParameterByName(name);
    IZ_ASSERT(handle != 0);

    shader->SetParamValue(
        handle,
        value,
        bytes);
}
