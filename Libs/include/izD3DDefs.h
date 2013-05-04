#if !defined(__IZANAGI_D3D_DEFS_H__)
#define __IZANAGI_D3D_DEFS_H__

#include <d3d9.h>
#include <d3dx9.h>
//#include <dxerr9.h>

#include <d3dx9shader.h>

#define D3D_INST        IDirect3D9
#define D3D_DEVICE      IDirect3DDevice9
#define D3D_TEXTURE     IDirect3DTexture9
#define D3D_CUBETEXTURE IDirect3DCubeTexture9
#define D3D_SURFACE     IDirect3DSurface9
#define D3D_VB          IDirect3DVertexBuffer9
#define D3D_IB          IDirect3DIndexBuffer9

#define D3D_EFFECT      ID3DXEffect
#define D3D_CONSTTABLE  ID3DXConstantTable 
#define D3D_VS          IDirect3DVertexShader9
#define D3D_PS          IDirect3DPixelShader9
#define D3D_VD          IDirect3DVertexDeclaration9

#define D3D_VTX_ELEMENT D3DVERTEXELEMENT9

#define D3D_VIEWPORT    D3DVIEWPORT9

// TODO
typedef IDirect3DBaseTexture9* TEX_HANDLE_DX9;

// シェーダ定数ハンドル
typedef D3DXHANDLE  SHADER_PARAM_HANDLE_DX9;

namespace izanagi
{
namespace graph
{
    /** 初期化パラメータ
     */
    struct SGraphicsDeviceInitParams {
        HWND hFocusWindow;
        HWND hDeviceWindow;

        DWORD BackBufferWidth;
        DWORD BackBufferHeight;
        BOOL Windowed;
        D3DMULTISAMPLE_TYPE MultiSampleType;

        DWORD Adapter;
        D3DDEVTYPE DeviceType;
        DWORD BehaviorFlags;

        D3DFORMAT DepthStencilFormat;

        DWORD PresentationInterval;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_D3D_DEFS_H__)
