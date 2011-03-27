#include "scenegraph/scene/Scene.h"
#include "scenegraph/mtrl/izMaterial.h"
#include "izGraph.h"

using namespace izanagi;

CScene* CScene::CreateScene(IMemoryAllocator* pAllocator)
{
	void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CScene));
	VRETURN_NULL(pBuf != IZ_NULL);

	CScene* pInstance = new(pBuf) CScene;
	{
		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;
	}

	return pInstance;
}

CScene::CScene()
{
	m_pAllocator = IZ_NULL;
	m_pSceneParam = IZ_NULL;
}

CScene::~CScene()
{
	SAFE_RELEASE(m_pSceneParam);
}

template <typename _T>
void CScene::Set(
	CStdHash<CKey, CHashElem<_T>, HASH_SIZE>& cHash, 
	const CKey& cKey, 
	_T* pData)
{
	// TODO
	CHashElem<_T>* pHashElem = CHashElem<_T>::CreateHashElem(m_pAllocator, cKey, pData);
	IZ_ASSERT(pHashElem != IZ_NULL);

	//pHashElem->m_HashItem.Init(cKey, p);

	//cHash.Add(pHashElem->GetHashItem());
}

template <typename _T>
_T* CScene::Get(
	CStdHash<CKey, CHashElem<_T>, HASH_SIZE>& cHash, 
	const CKey& cKey)
{
	CHashElem<_T>* pHashElem = cHash.FindData(cKey);

	_T* ret = (pHashElem != IZ_NULL
				? pHashElem->GetData()
				: IZ_NULL);

	return ret;
}

template <typename _T>
IZ_BOOL CScene::Remove(
	CStdHash<CKey, CHashElem<_T>, HASH_SIZE>& cHash, 
	const CKey& cKey)
{
	CHashElem<_T>* pHashElem = cHash.FindData(cKey);

	IZ_BOOL ret = (pHashElem != IZ_NULL);
	if (ret) {
		CHashElem<_T>::DeleteHashElem(
			m_pAllocator,
			pHashElem);
	}

	return ret;
}

template <typename _T>
IZ_UINT CScene::GetNum(const CStdHash<CKey, CHashElem<_T>, HASH_SIZE>& cHash) const
{
	IZ_UINT ret = cHash.GetDataNum();
	return ret;
}
