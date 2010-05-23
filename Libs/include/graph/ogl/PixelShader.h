#if !defined(__URANUS_GRAPH_PIXEL_SHADER_H__)
#define __URANUS_GRAPH_PIXEL_SHADER_H__

#include "ShaderConstTable.h"

namespace uranus {
	/**
	* ピクセルシェーダ
	*/
	class CPixelShader : public CShaderConstTable {
		friend class CGraphicsDevice;

	private:
		// インスタンス作成
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
		// 解放
		inline void InternalRelease();

	public:
		inline D3D_PS* GetRawInterface();

	protected:
		IMemoryAllocator* m_pAllocator;

		// 本体
		D3D_PS* m_pPS;
	};

	// inline **********************************

	// コンストラクタ
	CPixelShader::CPixelShader()
	{
		m_pAllocator = UN_NULL;
		m_pPS = UN_NULL;
	}

	// デストラクタ
	CPixelShader::~CPixelShader()
	{
		SAFE_RELEASE(m_pPS);
	}

	// 解放
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
