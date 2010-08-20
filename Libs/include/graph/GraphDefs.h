#if !defined(__URANUS_GRAPH_GRAPH_DEFS_H__)
#define __URANUS_GRAPH_GRAPH_DEFS_H__

#include "unDefs.h"
#include "unStd.h"

namespace uranus {
	/**
	* �����_�[�X�e�[�g
	*/
	enum E_GRAPH_RENDER_STATE {
		E_GRAPH_RS_ZWRITEENABLE = 0,
		E_GRAPH_RS_ZENABLE,
		E_GRAPH_RS_ZFUNC,

		E_GRAPH_RS_ALPHATESTENABLE,
		E_GRAPH_RS_ALPHAREF,
		E_GRAPH_RS_ALPHAFUNC,
		E_GRAPH_RS_ALPHABLENDENABLE,
		E_GRAPH_RS_BLENDMETHOD,
		
		E_GRAPH_RS_FILLMODE,
		E_GRAPH_RS_CULLMODE,
		
		E_GRAPH_RS_COLORWRITEENABLE_RGB,
		E_GRAPH_RS_COLORWRITEENABLE_A,

		E_GRAPH_RS_SCISSORTESTENABLE,

		E_GRAPH_RS_NUM,
		E_GRAPH_RS_FORCE_INT32 = 0x7fffffff,
	};

	/**
	* �t�B�����[�h
	*/
	enum E_GRAPH_FILL_MODE {
		E_GRAPH_FILL_MODE_POINT = 0,
		E_GRAPH_FILL_MODE_WIREFRAME,
		E_GRAPH_FILL_MODE_SOLID,

		E_GRAPH_FILL_MODE_NUM,
		E_GRAPH_FILL_MODE_FORCE_INT32 = 0x7fffffff,
	};

	/**
	* �u�����h���@
	*/
	enum E_GRAPH_BLEND_OP {
		E_GRAPH_BLEND_OP_ADD = 0,
		E_GRAPH_BLEND_OP_SUBTRACT,
		E_GRAPH_BLEND_OP_REVSUBTRACT,
		E_GRAPH_BLEND_OP_MIN,
		E_GRAPH_BLEND_OP_MAX,

		E_GRAPH_BLEND_OP_NUM,
		E_GRAPH_BLEND_OP_FORCE_INT32 = 0x7fffffff,
	};

	/**
	* �u�����h�W��
	*/
	enum E_GRAPH_BLEND {
		E_GRAPH_BLEND_ZERO = 0,
		E_GRAPH_BLEND_ONE,
		E_GRAPH_BLEND_SRCCOLOR,
		E_GRAPH_BLEND_INVSRCCOLOR,
		E_GRAPH_BLEND_SRCALPHA,
		E_GRAPH_BLEND_INVSRCALPHA,
		E_GRAPH_BLEND_DESTALPHA,
		E_GRAPH_BLEND_INVDESTALPHA,
		E_GRAPH_BLEND_DESTCOLOR,
		E_GRAPH_BLEND_INVDESTCOLOR,
		E_GRAPH_BLEND_SRCALPHASAT,
		E_GRAPH_BLEND_BOTHSRCALPHA,
		E_GRAPH_BLEND_BOTHINVSRCALPHA,

		E_GRAPH_BLEND_NUM,
		E_GRAPH_BLEND_FORCE_INT32 = 0x7fffffff,
	};

	/**
	* �J�����O���[�h
	*/
	enum E_GRAPH_CULL {
		E_GRAPH_CULL_NONE = 0,
		E_GRAPH_CULL_CW,
		E_GRAPH_CULL_CCW,

		E_GRAPH_CULL_NUM,
		E_GRAPH_CULL_FORCE_INT32,
	};

	/**
	* ��r����
	*/
	enum E_GRAPH_CMP_FUNC {
		E_GRAPH_CMP_FUNC_NEVER = 0,
		E_GRAPH_CMP_FUNC_LESS,
		E_GRAPH_CMP_FUNC_EQUAL,
		E_GRAPH_CMP_FUNC_LESSEQUAL,
		E_GRAPH_CMP_FUNC_GREATER,
		E_GRAPH_CMP_FUNC_NOTEQUAL,
		E_GRAPH_CMP_FUNC_GREATEREQUAL,
		E_GRAPH_CMP_FUNC_ALWAYS,

		E_GRAPH_CMP_FUNC_NUM,
		E_GRAPH_CMP_FUNC_FORCE_INT32 = 0x7ffffff,
	};

