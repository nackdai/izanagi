#if !defined(__IZANAGI_POSTEFFECT_SHADER_UTILITY_FXH__)
#define __IZANAGI_POSTEFFECT_SHADER_UTILITY_FXH__

//////////////////////////////////////////////////
// �[�x�l�ϊ�

// Z�l
float ConvertZ(float4 vIn)
{
#if defined(__Z_RGB__)
	// NOTE
	// RGB 24bit���x
#if 0
	return dot(vIn.rgb, float3(16711680.0f / 16777215.0f, 65280.0f / 16777215.0f, 255.0f / 16777215.0f));
#else
	return dot(vIn.rgb, float3(255.0f / 256.0f, 255.0f / (256.0f * 256.0f), 256.0f/ (256.0f * 256.0f * 256.0f)));
#endif
#elif defined(__Z_GBA__)
	// NOTE
	// GBA 24bit���x
	return dot(vIn.gba, float3(256.0f/ (256.0f * 256.0f * 256.0f), 255.0f / (256.0f * 256.0f), 255.0f / 256.0f));
#else
	// NOTE
	// 8bit���x RGB���ꂼ��ɓ����l�������Ă���
	return vIn.r;
#endif
}

// Z�l
float ConvertZ(in sampler smplIn, float2 vUV)
{
	return ConvertZ(tex2D(smplIn, vUV));
}

// W�l
float ConvertW(float4 vIn)
{
#if defined(__W_RGB__)
	// NOTE
	// RGB 24bit���x
#if 0
	return dot(vIn.rgb, float3(16711680.0f / 16777215.0f, 65280.0f / 16777215.0f, 255.0f / 16777215.0f));
#else
	return dot(vIn.rgb, float3(255.0f / 256.0f, 255.0f / (256.0f * 256.0f), 256.0f/ (256.0f * 256.0f * 256.0f)));
#endif
#else
	// NOTE
	// 8bit���x RGB���ꂼ��ɓ����l�������Ă���
	return vIn.r;
#endif
}

// W�l
float ConvertW(in sampler smplIn, float2 vUV)
{
	return ConvertW(tex2D(smplIn, vUV));
}

//////////////////////////////////////////////////
// �s�N�Z���V�F�[�_���͍\���̃w���p

// ���S�_�擾
// �R���R�k���i�|�C���g�t�B���^�j
float2 GetCenterUVFromDownScale3x3(S_PE_PS_IN_DOWNSCALE_3x3_POINT sIn)
{
	return sIn.vUV[2].xy;
}

// ���S�_�擾
// �X�|�C���g�T���v�����O�i���j�A�t�B���^�j
float2 GetCenterUVFrom9PointSample(S_PE_PS_IN_9POINTSAMPLE_LINEAR sIn)
{
	// NOTE
	// +---+---+---+
	// |   |   3   |
	// +-0-+---+---+
	// |   | 4 |   |
	// +---+---+-2-+
	// |   1   |   |
	// +---+---+---+

	return sIn.vUV_0;
}

// ���S�_�擾
// �X�|�C���g�T���v�����O�i�|�C���g�t�B���^�j
float2 GetCenterUVFrom9PointSample(S_PE_PS_IN_9POINTSAMPLE_POINT sIn)
{
	// NOTE
	// +---+---+---+
	// | 0 | 1 | 2 |
	// +---+---+---+
	// | 3 | 4 | 5 |
	// +---+---+---+
	// | 6 | 7 | 8 |
	// +---+---+---+

	return GetCenterUVFromDownScale3x3(sIn);
}

// �T���v�����O�_�擾
// �S���S�k���i���j�A�t�B���^�j
float2 GetSamplePointFromDownScale4x4(int idx, S_PE_PS_IN_DOWNSCALE_4x4_LINEAR sIn)
{
	// NOTE
	// +---+---+---+---+
	// |   |   |   |   |
	// +---0---+---1---+
	// |   |   |   |   |
	// +---+---x---+---+
	// |   |   |   |   |
	// +---2---+---3---+
	// |   |   |   |   |
	// +---+---+---+---+

	if (idx % 2 == 0) {
		return sIn.vUV[idx / 2].xy;
	}
	else {
		return sIn.vUV[idx / 2].zw;
	}
}

