#if !defined(__IZANAGI_GRAPH_VERTEX_SHADER_H__)
#define __IZANAGI_GRAPH_VERTEX_SHADER_H__

#include "ShaderConstTable.h"

namespace izanagi {
	/**
	* 頂点シェーダ
	*/
	class CVertexShader : public CShaderConstTable {
		friend class CGraphicsDevice;

	private:
		// インスタンス作成
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
		// 解放
		inline void InternalRelease();

	public:
		inline D3D_VS* GetRawInterface();

	protected:
		IMemoryAllocator* m_pAllocator;

		// 本体
		D3D_VS* m_pVS;
	};

	// inline **********************************

	// コンストラクタ
	CVertexShader::CVertexShader()
	{
		m_pAllocator = IZ_NULL;
		m_pVS = IZ_NULL;
	}

	// デストラクタ
	CVertexShader::~CVertexShader()
	{
		SAFE_RELEASE(m_pVS);
	}

	// 解放
	void CVertexShader::InternalRelease()
	{
		delete this;
		if (m_pAllocator != IZ_NULL) {
			m_pAllocator->Free(this);
		}
	}

	D3D_VS* CVertexShader::GetRawInterface()
	{
		return m_pVS;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_VERTEX_SHADER_H__)
