#if !defined(__SCENE_RENDERER_H__)
#define __SCENE_RENDERER_H__

#include "MeshManager.h"
#include "shader/PhongShader.h"

class CSceneRenderer {
private:
	static CSceneRenderer s_cInstance;

public:
	static CSceneRenderer& GetInstance() { return s_cInstance; }

private:
	inline CSceneRenderer();
	~CSceneRenderer() {}

	CSceneRenderer(const CSceneRenderer& rhs);
	const CSceneRenderer& operator=(const CSceneRenderer& rhs);

public:
	// 開放
	void Release();

	// シェーダ初期化
	IZ_BOOL InitShader(
		izanagi::IMemoryAllocator* pAllocator,
		izanagi::CGraphicsDevice* pDevice,
		LPCSTR lpszName);

	// 描画
	void Render(izanagi::CGraphicsDevice* pDevice);

public:
	inline CPhongShader* GetPhongShader();

private:
	CPhongShader* m_pPhongShader;
};

// inline ********************************

// コンストラクタ
CSceneRenderer::CSceneRenderer()
{
	m_pPhongShader = IZ_NULL;
}

CPhongShader* CSceneRenderer::GetPhongShader()
{
	return m_pPhongShader;
}

#endif	// #if !defined(__SCENE_RENDERER_H__)