	/**
	* �e�N�X�`���A�h���b�V���O
	*/
	enum E_GRAPH_TEX_ADDRESS {
		E_GRAPH_TEX_ADDRESS_WRAP = 0,
		E_GRAPH_TEX_ADDRESS_MIRROR,
		E_GRAPH_TEX_ADDRESS_CLAMP,
		E_GRAPH_TEX_ADDRESS_BORDER,
		E_GRAPH_TEX_ADDRESS_MIRRORONCE,

		E_GRAPH_TEX_ADDRESS_NUM,
		E_GRAPH_TEX_ADDRESS_FORCE_INT32 = 0x7ffffff,
	};

	/**
	* �e�N�X�`���t�B���^
	*/
	enum E_GRAPH_TEX_FILTER {
		E_GRAPH_TEX_FILTER_NONE = 0,		// filtering disabled (valid for mip filter only)
		E_GRAPH_TEX_FILTER_POINT,			// nearest
		E_GRAPH_TEX_FILTER_LINEAR,			// linear interpolation
		E_GRAPH_TEX_FILTER_ANISOTROPIC,		// anisotropic
		E_GRAPH_TEX_FILTER_PYRAMIDALQUAD,	// 4-sample tent
		E_GRAPH_TEX_FILTER_GAUSSIANQUAD,	// 4-sample gaussian

		E_GRAPH_TEX_FILTER_NUM,
		E_GRAPH_TEX_FILTER_FORCE_INT32 = 0x7fffffff,
	};

	/**
	* �T���v���[�X�e�[�g�^�C�v
	*/
	enum E_GRAPH_SAMPLER_STATE_TYPE {
		E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSU = 0,
		E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSV,
		E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSW,

		E_GRAPH_SAMPLER_STATE_TYPE_MAGFILTER,
		E_GRAPH_SAMPLER_STATE_TYPE_MINFILTER,
		E_GRAPH_SAMPLER_STATE_TYPE_MIPFILTER,

		E_GRAPH_SAMPLER_STATE_TYPE_NUM,
		E_GRAPH_SAMPLER_STATE_TYPE_FORCE_INT32 = 0x7fffffff,
	};

	inline UN_BOOL IsSamplerStateTypeAddr(E_GRAPH_SAMPLER_STATE_TYPE n)
	{
		return ((E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSU <= n) && (n <= E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSW));
	}
	inline UN_BOOL IsSamplerStateTypeFilter(E_GRAPH_SAMPLER_STATE_TYPE n)
	{
		return ((E_GRAPH_SAMPLER_STATE_TYPE_MAGFILTER <= n) && (n <= E_GRAPH_SAMPLER_STATE_TYPE_MIPFILTER));
	}

	/**
	* �A���t�@�u�����h����
	*/
	#define UN_GRAPH_ALPHA_BLEND_VAL(op, src, dst)	(((op & 0xff) << 16) | ((src & 0xff) << 8) | (dst & 0xff))
	#define UN_GRAPH_GET_ALPHA_BLEND_OP(val)		((val >> 16) & 0xff);
	#define UN_GRAPH_GET_ALPHA_BLEND_SRC(val)		((val >> 8) & 0xff);
	#define UN_GRAPH_GET_ALPHA_BLEND_DST(val)		(val & 0xff)
	enum E_GRAPH_ALPHA_BLEND {
		// �ʏ�̃A���t�@�u�����h
		E_GRAPH_ALPHA_BLEND_NORMAL   = UN_GRAPH_ALPHA_BLEND_VAL(E_GRAPH_BLEND_OP_ADD,         E_GRAPH_BLEND_SRCALPHA, E_GRAPH_BLEND_INVSRCALPHA),

		// ���Z
		E_GRAPH_ALPHA_BLEND_CsAs_Cd  = UN_GRAPH_ALPHA_BLEND_VAL(E_GRAPH_BLEND_OP_ADD,         E_GRAPH_BLEND_SRCALPHA, E_GRAPH_BLEND_ONE),

		E_GRAPH_ALPHA_BLEND_Cs_Cd    = UN_GRAPH_ALPHA_BLEND_VAL(E_GRAPH_BLEND_OP_ADD,         E_GRAPH_BLEND_ONE,      E_GRAPH_BLEND_ONE),
		E_GRAPH_ALPHA_BLEND_NCsAs_Cd = UN_GRAPH_ALPHA_BLEND_VAL(E_GRAPH_BLEND_OP_REVSUBTRACT, E_GRAPH_BLEND_SRCALPHA, E_GRAPH_BLEND_ONE),
		E_GRAPH_ALPHA_BLEND_NCs_Cd   = UN_GRAPH_ALPHA_BLEND_VAL(E_GRAPH_BLEND_OP_REVSUBTRACT, E_GRAPH_BLEND_ONE,      E_GRAPH_BLEND_ONE),

