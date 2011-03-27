#if !defined(__IZANAGI_POSTEFFECT_SHADER_DEFS_PASS_FXH__)
#define __IZANAGI_POSTEFFECT_SHADER_DEFS_PASS_FXH__

#include "ShaderDefsScript.fxh"
#include "ShaderDefsFunctor.fxh"

////////////////////////////////////////////////////////
// �p�X

//****************************************
// �A�m�e�[�V����

#if 0
// �p�X�̊֘A�t���ݒ�
// �E�t�@���N�^�ɕ����̃p�X�������������Ƃ��Ɏg�p����B
// �E�ő�łS�܂Őݒ�ł���B
// �E�������g��ݒ�A��d�o�^�͍s�����Ƃ��ł��Ȃ��B
#define SetConnectPass_1(pass0)							string ConnectPass = #pass0
#define SetConnectPass_2(pass0, pass1)					string ConnectPass = #pass0","#pass1
#define SetConnectPass_3(pass0, pass1, pass2)			string ConnectPass = #pass0","#pass1","#pass2
#define SetConnectPass_4(pass0, pass1, pass2, pass3)	string ConnectPass = #pass0","#pass1","#pass2","#pass3
#endif

// �o�͐�̃V�U�����O�ݒ�
// �E������0�ȊO�̒l���ݒ肳�ꂽ�ꍇ�́A�V�U�����O���s�����̂Ƃ���B
// �E�����_�[�^�[�Q�b�g�̃T�C�Y��inflate����`�Őݒ肷��B
// �E�����_�[�^�[�Q�b�g���w�肳��Ȃ��ꍇ�́A�����Őݒ肳�ꂽ�l�͎g�p����Ȃ��B
// �Ex�Ay ��[-32768 - 32767]�̐������Z�b�g���邱�ƁB����ȊO���Z�b�g�����Ƃ��̓���͕ۏႳ��Ȃ��B
#define SetScissorRectInflate(x, y)		int2 ScissorRectInflate = {x, y}

// �\�[�X�e�N�X�`����`�ݒ�
// �E�\�[�X�e�N�X�`���́ACPostEffectFunctorXXXX::GetMainSrcTexture �Ŏ擾�ł���e�N�X�`���̂��Ƃł���B
// �E�\�[�X�e�N�X�`����`�̃T�C�Y��inflate����`�Őݒ肷��B
// �Ex�Ay ��[-32768 - 32767]�̐������Z�b�g���邱�ƁB����ȊO���Z�b�g�����Ƃ��̓���͕ۏႳ��Ȃ��B
#define SetSrcTexRectInflate(x, y)		int2 SrcTexRectInflate = {x, y}

// �e�N�X�`���I�t�Z�b�g
#define SetTextureOffset(offset)		string TextureOffset = #offset

//****************************************
// �X�e�[�g

// NOTE
// �s�N�Z���p�C�v���C�������_�[�X�e�[�g�̂�

#define SetAlphaBlendEnable(b)		AlphaBlendEnable = b
//#define SetAlphaFunc(func)			AlphaFunc = func
//#define SetAlphaRef(ref)			AlphaRef = ref
#define SetAlphaTestEnable(b)		AlphaTestEnable = b
#define SetBlendOp(op)				BlendOp = op
//#define SetDestBlend(blend)			DestBlend = blend
//#define SetSrcBlend(blend)			SrcBlend = blend
//#define SetAlphaBlend(op, src, dst)	SetBlendOp(op); SetSrcBlend(src); SetDestBlend(dst)

#if defined(__HLSL__)
#define SetAlphaFunc(func, ref)		AlphaFunc = func; AlphaRef = ref
#define SetBlendFunc(src, dst)		SrcBlend = src; DestBlend = dst
#else	// #if defined(__HLSL__)
#define SetAlphaFunc(func, ref)		AlphaFunc = float2(func, ref)
#define SetBlendFunc(src, dst)		BlendFunc = int2(src, dst)
#endif	// #if defined(__HLSL__)

#define SetAlphaBlend(op, src, dst)	SetBlendOp(op); SetBlendFunc(src, dst)

