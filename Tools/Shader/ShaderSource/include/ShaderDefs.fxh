#if !defined(__IZANAGI_SHADER_SHADER_DEFS_FXH__)
#define __IZANAGI_SHADER_SHADER_DEFS_FXH__

///////////////////////////////////////////////
// For Sampler

#define BindTex(tex)			Texture = <tex>		// 対応するテクスチャ
#define SetAddressU(addr)		AddressU = addr		// AddressU
#define SetAddressV(addr)		AddressV = addr		// AddressV
#define SetMinFilter(filter)	MinFilter = filter	// MinFilter
#define SetMagFilter(filter)	MagFilter = filter	// MagFilter

// Address
#define SMPL_ADDRESS_WRAP		WRAP	// 0
#define SMPL_ADDRESS_MIRROR		MIRROR	// 1
#define SMPL_ADDRESS_CLAMP		CLAMP	// 2

// MinFilter・MagFilter
#define SMPL_FILTER_NONE		NONE	// 0
#define SMPL_FILTER_POINT		POINT	// 1
#define SMPL_FILTER_LINEAR		LINEAR	// 2

#endif	// #if !defined(__IZANAGI_SHADER_SHADER_DEFS_FXH__)
