#if !defined(__SCENE_H__)
#define __SCENE_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "Common.h"

class CCharacter;

class CScene {
public:
	CScene();
	~CScene();

public:
	IZ_BOOL Init(
		izanagi::IMemoryAllocator* allocator,
		izanagi::CGraphicsDevice* device);

	void Clear();

	void Render(
		CCharacter* character,
		izanagi::CGraphicsDevice* device);

private:
	izanagi::CShaderBasic* m_pShader;

	izanagi::CDebugMeshAxis* m_pAxis;

	izanagi::CGeometrySorter* m_GeomSorter;
	izanagi::CSceneRenderer* m_SceneRenderer;
};

#endif	// #if !defined(__CHARACETER_H__)
