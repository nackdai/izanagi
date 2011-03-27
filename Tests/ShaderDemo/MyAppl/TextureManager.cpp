#include <stdafx.h>
#include "TextureManager.h"

using namespace izanagi;

CTextureManager CTextureManager::s_cInstance;

CTextureManager::CTextureManager()
{
	m_pAllocator = IZ_NULL;
	m_pDevice = IZ_NULL;
}

CTextureManager::~CTextureManager()
{
	Clear();
}

IZ_BOOL CTextureManager::Init(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice)
{
	m_pAllocator = pAllocator;
	SAFE_REPLACE(m_pDevice, pDevice);

	return IZ_TRUE;
}

void CTextureManager::Clear()
{
	SAFE_RELEASE(m_pDevice);

	CStdList<CTexHolder>::Item* item = m_List.GetTop();
	while (item != IZ_NULL) {
		CTexHolder* p = item->GetData();

		p->Release();

		CStdList<CTexHolder>::Item* next = item->GetNext();
		item->Leave();

		item = next;

		FREE(m_pAllocator, p);
	}
}

IZ_INT CTextureManager::Read(IZ_PCSTR pszFile)
{
	IZ_BOOL result = IZ_TRUE;
	IZ_INT ret = -1;

	CTexture* pTex = m_pDevice->CreateTextureFromFile(
						pszFile,
						izanagi::E_GRAPH_PIXEL_FMT_RGBA8);
	result = (pTex != IZ_NULL);
	VGOTO(result, __EXIT__);

	CTexHolder* pHolder = (CTexHolder*)ALLOC_ZERO(m_pAllocator, sizeof(CTexHolder));
	result = (pHolder != IZ_NULL);
	VGOTO(result, __EXIT__);

	pHolder = new(pHolder) CTexHolder;

	pHolder->SetTexture(pTex);

	m_List.AddTail(pHolder->GetListItem());
	ret = m_List.GetItemNum() - 1;
	
__EXIT__:
	if (!result) {
		ret = -1;
		if (pHolder != IZ_NULL) {
			pHolder->Release();
			FREE(m_pAllocator, pHolder);
		}
		else if (pTex != IZ_NULL) {
			SAFE_RELEASE(pTex);
		}
	}
	return ret;
}

CTexture* CTextureManager::Get(IZ_UINT nIdx)
{
	IZ_UINT num = m_List.GetItemNum();
	IZ_ASSERT(nIdx < num);

	CStdList<CTexHolder>::Item* item = m_List.GetAt(nIdx);
	
	CTexture* ret = IZ_NULL;

	if (item != IZ_NULL) {
		CTexHolder* pHolder = item->GetData();
		ret = pHolder->GetTexture();
	}

	return ret;
}

IZ_BOOL CTextureManager::AllRead()
{
	static IZ_PCSTR path[] = {
		"./data/earth.bmp",
		"./data/tile.bmp",
		"./data/wall.jpg",
		"./data/stones.bmp",
		"./data/wood.jpg",
		"./data/rocks.jpg",
		"./data/saint_NM_height.tga",
		"./data/rocks_NM_height.tga",
		"./data/four_NM_height.tga",
		"./data/stones_NM_height.tga",
		"./data/wall_NM_height.tga",
	};
	IZ_C_ASSERT(COUNTOF(path) == DEMO_TEX_NUM);

	IZ_BOOL ret = IZ_FALSE;

	for (size_t i = 0; i < COUNTOF(path); i++) {
		ret = (Read(path[i]) >= 0);
		if (!ret) {
			break;
		}
	}

	if (!ret) {
		Clear();
	}

	return ret;
}
