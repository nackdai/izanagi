#if !defined(__ENV_BOX_H__)
#define __ENV_BOX_H__

#include "izGraph.h"
#include "shader/EnvBoxShader.h"

namespace izanagi {
	class IMemoryAllocator;
}

class CEnvBox {
protected:
	static CEnvBox s_cInstance;

public:
	static CEnvBox& GetInstance() { return s_cInstance; }

protected:
	CEnvBox();
	~CEnvBox();

	CEnvBox(const CEnvBox& rhs);
	const CEnvBox& operator=(const CEnvBox& rhs);

public:
	IZ_BOOL Init(
		izanagi::IMemoryAllocator* pAllocator,
		izanagi::CGraphicsDevice* pDevice,
		LPCSTR lpszCubeTex,
		LPCSTR lpszShader);

	IZ_BOOL InitShader(
		izanagi::IMemoryAllocator* pAllocator,
		izanagi::CGraphicsDevice* pDevice,
		LPCSTR lpszShader);

	void Release();

	void Render(izanagi::CGraphicsDevice* pDevice);

public:
	CEnvBoxShader* GetEnvBoxShader() { return m_pShader; }

protected:
	enum {
		VTX_NUM = 8,
		IDX_NUM = 10 + 1 + 5 + 5,
	};

protected:
	struct SVertex {
		IZ_FLOAT pos[3];
	};

protected:
	izanagi::CVertexBuffer* m_pVB;
	izanagi::CIndexBuffer* m_pIB;
	izanagi::CVertexDeclaration* m_pVD;

	izanagi::CCubeTexture* m_pTex;

	CEnvBoxShader* m_pShader;
};

#endif	// #if !defined(__ENV_BOX_H__)
