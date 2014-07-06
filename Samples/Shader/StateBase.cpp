#include "StateBase.h"
#include "izSampleKit.h"
#include "StateManager.h"

CStateBase::CStateBase(
    izanagi::sample::CSampleApp* app,
    izanagi::SCameraParam& camera)
: m_App(app), 
  m_Camera(camera)
{
}

// ‰Šú‰».
IZ_BOOL CStateBase::Init()
{
    // Nothing is done...
    return IZ_TRUE;
}

// XV
IZ_BOOL CStateBase::Update(
    IZ_FLOAT time,
    izanagi::graph::CGraphicsDevice* device)
{
    return IZ_TRUE;
}


// I—¹.
IZ_BOOL CStateBase::Destroy()
{
    return Leave();
}

IZ_BOOL CStateBase::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    State cur = CStateManager::GetInstance().GetCurrentState();

    if (key == izanagi::sys::E_KEYBOARD_BUTTON_UP) {
        CStateManager::GetInstance().ChangeState(cur + 1);
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_DOWN) {
        CStateManager::GetInstance().ChangeState(cur - 1);
    }

    return IZ_TRUE;
}

void CStateBase::RenderName(
    izanagi::graph::CGraphicsDevice* device,
    const char* name)
{
    izanagi::CDebugFont* debugFont = m_App->GetDebugFont();

    if (device->Begin2D()) {
        debugFont->Begin(device, 0, izanagi::CDebugFont::FONT_SIZE * 2);

        debugFont->DBPrint(
            device,
            "%s\n",
            name);

        debugFont->End();

        device->End2D();
    }
}

void CStateBase::SetShaderParam(
    izanagi::shader::CShaderBasic* shader,
    const char* name,
    const void* value,
    IZ_UINT bytes)
{
    izanagi::shader::IZ_SHADER_HANDLE handle = shader->GetParameterByName(name);
    if (handle > 0) {
        shader->SetParamValue(
            handle,
            value,
            bytes);
    }
    else
    {
        IZ_PRINTF("Can not find shader parameter [%s]\n", name);
    }
}
