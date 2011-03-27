// 2D�p���_�V�F�[�_

#include "2DShader.hlsl"

// �X�N���[���T�C�Y�̋t��
float4 g_vInvScreen = (float4)1.0f;

SVSOutput main(SVSInput sIn)
{
	SVSOutput sOut;

	// ���_�ʒu
	sOut.vPos = sIn.vPos;
	sOut.vPos.xy *= g_vInvScreen.xy;

	// [0.0f, 0.0f] - [1.0f, 1.0f] -> [-1.0f, -1.0f] - [1.0f, 1.0f]
	sOut.vPos.xy = sOut.vPos.xy * 2.0f - 1.0f;

	// �����Y���W�͔��]������
	sOut.vPos.y *= -1.0f;

	// ���_�J���[
	sOut.vColor = sIn.vColor;

	// UV���W
	sOut.vUV = sIn.vUV;

	return sOut;
}
