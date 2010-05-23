#if !defined(__URANUS_GRAPH_D3D9_PARAM_VALUE_CONVERTER_H__)
#define __URANUS_GRAPH_D3D9_PARAM_VALUE_CONVERTER_H__

#include "unDefs.h"
#include "graph/GraphDefs.h"

#define FUNC_ABSTRACT_TO_TARGET(p, ret, arg)	ret ConvAbstractToTarget_##p(arg v)
#define FUNC_TARGET_TO_ABSTRACT(p, ret, arg)	ret ConvTargetToAbstract_##p(arg v)

#define FUNC_CONV_PARAM_VAL(p, target, abst)\
	static FUNC_ABSTRACT_TO_TARGET(p, target, abst);\
	static FUNC_TARGET_TO_ABSTRACT(p, abst, target)

namespace uranus {
	/**
	* URANUS�̃p�����[�^ -> D3D9�̃p�����[�^
	*/
	class CD3D9ParamValueConverter {
	private:
		CD3D9ParamValueConverter();
		~CD3D9ParamValueConverter();

		NO_COPIABLE(CD3D9ParamValueConverter);

	public:
		// �t�B�����[�h
		FUNC_CONV_PARAM_VAL(FillMode, D3DFILLMODE, E_GRAPH_FILL_MODE);

		// �u�����h���@
		FUNC_CONV_PARAM_VAL(BlendOp, D3DBLENDOP, E_GRAPH_BLEND_OP);

		// �u�����h�W��
		FUNC_CONV_PARAM_VAL(Blend, D3DBLEND, E_GRAPH_BLEND);

		// �J�����O���[�h
		FUNC_CONV_PARAM_VAL(Cull, D3DCULL, E_GRAPH_CULL);

		// ��r����
		FUNC_CONV_PARAM_VAL(Cmp, D3DCMPFUNC, E_GRAPH_CMP_FUNC);

		// �e�N�X�`���A�h���b�V���O
		FUNC_CONV_PARAM_VAL(TexAddr, D3DTEXTUREADDRESS, E_GRAPH_TEX_ADDRESS);

		// �e�N�X�`���t�B���^
		FUNC_CONV_PARAM_VAL(TexFilter, D3DTEXTUREFILTERTYPE, E_GRAPH_TEX_FILTER);

		// �T���v���[�X�e�[�g�^�C�v
		FUNC_CONV_PARAM_VAL(SamplerStateType, D3DSAMPLERSTATETYPE, E_GRAPH_SAMPLER_STATE_TYPE);

		// �s�N�Z���t�H�[�}�b�g
		FUNC_CONV_PARAM_VAL(PixelFormat, D3DFORMAT, E_GRAPH_PIXEL_FMT);

		// �C���f�b�N�X�o�b�t�@�t�H�[�}�b�g
		FUNC_CONV_PARAM_VAL(IdxBufFmt, D3DFORMAT, E_GRAPH_INDEX_BUFFER_FMT);

		// �v���~�e�B�u�^�C�v
		FUNC_CONV_PARAM_VAL(PrimType, D3DPRIMITIVETYPE, E_GRAPH_PRIM_TYPE);

		// �L���[�u�}�b�v�t�F�C�X
		FUNC_CONV_PARAM_VAL(CubeFace, D3DCUBEMAP_FACES, E_GRAPH_CUBE_TEX_FACE);

		// ���_�錾�̃f�[�^�^
		FUNC_CONV_PARAM_VAL(VtxDeclType, D3DDECLTYPE, E_GRAPH_VTX_DECL_TYPE);

		// ���_�f�[�^�̎g����
		FUNC_CONV_PARAM_VAL(VtxDeclUsage, D3DDECLUSAGE, E_GRAPH_VTX_DECL_USAGE);
	};
}	// namespace uranus


#endif	// #if !defined(__URANUS_GRAPH_D3D9_PARAM_VALUE_CONVERTER_H__)