		E_GRAPH_ALPHA_BLEND_FORCE_INT32 = 0x7fffffff,
	};

	/**
	* �N���A�t���O
	*/
	enum E_GRAPH_CLEAR_FLAG {
		E_GRAPH_CLEAR_FLAG_COLOR   = 1 << 0,
		E_GRAPH_CLEAR_FLAG_DEPTH   = 1 << 1,
		E_GRAPH_CLEAR_FLAG_STENCIL = 1 << 2,

		E_GRAPH_CLEAR_FLAG_ALL = E_GRAPH_CLEAR_FLAG_COLOR | E_GRAPH_CLEAR_FLAG_DEPTH | E_GRAPH_CLEAR_FLAG_STENCIL,

		E_GRAPH_CLEAR_FLAG_FORCE_INT32 = 0x7fffffff,
	};

	/**
	* �V�[���I���t���O
	*/
	enum E_GRAPH_END_SCENE_FLAG {
		E_GRAPH_END_SCENE_FLAG_RT_0 = 1 << 0,			// �����_�[�^�[�Q�b�g�O
		E_GRAPH_END_SCENE_FLAG_RT_1 = 1 << 1,			// �����_�[�^�[�Q�b�g�P

		E_GRAPH_END_SCENE_FLAG_DEPTH_STENCIL = 1 << 2,	// �[�x�E�X�e���V��

		E_GRAPH_END_SCENE_FLAG_FORCE_INT32 = 0x7fffffff,
	};

	/**
	* �s�N�Z���t�H�[�}�b�g
	*/
	enum E_GRAPH_PIXEL_FMT {
		E_GRAPH_PIXEL_FMT_RGBA8 = 0,
		E_GRAPH_PIXEL_FMT_BGRA8,

		E_GRAPH_PIXEL_FMT_RGBA4,
		E_GRAPH_PIXEL_FMT_RGB10A2,
		E_GRAPH_PIXEL_FMT_A8,
		
		E_GRAPH_PIXEL_FMT_R32F,
		E_GRAPH_PIXEL_FMT_RGBA16F,
		E_GRAPH_PIXEL_FMT_RGBA32F,

		E_GRAPH_PIXEL_FMT_DXT1,
		E_GRAPH_PIXEL_FMT_DXT3,
		E_GRAPH_PIXEL_FMT_DXT5,

		E_GRAPH_PIXEL_FMT_NUM,
		E_GRAPH_PIXEL_FMT_FORCE_INT32 = 0x7fffffff,
	};

	/**
	* �C���f�b�N�X�o�b�t�@�t�H�[�}�b�g
	*/
	enum E_GRAPH_INDEX_BUFFER_FMT {
		E_GRAPH_INDEX_BUFFER_FMT_INDEX16 = 0,
		E_GRAPH_INDEX_BUFFER_FMT_INDEX32,

		E_GRAPH_INDEX_BUFFER_FMT_NUM,
		E_GRAPH_INDEX_BUFFER_FMT_FORCE_INT32 = 0x7fffffff,
	};

	/**
	* �e�N�X�`���^�C�v
	*/
	enum E_GRAPH_TEX_TYPE {
		E_GRAPH_TEX_TYPE_PLANE = 0,	// �ʏ�
		E_GRAPH_TEX_TYPE_CUBE,		// �L���[�u
		E_GRAPH_TEX_TYPE_VOLUME,	// �{�����[��

		E_GRAPH_TEX_TYPE_NUM,
		E_GRAPH_TEX_TYPE_FORCE_INT32 = 0x7fffffff,
	};

	/**
	* �L���[�u�e�N�X�`����
	*/
	enum E_GRAPH_CUBE_TEX_FACE {
		E_GRAPH_CUBE_TEX_FACE_X_P = 0,	// +X
		E_GRAPH_CUBE_TEX_FACE_X_N,		// -X
		E_GRAPH_CUBE_TEX_FACE_Y_P,		// +Y
		E_GRAPH_CUBE_TEX_FACE_Y_N,		// -Y
		E_GRAPH_CUBE_TEX_FACE_Z_P,		// +Z
		E_GRAPH_CUBE_TEX_FACE_Z_N,		// -Z

