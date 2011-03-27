#if !defined(__IZANAGI_POSTEFFECT_SHADER_DEFS_TEX_FXH__)
#define __IZANAGI_POSTEFFECT_SHADER_DEFS_TEX_FXH__

////////////////////////////////////////////////////////
// �e�N�X�`��

//****************************************
// �A�m�e�[�V����

#define SetTexSize(w, h)		float2 Dimensions = {w, h}		// �e�N�X�`���T�C�Y
#define SetTexRatio(w, h)		float2 ViewportRatio = {w, h}	// �X�N���[���T�C�Y�䗦
#define SetTexFormat(fmt)		string Format = fmt				// �s�N�Z���t�H�[�}�b�g
#define SetIsRenderTarget(b)	bool IsRenderTarget = b			// �����_�[�^�[�Q�b�g���ǂ���
#define SetIsDynamic(b)			bool IsDynamic = b				// ���I���ǂ���

// �s�N�Z���t�H�[�}�b�g�ݒ�l
#define PIXEL_FORMAT_RGBA8		"A8R8G8B8"
#define PIXEL_FORMAT_RGBA16F	"A16B16G16R16F"
#define PIXEL_FORMAT_RGBA32F	"A32B32G32R32F"
#define PIXEL_FORAMT_R16F		"R16F"
#define PIXEL_FORAMT_R32F		"R32F"
#define PIXEL_FORMAT_A2RGB10	"A2R10G10B10"
#define PIXEL_FORMAT_A8			"A8"

#endif	// #if !defined(__IZANAGI_POSTEFFECT_SHADER_DEFS_TEX_FXH__)
