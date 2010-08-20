#if !defined(__URANUS_GRAPH_OGL_PARAM_VALUE_CONVERTER_H__)
#define __URANUS_GRAPH_OGL_PARAM_VALUE_CONVERTER_H__

#include "unDefs.h"
#include "unD3DDefs.h"
#include "graph/GraphDefs.h"

#define FUNC_ABSTRACT_TO_TARGET(p, ret, arg)	ret ConvAbstractToTarget_##p(arg v)
#define FUNC_TARGET_TO_ABSTRACT(p, ret, arg)	ret ConvTargetToAbstract_##p(arg v)

#define FUNC_CONV_PARAM_VAL(p, target, abst)\
	static FUNC_ABSTRACT_TO_TARGET(p, target, abst);\
	static FUNC_TARGET_TO_ABSTRACT(p, abst, target)

#define UN_OGL_PIXEL_FMT(fmt, type)	(((fmt) << 16) | (type))
#define UN_OGL_GET_PIXEL_FMT(val)	(((fmt) & 0xff00) >> 16)
#define UN_OGL_GET_PIXEL_TYPE(val)	((fmt) & 0xff)

namespace uranus {
	/**
	* URANUS�̃p�����[�^ -> OGL�̃p�����[�^
	*/
	class COGLParamValueConverter {
	private:
		COGLParamValueConverter();
		~COGLParamValueConverter();

		NO_COPIABLE(COGLParamValueConverter);

	public:
		// �t�B�����[�h
		FUNC_CONV_PARAM_VAL(FillMode, GLenum, E_GRAPH_FILL_MODE);

		// �u�����h���@
		FUNC_CONV_PARAM_VAL(BlendOp, GLenum, E_GRAPH_BLEND_OP);

		// �u�����h�W��
		FUNC_CONV_PARAM_VAL(Blend, GLenum, E_GRAPH_BLEND);

		// �J�����O���[�h
		FUNC_CONV_PARAM_VAL(Cull, GLenum, E_GRAPH_CULL);

		// ��r����
		FUNC_CONV_PARAM_VAL(Cmp, GLenum, E_GRAPH_CMP_FUNC);

		// �e�N�X�`���A�h���b�V���O
		FUNC_CONV_PARAM_VAL(TexAddr, GLenum, E_GRAPH_TEX_ADDRESS);

		// �e�N�X�`���t�B���^
		FUNC_CONV_PARAM_VAL(TexFilter, GLenum, E_GRAPH_TEX_FILTER);

		// �T���v���[�X�e�[�g�^�C�v
		FUNC_CONV_PARAM_VAL(SamplerStateType, GLenum, E_GRAPH_SAMPLER_STATE_TYPE);

		// �s�N�Z���t�H�[�}�b�g
		FUNC_CONV_PARAM_VAL(PixelFormat, GLenum, E_GRAPH_PIXEL_FMT);

		// �C���f�b�N�X�o�b�t�@�t�H�[�}�b�g
		FUNC_CONV_PARAM_VAL(IdxBufFmt, GLenum, E_GRAPH_INDEX_BUFFER_FMT);

		// �v���~�e�B�u�^�C�v
		FUNC_CONV_PARAM_VAL(PrimType, GLenum, E_GRAPH_PRIM_TYPE);

		// �L���[�u�}�b�v�t�F�C�X
		FUNC_CONV_PARAM_VAL(CubeFace, GLenum, E_GRAPH_CUBE_TEX_FACE);

		// ���_�錾�̃f�[�^�^
		FUNC_CONV_PARAM_VAL(VtxDeclType, GLenum, E_GRAPH_VTX_DECL_TYPE);

		// ���_�f�[�^�̎g����
		FUNC_CONV_PARAM_VAL(VtxDeclUsage, GLenum, E_GRAPH_VTX_DECL_USAGE);
	};

	UN_C_ASSERT(sizeof(GLenum) == sizeof(UN_UINT));

	typedef COGLParamValueConverter CParamValueConverter;
}	// namespace uranus


#endif	// #if !defined(__URANUS_GRAPH_OGL_PARAM_VALUE_CONVERTER_H__)