		E_GRAPH_CUBE_TEX_FACE_NUM,
		E_GRAPH_CUBE_TEX_FACE_FORCE_INT32 = 0x7fffffff,
	};

	/**
	* 2D�`��ݒ�
	*/
	enum E_GRAPH_2D_RENDER_OP {
		E_GRAPH_2D_RENDER_OP_VTX = 0,		// Dst = Vtx
		E_GRAPH_2D_RENDER_OP_TEX,			// Dst = Tex
		E_GRAPH_2D_RENDER_OP_MODULATE,		// Dst = Vtx x Tex
		E_GRAPH_2D_RENDER_OP_MODULATE_2X,	// Dst = (Vtx x Tex) << 1
		E_GRAPH_2D_RENDER_OP_MODULATE_4X,	// Dst = (Vtx x Tex) << 2
		E_GRAPH_2D_RENDER_OP_ADD,			// Dst = Vtx + Tex

		E_GRAPH_2D_RENDER_OP_MODULATE_ALPHA,	// Dst = Vtx x Tex.a

		E_GRAPH_2D_RENDER_OP_NUM,
		E_GRAPH_2D_RENDER_OP_FORCE_INT32 = 0x7fffffff,
	};

	/**
	* �v���~�e�B�u�^�C�v
	*/
	enum E_GRAPH_PRIM_TYPE {
		E_GRAPH_PRIM_TYPE_POINTLIST,
		E_GRAPH_PRIM_TYPE_LINELIST,
		E_GRAPH_PRIM_TYPE_LINESTRIP,
		E_GRAPH_PRIM_TYPE_TRIANGLELIST,
		E_GRAPH_PRIM_TYPE_TRIANGLESTRIP,
		E_GRAPH_PRIM_TYPE_TRIANGLEFAN,

		E_GRAPH_PRIM_TYPE_NUM,
		E_GRAPH_PRIM_TYPE_FORCE_INT32 = 0x7fffffff,
	};

	/**
	* ���\�[�X�쐬�^�C�v
	*/
	enum E_GRAPH_RSC_CREATE_TYPE {
		E_GRAPH_RSC_CREATE_TYPE_STATIC  = 1 << 0,
		E_GRAPH_RSC_CREATE_TYPE_DYNAMIC = 1 << 1,
		E_GRAPH_RSC_CREATE_TYPE_STATIC_DYNAMIC = E_GRAPH_RSC_CREATE_TYPE_DYNAMIC | E_GRAPH_RSC_CREATE_TYPE_STATIC,

		E_GRAPH_RSC_CREATE_TYPE_FORCE_INT32 = 0x7fffffff,
	};

	/**
	* ���_�錾�̃f�[�^�^
	*/
	enum E_GRAPH_VTX_DECL_TYPE {
		E_GRAPH_VTX_DECL_TYPE_FLOAT1 =  0,	// 1D float expanded to (value, 0., 0., 1.)
		E_GRAPH_VTX_DECL_TYPE_FLOAT2, 		// 2D float expanded to (value, value, 0., 1.)
		E_GRAPH_VTX_DECL_TYPE_FLOAT3, 		// 3D float expanded to (value, value, value, 1.)
		E_GRAPH_VTX_DECL_TYPE_FLOAT4, 		// 4D float
		E_GRAPH_VTX_DECL_TYPE_COLOR, 		// 4D packed unsigned bytes mapped to 0. to 1. range. Input is in format (ARGB) expanded to (R, G, B, A)
		E_GRAPH_VTX_DECL_TYPE_UBYTE4,		// 4D unsigned byte
		E_GRAPH_VTX_DECL_TYPE_SHORT2,		// 2D signed short expanded to (value, value, 0., 1.)
		E_GRAPH_VTX_DECL_TYPE_SHORT4,		// 4D signed short

		// The following types are valid only with vertex shaders >= 2.0

