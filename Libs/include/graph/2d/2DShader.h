#if !defined(__URANUS_GRAPH_2D_SHADER_H__)
#define __URANUS_GRAPH_2D_SHADER_H__

#include "std/StdObject.h"
#include "graph/GraphDefs.h"

namespace uranus {
	class CVertexShader;
	class CPixelShader;
	class CGraphicsDevice;

	// 2D�`��p�V�F�[�_
	class C2DShader : public CObject {
		friend class C2DRenderer;

	private:
		// �C���X�^���X�쐬
		static C2DShader* Create2DShader(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice);

	private:
		C2DShader();
		~C2DShader();

		C2DShader(const C2DShader& rhs);
		const C2DShader& operator=(const C2DShader& rhs);

	private:
		// �J��
		inline void InternalRelease();

		// �V�F�[�_�쐬
		UN_BOOL CreateShader(CGraphicsDevice* pDevice);

		// �V�F�[�_�Z�b�g
		UN_BOOL SetShader(CGraphicsDevice* pDevice);

		// �V�F�[�_�p�����[�^�Z�b�g
		UN_BOOL SetShaderParams(CGraphicsDevice* pDevice);

	private:
		// �`��ݒ���Z�b�g
		inline void SetRenderOp(E_GRAPH_2D_RENDER_OP nOp);

		// �`��ݒ���擾
		inline E_GRAPH_2D_RENDER_OP GetRenderOp() const;

	private:
		// �V�F�[�_�p�����[�^
		enum {
			VTX_PARAM_INV_SCREEN,		// �X�N���[���T�C�Y�̋t��

			VTX_PARAM_NUM,
		};

	private:
		IMemoryAllocator* m_pAllocator;

		CVertexShader* m_pVS;
		CPixelShader* m_pPS[E_GRAPH_2D_RENDER_OP_NUM];

		// �`��ݒ�
		E_GRAPH_2D_RENDER_OP m_nOp;

		// �V�F�[�_�p�����[�^�n���h��
		SHADER_PARAM_HANDLE m_hVtxParam[VTX_PARAM_NUM];
	};

	// �J��
	void C2DShader::InternalRelease()
	{
		delete this;
		if (m_pAllocator != UN_NULL) {
			m_pAllocator->Free(this);
		}
	}

	// �`��ݒ���Z�b�g
	void C2DShader::SetRenderOp(E_GRAPH_2D_RENDER_OP nOp)
	{
		m_nOp = nOp;
	}

	// �`��ݒ���擾
	E_GRAPH_2D_RENDER_OP C2DShader::GetRenderOp() const
	{
		return m_nOp;
	}

}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_2D_SHADER_H__)
