#include "SampleWindowProc.h"
#include "SampleApp.h"

using namespace izanagi;
using namespace sample;

CSampleWndProc::CSampleWndProc() 
{
    m_App = IZ_NULL;;

    m_Flags.onLBtn = IZ_FALSE;
    m_Flags.onRBtn = IZ_FALSE;

    funcInit = IZ_NULL;
    funcDestroy = IZ_NULL;
}

CSampleWndProc::~CSampleWndProc()
{
}

void CSampleWndProc::SetApp(CSampleApp* app)
{
    m_App = app;
}

void CSampleWndProc::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    m_App->OnKeyDown(key);
}

void CSampleWndProc::OnKeyUp(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    m_App->OnKeyUp(key);
}

void CSampleWndProc::OnMouseLBtnDown(const izanagi::CIntPoint& point)
{
    m_Flags.onLBtn = IZ_TRUE;
    m_PrevPoint = point;

    m_App->OnMouseLBtnDown(point);
}

void CSampleWndProc::OnMouseLBtnUp(const izanagi::CIntPoint& point)
{
    m_Flags.onLBtn = IZ_FALSE;

    m_App->OnMouseLBtnUp(point);
}

void CSampleWndProc::OnMouseRBtnDown(const izanagi::CIntPoint& point)
{
    m_Flags.onRBtn = IZ_TRUE;
    m_PrevPoint = point;

    m_App->OnMouseRBtnDown(point);
}

void CSampleWndProc::OnMouseRBtnUp(const izanagi::CIntPoint& point)
{
    m_Flags.onRBtn = IZ_FALSE;

    m_App->OnMouseRBtnUp(point);
}

void CSampleWndProc::OnMouseMove(const izanagi::CIntPoint& point)
{
    IZ_ASSERT(m_App != IZ_NULL);
    m_App->OnMouseMove(point);
}

void CSampleWndProc::OnMouseWheel(IZ_INT delta)
{
    m_App->GetCamera().Dolly(delta * 0.1f);
}

void CSampleWndProc::OnPaint()
{
    IZ_ASSERT(m_App != IZ_NULL);

    m_App->Render();
    m_App->Present(nullptr);
}

void CSampleWndProc::OnIdle(void* nativeParam)
{
    IZ_ASSERT(m_App != IZ_NULL);

#if 0
    m_App->GetTimer(0).Begin();
    m_App->GetTimer(1).Begin();

    m_App->Update();
    m_App->Render();

    m_App->GetTimer(1).End();

    m_App->Present();

    m_App->GetTimer(0).End();
#else
    m_App->Idle(nativeParam);
#endif
}

void CSampleWndProc::OnInit(const izanagi::sys::WindowHandle& handle)
{
    if (funcInit) {
        (*funcInit)(handle, m_App);
    }
}

void CSampleWndProc::OnTerminate()
{
    m_App->Release();
}

void CSampleWndProc::OnDestroy()
{
    if (funcDestroy) {
        (*funcDestroy)();
    }
}