// �T���v�����O�_�擾
// �S���S�k���i�|�C���g�t�B���^�j
float2 GetSamplePointFromDownScale4x4(int idx, S_PE_PS_IN_DOWNSCALE_4x4_POINT sIn)
{
	// NOTE
	// +---+---+---+---+
	// | 0 | 1 | 2 | 3 |
	// +---+---+---+---+
	// | 4 | 5 | 6 | 7 |
	// +---+---x---+---+
	// | 8 | 9 | 10| 11|
	// +---+---+---+---+
	// | 12| 13| 14| 15|
	// +---+---+---+---+

	if (idx % 2 == 0) {
		return sIn.vUV[idx / 2].xy;
	}
	else {
		return sIn.vUV[idx / 2].zw;
	}
}

// �T���v�����O�_�擾
// �Q���Q�k���i���j�A�t�B���^�j
float2 GetSamplePointFromDownScale2x2(int idx, S_PE_PS_IN_DOWNSCALE_2x2_LINEAR sIn)
{
	return sIn.vUV;
}

// �T���v�����O�_�擾
// �Q���Q�k���i�|�C���g�t�B���^�j
float2 GetSamplePointFromDownScale2x2(int idx, S_PE_PS_IN_DOWNSCALE_2x2_POINT sIn)
{
	// NOTE
	// +---+---+
	// | 0 | 1 |
	// +---x---+
	// | 2 | 3 |
	// +---+---+

	if (idx % 2 == 0) {
		return sIn.vUV[idx / 2].xy;
	}
	else {
		return sIn.vUV[idx / 2].zw;
	}
}

// �T���v�����O�_�擾
// �R���R�k���i���j�A�t�B���^�j
float2 GetSamplePointFromDownScale3x3(int idx, S_PE_PS_IN_DOWNSCALE_3x3_LINEAR sIn)
{
	// NOTE
	// +---+---+---+
	// |  0|   |1  |
	// +---+---+---+
	// |   | x |   |
	// +---+---+---+
	// |  2|   |3  |
	// +---+---+---+

	if (idx % 2 == 0) {
		return sIn.vUV[idx / 2].xy;
	}
	else {
		return sIn.vUV[idx / 2].zw;
	}
}

// �T���v�����O�_�擾
// �R���R�k���i�|�C���g�t�B���^�j
float2 GetSamplePointFromDownScale3x3(int idx, S_PE_PS_IN_DOWNSCALE_3x3_POINT sIn)
{
	// NOTE
	// +---+---+---+
	// | 0 | 1 | 2 |
	// +---+---+---+
	// | 3 | 4 | 5 |
	// +---+---+---+
	// | 6 | 7 | 8 |
	// +---+---+---+

	if (idx == 8) {
		return sIn.vUV_0;
	}
	else if (idx % 2 == 0) {
		return sIn.vUV[idx / 2].xy;
	}
	else {
		return sIn.vUV[idx / 2].zw;
	}
}

// �T���v�����O�_�擾
// �X�|�C���g�T���v�����O�i���j�A�t�B���^�j
float2 GetSamplePointFrom9PointSample(int idx, S_PE_PS_IN_9POINTSAMPLE_LINEAR sIn)
{
	// NOTE
	// +---+---+---+
	// |   |   3   |
	// +-0-+---+---+
	// |   | 4 |   |
	// +---+---+-2-+
	// |   1   |   |
	// +---+---+---+

	if (idx == 4) {
		return sIn.vUV_0;
	}
	else if (idx % 2 == 0) {
		return sIn.vUV[idx / 2].xy;
	}
	else {
		return sIn.vUV[idx / 2].zw;
	}
}

// �T���v�����O�_�擾
// �X�|�C���g�T���v�����O�i�|�C���g�t�B���^�j
float2 GetSamplePointFrom9PointSample(int idx, S_PE_PS_IN_9POINTSAMPLE_POINT sIn)
{
	// NOTE
	// +---+---+---+
	// | 0 | 1 | 2 |
	// +---+---+---+
	// | 3 | 4 | 5 |
	// +---+---+---+
	// | 6 | 7 | 8 |
	// +---+---+---+

	return GetSamplePointFromDownScale3x3(idx, sIn);
}

#endif	// #if !defined(__IZANAGI_POSTEFFECT_SHADER_UTILITY_FXH__)
