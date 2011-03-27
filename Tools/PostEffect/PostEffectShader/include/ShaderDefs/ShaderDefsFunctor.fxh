#if !defined(__IZANAGI_POSTEFFECT_SHADER_DEFS_FUNCTOR_ID_FXH__)
#define __IZANAGI_POSTEFFECT_SHADER_DEFS_FUNCTOR_ID_FXH__

////////////////////////////////////////////////////////
// �t�@���N�^�ݒ�

// �t�@���N�^���Z�b�g
#define SetFunctorName(name)	string FunctorName = name

// �t�@���N�^�����Z�b�g
#define SetFunctorArgsF_1(v0)				float FunctorArgsF = v0
#define SetFunctorArgsF_2(v0, v1)			float2 FunctorArgsF = {v0, v1};
#define SetFunctorArgsF_3(v0, v1, v2)		float3 FunctorArgsF = {v0, v1, v2};
#define SetFunctorArgsF_4(v0, v1, v2, v3)	float4 FunctorArgsF = {v0, v1, v2, v3};
#define SetFunctorArgsS_1(v0)				string FunctorArgsS = #v0
#define SetFunctorArgsS_2(v0, v1)			string FunctorArgsS = #v0","#v1
#define SetFunctorArgsS_3(v0, v1, v2)		string FunctorArgsS = #v0","#v1","#v2
#define SetFunctorArgsS_4(v0, v1, v2, v3)	string FunctorArgsS = #v0","#v1","#v2","#v3

#if 0
/********************************************
   �e�t�@���N�^ID�Z�b�g�i�v�t�@���N�^�����j
*********************************************/

// �J��Ԃ��k��
#define SetFunctorLoopDownScale(cnt)\
	SetFunctorName("PostEffectFunctorLoopDownScale");\
	SetFunctorArgsF_1(cnt)

// �e�N�X�`���؂�ւ�
#define SetFunctorSwitchTex(tex0, tex1)\
	SetFunctorName("PostEffectFunctorSwitchTex");\
	SetFunctorArgsS_2(tex0, tex1)
#endif

#if 0
////////////////////////////////////////////////////////
// �����p�X�t�@���N�^

// �����p�X�t�@���N�^��`�J�n
#define MULTI_PASS_FUNCTOR(name, functor_name)\
	technique name\
	<\
		bool IsMultiPassFunctor = true;\
		SetFunctorName(functor_name);\
	>

// �����p�X�t�@���N�^���ߍ���
#define EmbedMultiPassFunctor(name)\
	pass name\
	<\
		bool IsEmbedMultiPassFunctor = true;\
	> {}
#endif

#if 0
////////////////////////////////////////////////////////
// �p�X�O���L�q

// �J�e�S���I�ɂ����������̂��ǂ����͕�����񂪁E�E�E

// �p�X�̋L�q���e�N�j�b�N�̊O���ɋL�q���邱�ƂŃR�[�h��ǂ݂₷������̂Ɏg��

// �����p�X�p�b�N��`�J�n
#define DEFINE_PASSES(name)\
	technique name\
	<\
		bool IsDefinePasses = true;\
	>

// �����p�X�p�b�N���ߍ���
#define EmbedDefinePasses(name)\
	pass name\
	<\
		bool IsEmbedDefinePasses = true;\
	> {}
#endif

#endif	// #if !defined(__IZANAGI_POSTEFFECT_SHADER_DEFS_FUNCTOR_ID_FXH__)