		E_GRAPH_VTX_DECL_TYPE_UBYTE4N,		// Each of 4 bytes is normalized by dividing to 255.0
		E_GRAPH_VTX_DECL_TYPE_SHORT2N,		// 2D signed short normalized (v[0]/32767.0,v[1]/32767.0,0,1)
		E_GRAPH_VTX_DECL_TYPE_SHORT4N, 		// 4D signed short normalized (v[0]/32767.0,v[1]/32767.0,v[2]/32767.0,v[3]/32767.0)
		E_GRAPH_VTX_DECL_TYPE_USHORT2N, 	// 2D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,0,1)
		E_GRAPH_VTX_DECL_TYPE_USHORT4N, 	// 4D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,v[2]/65535.0,v[3]/65535.0)
		E_GRAPH_VTX_DECL_TYPE_UDEC3, 		// 3D unsigned 10 10 10 format expanded to (value, value, value, 1)
		E_GRAPH_VTX_DECL_TYPE_DEC3N, 		// 3D signed 10 10 10 format normalized and expanded to (v[0]/511.0, v[1]/511.0, v[2]/511.0, 1)
		E_GRAPH_VTX_DECL_TYPE_FLOAT16_2,	// Two 16-bit floating point values, expanded to (value, value, 0, 1)
		E_GRAPH_VTX_DECL_TYPE_FLOAT16_4, 	// Four 16-bit floating point values
		E_GRAPH_VTX_DECL_TYPE_UNUSED, 		// When the type field in a decl is unused.

		E_GRAPH_VTX_DECL_TYPE_NUM,
		E_GRAPH_VTX_DECL_TYPE_FORCE_INT32 = 0x7fffffff,
	};

	/**
	* ���_�f�[�^�̎g����
	*/
	enum E_GRAPH_VTX_DECL_USAGE {
		E_GRAPH_VTX_DECL_USAGE_POSITION = 0,	// ���_�ʒu
		E_GRAPH_VTX_DECL_USAGE_BLENDWEIGHT,		// �u�����f�B���O�̏d��
		E_GRAPH_VTX_DECL_USAGE_BLENDINDICES,	// �u�����f�B���O �C���f�b�N�X 
		E_GRAPH_VTX_DECL_USAGE_NORMAL,			// ���_�@��
		E_GRAPH_VTX_DECL_USAGE_PSIZE,			// �|�C���g �T�C�Y 
		E_GRAPH_VTX_DECL_USAGE_TEXCOORD,		// �e�N�X�`�����W
		E_GRAPH_VTX_DECL_USAGE_TANGENT,			// ���_�̐ڐ�
		E_GRAPH_VTX_DECL_USAGE_BINORMAL,		// ���_�̏]�@��
		E_GRAPH_VTX_DECL_USAGE_TESSFACTOR,
		E_GRAPH_VTX_DECL_USAGE_POSITIONT,		// ���W�ϊ����ꂽ�ʒu���W
		E_GRAPH_VTX_DECL_USAGE_COLOR,			// ���_�J���[
		E_GRAPH_VTX_DECL_USAGE_FOG,				// �t�H�O
		E_GRAPH_VTX_DECL_USAGE_DEPTH,
		E_GRAPH_VTX_DECL_USAGE_SAMPLE,

		E_GRAPH_VTX_DECL_USAGE_NUM,
		E_GRAPH_VTX_DECL_USAGE_FORCE_INT32 = 0x7fffffff,
	};

	/**
	* �r���[�|�[�g
	*/
	struct SViewport {
		// Viewport Top left
		UN_UINT x;
		UN_UINT y;

		// Viewport Dimensions
		UN_UINT width;
		UN_UINT height;

		// Min/max of clip Volume
		UN_FLOAT minZ;
		UN_FLOAT maxZ;
	};

	/**
	* �T���v���X�e�[�g
	*/
	struct S_SAMPLER_STATE {
		// �t�B���^
		E_GRAPH_TEX_FILTER minFilter;
		E_GRAPH_TEX_FILTER magFilter;
		E_GRAPH_TEX_FILTER mipFilter;

		// �A�h���b�V���O
		E_GRAPH_TEX_ADDRESS addressU;
		E_GRAPH_TEX_ADDRESS addressV;

		// �R���X�g���N�^
		S_SAMPLER_STATE()
		{
			minFilter = E_GRAPH_TEX_FILTER_LINEAR;
			magFilter = E_GRAPH_TEX_FILTER_LINEAR;
			mipFilter = E_GRAPH_TEX_FILTER_LINEAR;

			addressU = E_GRAPH_TEX_ADDRESS_CLAMP;
			addressV = E_GRAPH_TEX_ADDRESS_CLAMP;
		}
	};

	////////////////////////////////////////////
	
	/**
	* ���Z�b�g�p�R�[���o�b�N�֐��^
	*/
	typedef UN_BOOL (*GraphicsDeviceResetCallBack)();

	/**
	* �f�o�C�X���X�g�p�R�[���o�b�N�֐��^
	*/
	typedef void (*GraphicsDeviceLostDeviceCallBack)();
}	// namespace uranus;

#endif	// #if !defined(__URANUS_GRAPH_GRAPH_DEFS_H__)
