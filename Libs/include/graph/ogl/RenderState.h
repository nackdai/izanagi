#if !defined(__URANUS_GRAPH_RENDER_STATE_H__)
#define __URANUS_GRAPH_RENDER_STATE_H__

#include "graph/GraphDefs.h"

namespace uranus {
	class CGraphicsDevice;
	class CVertexBuffer;
	class CIndexBuffer;
	class CVertexShader;
	class CPixelShader;
	class CVertexDeclaration;
	class CSurface;

	enum {
		TEX_STAGE_NUM = 8,
		MAX_MRT_NUM = 2,
	};

	////////////////////////////////////////////
	/**
	* �����_�[�X�e�[�g�p�����[�^
	*/
	struct S_RENDER_STATE {
		union {
			struct {
				UN_DWORD isZWriteEnable;		// D3DRS_ZWRITEENABLE
				UN_DWORD isZTestEnable;			// D3DRS_ZENABLE
				UN_DWORD cmpZFunc;				// D3DRS_ZFUNC

				UN_DWORD isAlphaTestEnable;		// D3DRS_ALPHATESTENABLE
				UN_DWORD alphaRef;				// D3DRS_ALPHAREF
				UN_DWORD cmpAlphaFunc;			// D3DRS_ALPHAFUNC

				UN_DWORD isAlphaBlendEnable;	// D3DRS_ALPHABLENDENABLE

				// �A���t�@�u�����h����
				UN_DWORD methodAlphaBlend;

				UN_DWORD fillMode;				// D3DRS_FILLMODE
				UN_DWORD cullMode;				// D3DRS_CULLMODE

				// D3DRS_COLORWRITEENABLE
				UN_DWORD isEnableRenderRGB;	// RGB
				UN_DWORD isEnableRenderA;	// A

				UN_DWORD isScissorEnable;		// D3DRS_SCISSORTESTENABLE
			};
			UN_DWORD dwRS[E_GRAPH_RS_NUM];
		};

		SViewport vp;			// �r���[�|�[�g
		
		CIntRect rcScissor;			// �V�U�[��`

		// TODO
		// �X�e���V��

		CVertexBuffer* curVB;		// ���_�o�b�t�@
		CIndexBuffer* curIB;		// �C���f�b�N�X�o�b�t�@

		CVertexShader* curVS;		// ���_�V�F�[�_
		CPixelShader* curPS;		// �s�N�Z���V�F�[�_

		CVertexDeclaration* curVD;	// ���_�錾

		UN_DWORD curFVF;			// FVF

		// �����_�[�^�[�Q�b�g
		CSurface* curRT[MAX_MRT_NUM];
		CSurface* curDepth;
	};

	////////////////////////////////////////////
	/**
	* �����_�[�X�e�[�g
	*/
	class CRenderState : public S_RENDER_STATE {
		friend class CGraphicsDevice;

	private:
		CRenderState();
		~CRenderState();

		CRenderState(const CRenderState& rhs);
		const CRenderState& operator=(const CRenderState& rhs);

	private:
		// �����_�[�X�e�[�g���Z�b�g
		void SetRenderState(
			CGraphicsDevice* pDevice,
			E_GRAPH_RENDER_STATE nState,
			UN_DWORD val);

		// ���݂̃����_�[�X�e�[�g��ۑ�
		UN_BOOL Save();

		// �ۑ����������_�[�X�e�[�g�����ɖ߂�
		UN_BOOL Load(CGraphicsDevice* pDevice);

	private:
		// �O���t�B�b�N�X�f�o�C�X���猻�ݐݒ肳��Ă���l���擾����
		void GetParamsFromGraphicsDevice(CGraphicsDevice* pDevice);

		// �[�x
		void EnableZWrite(CGraphicsDevice* pDevice, UN_DWORD flag);			// �[�x�l�`�����ݗL���E����
		void EnableZTest(CGraphicsDevice* pDevice, UN_DWORD flag);			// �[�x�e�X�g�L���E����
		void SetZTestFunc(CGraphicsDevice* pDevice, UN_DWORD func);			// �[�x�e�X�g������@

		// �A���t�@
		void EnableAlphaTest(CGraphicsDevice* pDevice, UN_DWORD flag);			// �A���t�@�e�X�g�L���E����
		void SetAlphaTestRef(CGraphicsDevice* pDevice, UN_DWORD ref);			// �A���t�@�e�X�g��l
		void SetAlphaTestFunc(CGraphicsDevice* pDevice, UN_DWORD func);			// �A���t�@�e�X�g������@
		void EnableAlphaBlend(CGraphicsDevice* pDevice, UN_DWORD flag);			// �A���t�@�u�����h�L���E����
		void SetAlphaBlendMethod(CGraphicsDevice* pDevice, UN_DWORD method);	// �A���t�@�u�����h���@

		// �`�惂�[�h
		void SetFillMode(CGraphicsDevice* pDevice, UN_DWORD fill);			// �t�B�����[�h
		void SetCullMode(CGraphicsDevice* pDevice, UN_DWORD cull);			// �J�����O���[�h

		// �����_�[�^�[�Q�b�g�̃J���[�o�b�t�@�̕`�����ݐݒ�
		void EnableRenderColorRGB(CGraphicsDevice* pDevice, UN_DWORD enableRGB);
		void EnableRenderColorA(CGraphicsDevice* pDevice, UN_DWORD enableA);

		// �V�U�[�e�X�g
		void EnableScissorTest(CGraphicsDevice* pDevice, UN_DWORD flag);

	private:
		S_RENDER_STATE m_SaveRS;
		UN_BOOL m_bIsSavedRS;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_RENDER_STATE_H__)
