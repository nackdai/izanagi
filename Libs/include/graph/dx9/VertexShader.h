#if !defined(__URANUS_GRAPH_VERTEX_SHADER_H__)
#define __URANUS_GRAPH_VERTEX_SHADER_H__

#include "ShaderConstTable.h"

namespace uranus {
	/**
	* ���_�V�F�[�_
	*/
	class CVertexShader : public CShaderConstTable {
		friend class CGraphicsDevice;

	private:
		// �C���X�^���X�쐬
		static CVertexShader* CreateVertexShader(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			const void* pProgram);

	protected:
		inline CVertexShader();
		inline ~CVertexShader();

		CVertexShader(const CVertexShader& rhs);
		const CVertexShader& operator=(const CVertexShader& rhs);

	protected:
		// ���
		inline void InternalRelease();

	public:
		inline D3D_VS* GetRawInterface();

	protected:
		IMemoryAllocator* m_pAllocator;

		// �{��
		D3D_VS* m_pVS;
	};

	// inline **********************************

	// �R���X�g���N�^
	CVertexShader::CVertexShader()
	{
		m_pAllocator = UN_NULL;
		m_pVS = UN_NULL;
	}

	// �f�X�g���N�^
	CVertexShader::~CVertexShader()
	{
		SAFE_RELEASE(m_pVS);
	}

	// ���
	void CVertexShader::InternalRelease()
	{
		delete this;
		if (m_pAllocator != UN_NULL) {
			m_pAllocator->Free(this);
		}
	}

	D3D_VS* CVertexShader::GetRawInterface()
	{
		return m_pVS;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_VERTEX_SHADER_H__)
