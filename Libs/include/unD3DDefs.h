#if !defined(__URANUS_D3D_DEFS_H__)
#define __URANUS_D3D_DEFS_H__

#include <d3d9.h>
#include <d3dx9.h>
//#include <dxerr9.h>

#include <d3dx9shader.h>

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800 
#endif

#include <dinput.h>

#define D3D_INST		IDirect3D9
#define D3D_DEVICE		IDirect3DDevice9
#define D3D_TEXTURE		IDirect3DTexture9
#define D3D_CUBETEXTURE	IDirect3DCubeTexture9
#define D3D_SURFACE		IDirect3DSurface9
#define D3D_VB			IDirect3DVertexBuffer9
#define D3D_IB			IDirect3DIndexBuffer9

#define D3D_EFFECT		ID3DXEffect
#define D3D_CONSTTABLE	ID3DXConstantTable 
#define D3D_VS			IDirect3DVertexShader9
#define D3D_PS			IDirect3DPixelShader9
#define D3D_VD			IDirect3DVertexDeclaration9

#define D3D_VTX_ELEMENT	D3DVERTEXELEMENT9

#define	D3D_VIEWPORT	D3DVIEWPORT9

#define D_INPUT			IDirectInput8
#define D_INPUT_DEVICE	IDirectInputDevice8

// TODO
typedef IDirect3DBaseTexture9* TEX_HANDLE;

// シェーダ定数ハンドル
typedef D3DXHANDLE	SHADER_PARAM_HANDLE;

#endif	// #if !defined(__URANUS_D3D_DEFS_H__)
