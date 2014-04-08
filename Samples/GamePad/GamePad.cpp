#include "izSampleKit.h"

#include <dinput.h>
#include <XInput.h>

class CGamePadApp : public izanagi::sample::CSampleApp {
public:
    CGamePadApp();
    virtual ~CGamePadApp();

protected:
    // 初期化.
    virtual IZ_BOOL InitInternal(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::sample::CSampleCamera& camera);

    // 解放.
    virtual void ReleaseInternal();

    // 更新.
    virtual void UpdateInternal(izanagi::graph::CGraphicsDevice* device);

    // 描画.
    virtual void RenderInternal(izanagi::graph::CGraphicsDevice* device);
};

CGamePadApp::CGamePadApp()
{
}

CGamePadApp::~CGamePadApp()
{
}

// 初期化.
IZ_BOOL CGamePadApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    return IZ_TRUE;
}

// 解放.
void CGamePadApp::ReleaseInternal()
{
}

// 更新.
void CGamePadApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::sys::CPad* pad = GetPad();

    if (pad != IZ_NULL)
    {
        pad->Update();
    }
}

namespace
{
    void DumpDInputPad(
        izanagi::graph::CGraphicsDevice* device,
        izanagi::CDebugFont* font,
        izanagi::sys::CPad* pad)
    {
        font->DBPrint(device, "DirectInput ----\n");

        const DIJOYSTATE& raw = *(reinterpret_cast<const DIJOYSTATE*>(pad->GetRawState()));
        
        font->DBPrint(device, "AxisPos [%d][%d][%d]\n", raw.lX, raw.lY, raw.lZ);
        font->DBPrint(device, "AxisRot [%d][%d][%d]\n", raw.lRx, raw.lRy, raw.lRz);
        font->DBPrint(device, "Slider  [%d][%d]\n", raw.rglSlider[0], raw.rglSlider[1]);
        font->DBPrint(
            device, 
            "Pov     [%d][%d][%d][%d]\n", 
            raw.rgdwPOV[0], raw.rgdwPOV[1], raw.rgdwPOV[2], raw.rgdwPOV[3]);

        for (IZ_UINT i = 0; i < 32; i += 2)
        {
            font->DBPrint(
                device, 
                "Button  %d[%d] %d[%d]\n",
                i, raw.rgbButtons[i],
                i + 1, raw.rgbButtons[i + 1]);
        }
    }

    void DumpXInputPad(
        izanagi::graph::CGraphicsDevice* device,
        izanagi::CDebugFont* font,
        izanagi::sys::CPad* pad)
    {
        font->DBPrint(device, "XInput ----\n");

        const XINPUT_STATE& raw = *(reinterpret_cast<const XINPUT_STATE*>(pad->GetRawState()));
        
        font->DBPrint(device, "Button [%x]\n", raw.Gamepad.wButtons);

        font->DBPrint(
            device,
            "Trigger L[%d] R[%d]\n",
            raw.Gamepad.bLeftTrigger,
            raw.Gamepad.bRightTrigger);

        font->DBPrint(
            device,
            "TumpL X[%d] Y[%d]\n",
            raw.Gamepad.sThumbLX,
            raw.Gamepad.sThumbLY);

        font->DBPrint(
            device,
            "TumpR X[%d] Y[%d]\n",
            raw.Gamepad.sThumbRX,
            raw.Gamepad.sThumbRY);
    }
}

// 描画.
void CGamePadApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::sys::CPad* pad = GetPad();
    izanagi::CDebugFont* font = GetDebugFont();

    if (device->Begin2D())
    {
        font->Begin(device, 0, 100);
        {
            if (pad != IZ_NULL)
            {
                font->DBPrint(device, "raw ----\n");

                if (pad->GetType() == izanagi::sys::E_PAD_TYPE_X_INPUT)
                {
                    DumpXInputPad(device, font, pad);
                }
                else
                {
                    DumpDInputPad(device, font, pad);
                }

                font->DBPrint(device, "\n");
                font->DBPrint(device, "izanagi ----\n");

                const izanagi::sys::PadState& state = pad->GetCurState();

                font->DBPrint(device, "AxisX x[%f] y[%f]\n", state.axisLeft[0], state.axisLeft[1]);
                font->DBPrint(device,"AxisY x[%f] y[%f]\n", state.axisRight[0], state.axisRight[1]);

                font->DBPrint(
                    device,
                    "Trigger L[%f] R[%f]\n",
                    state.triggerLeft,
                    state.triggerRight);

                const char* names[] =
                {
                    "R_DOWN", "R_RIGHT", "R_LEFT", "R_UP",
                    "L1", "R1",
                    "SELECT", "START",
                    "L3", "R3",
                    "L2", "R2",
                    "L_UP", "L_RIGHT", "L_DOWN", "L_LEFT",
                };
                IZ_C_ASSERT(COUNTOF(names) == izanagi::sys::E_PAD_BUTTON_NUM);

                for (IZ_UINT i = 0; i < izanagi::sys::E_PAD_BUTTON_NUM;)
                {
                    font->DBPrint(device, "%s [%d] ", names[i], state.buttons[i]);
                    i++;

                    if (i < izanagi::sys::E_PAD_BUTTON_NUM)
                    {
                        font->DBPrint(device, "%s [%d]\n", names[i], state.buttons[i]);
                        i++;
                    }
                }
            }
            else
            {
                font->DBPrint(device, "No GamePad");
            }
        }
        font->End();

        device->End2D();
    }
}

static const IZ_UINT BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

CGamePadApp app;

IzMain(SCREEN_WIDTH, SCREEN_HEIGHT)
{
    int ret = SampleMain(
        IzGetSystemDataForMainFunc(),
        &app,
        "GamePad",
        IzGetScreenWidth(), IzGetScreenHeight(),
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
