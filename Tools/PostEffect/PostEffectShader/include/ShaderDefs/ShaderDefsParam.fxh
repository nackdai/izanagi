#if !defined(__IZANAGI_POSTEFFECT_SHADER_DEFS_PARAM_FXH__)
#define __IZANAGI_POSTEFFECT_SHADER_DEFS_PARAM_FXH__

////////////////////////////////////////////////////////
// �p�����[�^

//****************************************
// �A�m�e�[�V����

// UI�R���g���[���ݒ�p
#define SetUIWidget(widget)			string UIWidget = widget
#define SetUIName(name)				string UIName = name
#define SetUIHelp(help)				string UIHelp = help

#define SetUIMax1(v0)				float1 UIMax = v0
#define SetUIMax2(v0, v1)			float2 UIMax = {v0, v1}
#define SetUIMax3(v0, v1, v2)		float3 UIMax = {v0, v1, v2}
#define SetUIMax4(v0, v1, v2, v3)	float4 UIMax = {v0, v1, v2, v3}

#define SetUIMin1(v0)				float1 UIMin = v0
#define SetUIMin2(v0, v1)			float2 UIMin = {v0, v1}
#define SetUIMin3(v0, v1, v2)		float3 UIMin = {v0, v1, v2}
#define SetUIMin4(v0, v1, v2, v3)	float4 UIMin = {v0, v1, v2, v3}

#define SetUIStep1(v0)				float1 UIStep = v0
#define SetUIStep2(v0, v1)			float2 UIStep = {v0, v1}
#define SetUIStep3(v0, v1, v2)		float3 UIStep = {v0, v1, v2}
#define SetUIStep4(v0, v1, v2, v3)	float4 UIStep = {v0, v1, v2, v3}

#if 0
// UI�R���g���[��(UIWidget)�ݒ�l
#define UI_WIDGET_SLIDER		"Slider"		// �X���C�_�[
#define UI_WIDGET_SPINNER		"Spinner"		// �X�s�i�[
#define UI_WIDGET_COLOR			"Color"			// �J���[�s�b�J�[
#define UI_WIDGET_GRAYSCALE		"GrayScale"		// �S�Ă̐����𓯂��l�ɂ���K�v������
#define UI_WIDGET_NUMERIC		"Numeric"		// ���l�w��
#define UI_WIDGET_STRING		"String"		// ������w��
#define UI_WIDGET_TEXTURE		"Texture"		// �e�N�X�`���w��
#define UI_WIDGET_DIRECTION		"Direction"		// ���C�g�̕����Ƃ��i�|�X�g�G�t�F�N�g�ł͕s�v���ȁE�E�E�j
#define UI_WIDGET_OBJECT		"Object"		// �A�m�e�[�V�����Ɉˑ�
#define UI_WIDGET_NONE			"None"			// ���̃A�m�e�[�V������\�����Ȃ�
#endif

// �G�t�F�N�g���ł͎g�p���Ȃ��ꍇ�ł��A�폜���Ȃ�
#define SetDoNotStrip(b)	bool DoNotStrip = b

#endif	// #if !defined(__IZANAGI_POSTEFFECT_SHADER_DEFS_PARAM_FXH__)
