#if !defined(__URANUS_GRAPH_PIXEL_SHADER_H__)
#define __URANUS_GRAPH_PIXEL_SHADER_H__

#include "ShaderConstTable.h"

namespace uranus {
	/**
	* �s�N�Z���V�F�[�_
	*/
	class CPixelShader : public CShaderConstTable {
		friend class CGraphicsDevice;

	private:
		// �C���X�^���X�쐬
		static CPixelShader* CreatePixelShader(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			const void* pProgram);

	protected:
		inline CPixelShader();
		inline ~CPixelShader();

		CPixelShader(const CPixelShader& rhs);
		const CPixelShader& operator=(const CPixelShader& rhs);

	protected:
		// ���
		inline void InternalRelease();

	public:
		inline D3D_PS* GetRawInterface();

	protected:
		IMemoryAllocator* m_pAllocator;

		// �{��
		D3D_PS* m_pPS;
	};

	// inline **********************************

	// �R���X�g���N�^
	CPixelShader::CPixelShader()
	{
		m_pAllocator = UN_NULL;
		m_pPS = UN_NULL;
	}

	// �f�X�g���N�^
	CPixelShader::~CPixelShader()
	{
		SAFE_RELEASE(m_pPS);
	}

	// ���
	void CPixelShader::InternalRelease()
	{
		delete this;
		if (m_pAllocator != UN_NULL) {
			m_pAllocator->Free(this);
		}
	}

	D3D_PS* CPixelShader::GetRawInterface()
	{
		return m_pPS;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_PIXEL_SHADER_H__)
