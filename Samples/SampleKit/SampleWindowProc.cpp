#include "SampleWindowProc.h"
#include "SampleApp.h"

using namespace izanagi;
using namespace sample;

CSampleWndProc::CSampleWndProc(CSampleApp* app) 
{
    m_App = app;

    m_Flags.onLBtn = IZ_FALSE;
    m_Flags.onRBtn = IZ_FALSE;

    funcInit = IZ_NULL;
    funcTerminate = IZ_NULL;
    funcDestroy = IZ_NULL;
}

CSampleWndProc::~CSampleWndProc()
{
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
}

void CSampleWndProc::OnMouseRBtnDown(const izanagi::CIntPoint& point)
{
    m_Flags.onRBtn = IZ_TRUE;
    m_PrevPoint = point;
}

void CSampleWndProc::OnMouseRBtnUp(const izanagi::CIntPoint& point)
{
    m_Flags.onRBtn = IZ_FALSE;
}

namespace {
    inline IZ_FLOAT _NormalizeHorizontal(
        CSampleApp* app,
        IZ_UINT x)
    {
        IZ_UINT width = app->GetScreenWidth();
        IZ_FLOAT ret = (2.0f * x - width) / (IZ_FLOAT)width;
        return ret;
    }

    inline IZ_FLOAT _NormalizeVertical(
        CSampleApp* app,
        IZ_UINT y)
    {
        IZ_UINT height = app->GetScreenHeight();
        IZ_FLOAT ret = (height - 2.0f * y) / (IZ_FLOAT)height;
        return ret;
    }
}

void CSampleWndProc::OnMouseMove(const izanagi::CIntPoint& point)
{
    IZ_ASSERT(m_App != IZ_NULL);

    if (m_Flags.onLBtn) {
        m_App->GetCamera().Rotate(
            izanagi::CFloatPoint(
                _NormalizeHorizontal(m_App, m_PrevPoint.x),
                _NormalizeVertical(m_App, m_PrevPoint.y)),
            izanagi::CFloatPoint(
                _NormalizeHorizontal(m_App, point.x),
                _NormalizeVertical(m_App, point.y))
        );
    }
    else if (m_Flags.onRBtn) {
        float fOffsetX = (float)(m_PrevPoint.x - point.x);
        fOffsetX *= 0.5f;

        float fOffsetY = (float)(m_PrevPoint.y - point.y);
        fOffsetY *= 0.5f;

        m_App->GetCamera().Move(fOffsetX, fOffsetY);
    }

    m_PrevPoint = point;
}

void CSampleWndProc::OnMouseWheel(IZ_INT delta)
{
    m_App->GetCamera().Dolly(delta * 0.1f);
}

void CSampleWndProc::OnPaint()
{
    IZ_ASSERT(m_App != IZ_NULL);

    m_App->Render();
    m_App->Present();
}

void CSampleWndProc::OnIdle()
{
    IZ_ASSERT(m_App != IZ_NULL);

    m_App->GetTimer(0).Begin();
    m_App->GetTimer(1).Begin();

    m_App->Update();
    m_App->Render();

    m_App->GetTimer(1).End();

    m_App->Present();

    m_App->GetTimer(0).End();
}

void CSampleWndProc::OnInit(const izanagi::sys::WindowHandle& handle)
{
    if (funcInit) {
        (*funcInit)(handle, m_App);
    }
}

void CSampleWndProc::OnTerminate()
{
    if (funcTerminate) {
        (*funcTerminate)(m_App);
    }
}

void CSampleWndProc::OnDestroy()
{
    if (funcDestroy) {
        (*funcDestroy)();
    }
}