// ��r����
#if defined(__HLSL__)
#define CMP_NEVER				NEVER			// ��Ƀe�X�g�͎��s. 
#define CMP_LESS				LESS			// NEW < OLD �Ȃ琬��. 
#define CMP_EQUAL				EQUAL			// NEW == OLD �Ȃ琬��. 
#define CMP_LESSEQUAL			LESSEQUAL		// NEW <= OLD �Ȃ琬��. 
#define CMP_GREATER				GREATER			// NEW > OLD �Ȃ琬��. 
#define CMP_NOTEQUAL			NOTEQUAL		// NEW != OLD �Ȃ琬��. 
#define CMP_GREATEREQUAL		GREATEREQUAL	// NEW >= OLD �Ȃ琬��. 
#define CMP_ALWAYS				ALWAYS			// ��Ƀe�X�g�͐���. 
#else	// #if defined(__HLSL__)
#define CMP_NEVER				Never			// ��Ƀe�X�g�͎��s. 
#define CMP_LESS				Less			// NEW < OLD �Ȃ琬��. 
#define CMP_EQUAL				Equal			// NEW == OLD �Ȃ琬��. 
#define CMP_LESSEQUAL			LEqual			// NEW <= OLD �Ȃ琬��. 
#define CMP_GREATER				Greater			// NEW > OLD �Ȃ琬��. 
#define CMP_NOTEQUAL			NotEqual		// NEW != OLD �Ȃ琬��. 
#define CMP_GREATEREQUAL		GEqual			// NEW >= OLD �Ȃ琬��. 
#define CMP_ALWAYS				Always			// ��Ƀe�X�g�͐���. 
#endif	// #if defined(__HLSL__)

// �u�����f�B���O����
#if defined(__HLSL__)
#define BLENDOP_ADD				ADD
#define BLENDOP_SUBTRACT		SUBTRACT
#define BLENDOP_REVSUBTRACT		REVSUBTRACT
#define BLENDOP_MIN				MIN
#define BLENDOP_MAX				MAX
#else	// #if defined(__HLSL__)
#define BLENDOP_ADD				Add
#define BLENDOP_SUBTRACT		Subtract
#define BLENDOP_REVSUBTRACT		ReverseSubtract
#define BLENDOP_MIN				Min
#define BLENDOP_MAX				Max
#endif	// #if defined(__HLSL__)

// �u�����f�B���O���[�h
#if defined(__HLSL__)
#define BLEND_ZERO				ZERO			// (0, 0, 0, 0)
#define BLEND_ONE				ONE				// (1, 1, 1, 1)
#define BLEND_SRCCOLOR			SRCCOLOR		// (Rs, Gs, Bs, As)
#define BLEND_INVSRCCOLOR		INVSRCCOLOR		// (1 - Rs, 1 - Gs, 1 - Bs, 1 - As)
#define BLEND_SRCALPHA			SRCALPHA		// (As, As, As, As)
#define BLEND_INVSRCALPHA		INVSRCALPHA		// (1 - As, 1 - As, 1 - As, 1 - As)
#define BLEND_DESTALPHA			DESTALPHA		// (Ad, Ad, Ad, Ad)
#define BLEND_INVDESTALPHA		INVDESTALPHA	// (1 - Ad, 1 - Ad, 1 - Ad, 1 - Ad)
#define BLEND_DESTCOLOR			DESTCOLOR		// (Rd, Gd, Bd, Ad)
#define BLEND_INVDESTCOLOR		INVDESTCOLOR	// (1 - Rs, 1 - Gs, 1 - Bs, 1 - As)
#define BLEND_SRCALPHASAT		SRCALPHASAT		// (f, f, f, 1)	(f = min(As, 1 - Ad)
#else	// #if defined(__HLSL__)
#define BLEND_ZERO				Zero				// (0, 0, 0, 0)
#define BLEND_ONE				One					// (1, 1, 1, 1)
#define BLEND_SRCCOLOR			SrcColor			// (Rs, Gs, Bs, As)
#define BLEND_INVSRCCOLOR		OneMinusSrcColor	// (1 - Rs, 1 - Gs, 1 - Bs, 1 - As)
#define BLEND_SRCALPHA			SrcAlpha			// (As, As, As, As)
#define BLEND_INVSRCALPHA		OneMinusSrcAlpha	// (1 - As, 1 - As, 1 - As, 1 - As)
#define BLEND_DESTALPHA			DstAlpha			// (Ad, Ad, Ad, Ad)
#define BLEND_INVDESTALPHA		OneMinusDstAlpha	// (1 - Ad, 1 - Ad, 1 - Ad, 1 - Ad)
#define BLEND_DESTCOLOR			DestColor			// (Rd, Gd, Bd, Ad)
#define BLEND_INVDESTCOLOR		OneMinusDestColor	// (1 - Rs, 1 - Gs, 1 - Bs, 1 - As)
#define BLEND_SRCALPHASAT		SrcAlphaSaturate	// (f, f, f, 1)	(f = min(As, 1 - Ad)
#endif	// #if defined(__HLSL__)

