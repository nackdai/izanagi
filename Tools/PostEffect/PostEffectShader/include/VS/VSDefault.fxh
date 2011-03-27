#if !defined(__IZANAGI_POSTEFFECT_VS_DEFAULT_FXH__)
#define __IZANAGI_POSTEFFECT_VS_DEFAULT_FXH__

#include "VSCommon.fxh"

// �f�t�H���g���_�V�F�[�_
S_PE_VS_OUT VSDefault(S_PE_VS_IN sIn)
{
	S_PE_VS_OUT sOut;

	// ���_�ʒu�v�Z
	sOut.vPos = _ComputeVSPosition(sIn.vPos);

	// �e�N�X�`�����W
	//	out = in * scale + offset
	sOut.vUV.xy = sIn.vUV.xy * g_vTexParam.xy + g_vTexParam.zw;

	return sOut;
}

#endif	// #if !defined(__IZANAGI_POSTEFFECT_VS_DEFAULT_FXH__)
