#if !defined(__CHARACETER_H__)
#define __CHARACETER_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izSceneGraph.h"

class CCharacter : public izanagi::CObject {
private:
	static CCharacter* sInstance;

public:
	static CCharacter* GetInstance() { return sInstance; }

	static CCharacter* CreateCharacter(
		izanagi::IMemoryAllocator* allocator,
		izanagi::graph::CGraphicsDevice* device);

private:
	CCharacter();
	~CCharacter();

private:
	void InternalRelease();

	IZ_BOOL Init();

public:
	void Update(
		IZ_FLOAT time,
		izanagi::IAnimation* animation);

	void SetShader(izanagi::IShader* shader);

	izanagi::CModel* GetMdl() { return m_pMdl; }

private:
	izanagi::IMemoryAllocator* m_Allocator;
	izanagi::graph::CGraphicsDevice* m_Device;

	izanagi::graph::CTexture* m_pTex[2];
	izanagi::CModel* m_pMdl;
	izanagi::CMesh* m_pMsh;
	izanagi::CSkeleton* m_pSkl;
	izanagi::CMaterial* m_pMtrl[2];
};

#endif	// #if !defined(__CHARACETER_H__)