// �A���t�@�u�����h�ݒ�
#define SetAlphaBlend_Normal()		SetAlphaBlend(BLENDOP_ADD, BLEND_SRCALPHA, BLEND_INVSRCALPHA)	// �ʏ�
#define SetAlphaBlend_CsAs_Cd()		SetAlphaBlend(BLENDOP_ADD, BLEND_SRCALPHA, BLEND_ONE)			// ���Z(�A���t�@����)
#define SetAlphaBlend_Cs_Cd()		SetAlphaBlend(BLENDOP_ADD, BLEND_ONE, BLEND_ONE)				// ���Z(�A���t�@�Ȃ�)
#define SetAlphaBlend_NCsAs_Cd()	SetAlphaBlend(BLENDOP_REVSUBTRACT, BLEND_SRCALPHA, BLEND_ONE)	// ���Z(�A���t�@����)
#define SetAlphaBlend_NCs_Cd()		SetAlphaBlend(BLENDOP_REVSUBTRACT, BLEND_ONE, BLEND_ONE)		// ���Z(�A���t�@�Ȃ�)

// ���_�V�F�[�_�ݒ�
#define SetVertexShader(profile, EntryVS)	VertexShader = compile profile EntryVS()

#if 0
#define SetVertexShaderDefault(profile)		SetVertexShader(profile, VSDefault)

#define SetVertexShaderSampling(profile, num)	SetVertexShader(profile, VSSampling_##num)
#define SetVertexShaderSampling_1(profile)		SetVertexShaderSampling(profile, 1)
#define SetVertexShaderSampling_4(profile)		SetVertexShaderSampling(profile, 4)
#define SetVertexShaderSampling_8(profile)		SetVertexShaderSampling(profile, 8)
#define SetVertexShaderSampling_16(profile)		SetVertexShaderSampling(profile, 16)
#define SetVertexShaderSampling_5(profile)		SetVertexShaderSampling(profile, 5)
#define SetVertexShaderSampling_7(profile)		SetVertexShaderSampling(profile, 7)
#define SetVertexShaderSampling_9(profile)		SetVertexShaderSampling(profile, 9)
#define SetVertexShaderSampling_13(profile)		SetVertexShaderSampling(profile, 13)

#define SetVSDownScale4x4_Linear(profile)		SetVertexShaderSampling_4(profile)
#define SetVSDownScale4x4_Point(profile)		SetVertexShaderSampling_16(profile)
#define SetVSDownScale2x2_Linear(profile)		SetVertexShaderSampling_1(profile)
#define SetVSDownScale2x2_Point(profile)		SetVertexShaderSampling_4(profile)
#define SetVSDownScale3x3_Linear(profile)		SetVertexShaderSampling_4(profile)
#define SetVSDownScale3x3_Point(profile)		SetVertexShaderSampling_9(profile)
#define SetVSBloomBlurV_Linear(profile)			SetVertexShaderSampling_7(profile)	
#define SetVSBloomBlurV_Point(profile)			SetVertexShaderSampling_13(profile)
#define SetVSBloomBlurH_Linear(profile)			SetVertexShaderSampling_7(profile)
#define SetVSBloomBlurH_Point(profile)			SetVertexShaderSampling_13(profile)

#define SetVSGaussBlur5x5(profile)				SetVertexShaderSampling_13(profile)
#define SetVSRenderStar(profile)				SetVertexShaderSampling_8(profile)

#define SetVS9PointSample_Linear(profile)		SetVertexShaderSampling_5(profile)
#define SetVS9PointSample_Point(profile)		SetVertexShaderSampling_9(profile)
#endif

// �s�N�Z���V�F�[�_�ݒ�
#define SetPixelShader(profile, EntryPS)	PixelShader = compile profile EntryPS()

#endif	// #if !defined(__IZANAGI_POSTEFFECT_SHADER_DEFS_PASS_FXH__)
