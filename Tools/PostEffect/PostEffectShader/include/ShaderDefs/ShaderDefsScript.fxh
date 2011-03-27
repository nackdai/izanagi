#if !defined(__IZANAGI_POSTEFFECT_SHADER_DEFS_SCRIPT_FXH__)
#define __IZANAGI_POSTEFFECT_SHADER_DEFS_SCRIPT_FXH__

/////////////////////////////////////
// �e�N�j�b�N�E�p�X����

#define SetRenderTarget(tex)		string RenderColorTarget=#tex	// �����_�[�^�[�Q�b�g�w��
#define SetDefaultRenderTarget()	string RenderColorTarget=""		// �����_�[�^�[�Q�b�g�����ɖ߂�
#define SetClearColor(col)			int ClearColor=col			// �N���A�J���[�w��

/*************
   �ȈՐݒ�
*************/

// �V�[���J�n
#define BeginScene(tex)	SetRenderTarget(tex)

// �V�[���J�n�i�N���A�J���[�L��j
#define BeginSceneWithClearColor(tex, col)\
	SetRenderTarget(tex);\
	SetClearColor(col)

// �V�[���J�n�i�����_�[�^�[�Q�b�g�����ɖ߂��j
#define BeginSceneByDefault()	SetDefaultRenderTarget()

// �V�[���J�n�i�����_�[�^�[�Q�b�g�����ɖ߂��A�N���A�J���[�L��j
#define BeginSceneByDefaultWithClearColor(col)\
	SetDefaultRenderTarget();\
	SetClearColor(col)

#endif	// #if !defined(__IZANAGI_POSTEFFECT_SHADER_DEFS_SCRIPT_FXH__)
