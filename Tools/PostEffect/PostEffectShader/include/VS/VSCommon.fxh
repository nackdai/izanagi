#if !defined(__IZANAGI_POSTEFFECT_VS_COMMON_FXH__)
#define __IZANAGI_POSTEFFECT_VS_COMMON_FXH__

#include "../PostEffectShaderIO.fxh"

/********************************************
   ���_�V�F�[�_���ʏ���
********************************************/

// ���_�V�F�[�_����
struct S_PE_VS_IN {
	float4 vPos	: POSITION;
	float2 vUV	: TEXCOORD0;
};

///////////////////////////////////////////
// ���_�ʒu�v�Z

// ���_�ʒu�I�t�Z�b�g
float4 g_vPosOffset = (float4)0.0f;

// ���_�ʒu�v�Z
float4 _ComputeVSPosition(in float4 vPos)
{
	float4 vRet = vPos;

	// [0.0f, 0.0f] - [1.0f, 1.0f] -> [-1.0f, -1.0f] - [1.0f, 1.0f]
	vRet.xy = (vRet.xy + g_vPosOffset.xy) * 2.0f - 1.0f;

	// �����Y���W�͔��]������
	vRet.y *= -1.0f;

	return vRet;
}

///////////////////////////////////////////
// �e�N�X�`�����W�v�Z

// xy : scale
// zw : offset
float4 g_vTexParam = {
	1.0f, 1.0f, 0.0f, 0.0f
};

// �X�N���[���T�C�Y
float2 g_vQuadScreenSize : VIEWPORTPIXELSIZE;

///////////////////////////////////////////
// ���_�V�F�[�_�o��

#define S_PE_VS_OUT				S_PE_PS_IN
#define S_PE_VS_OUT_SAMPLING_4	S_PE_PS_IN_SAMPLING_4
#define S_PE_VS_OUT_SAMPLING_8	S_PE_PS_IN_SAMPLING_8
#define S_PE_VS_OUT_SAMPLING_16	S_PE_PS_IN_SAMPLING_16
#define S_PE_VS_OUT_SAMPLING_5	S_PE_PS_IN_SAMPLING_5
#define S_PE_VS_OUT_SAMPLING_7	S_PE_PS_IN_SAMPLING_7
#define S_PE_VS_OUT_SAMPLING_9	S_PE_PS_IN_SAMPLING_9
#define S_PE_VS_OUT_SAMPLING_13	S_PE_PS_IN_SAMPLING_13
#define S_PE_VS_OUT_SAMPLING_15	S_PE_PS_IN_SAMPLING_15

#endif	// #if !defined(__IZANAGI_POSTEFFECT_VS_COMMON_FXH__)
