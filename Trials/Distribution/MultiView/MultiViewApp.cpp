#include "MultiViewApp.h"

MultiViewApp::MultiViewApp()
{
}

MultiViewApp::~MultiViewApp()
{
}

// 初期化.
IZ_BOOL MultiViewApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    const GLubyte* extensions = glGetString(GL_EXTENSIONS);
    const char * found_extension = strstr((const char*)extensions, "GL_OVR_multiview");
    result = (NULL != found_extension);
    if (!result)
    {
        IZ_PRINTF("OpenGL ES 3.0 implementation does not support GL_OVR_multiview extension.\n");
        exit(EXIT_FAILURE);
    }

    // カメラ
    camera.Init(
        izanagi::math::CVector4(0.0f, 0.0f, 30.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 0.0f, 0.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        500.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)device->GetBackBufferWidth() / device->GetBackBufferHeight());
    camera.Update();

__EXIT__:
    if (!result) {
        ReleaseInternal();
    }

    return IZ_TRUE;
}

// 解放.
void MultiViewApp::ReleaseInternal()
{
}

// 更新.
void MultiViewApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    auto& camera = GetCamera();
    
    camera.Update();
}

namespace {
    inline void _SetShaderParam(
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
}

// 描画.
void MultiViewApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
}

IZ_BOOL MultiViewApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    return IZ_TRUE;
}
