#include "shader/ShaderManager.h"

using namespace izanagi;

/**
* Create instance of CShaderManager.
*/
CShaderManager* CShaderManager::CreateShaderManager(IMemoryAllocator* pAllocator)
{
	IZ_ASSERT(pAllocator != IZ_NULL);

	void* pBuf = ALLOC(pAllocator, sizeof(CShaderManager));
	VRETURN_NULL(pBuf != IZ_NULL);

	CShaderManager* pInstance = new(pBuf) CShaderManager;
	{
		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;
	}

	return pInstance;
}

CShaderManager::CShaderManager()
{
	m_pAllocator = IZ_NULL;
}

CShaderManager::~CShaderManager()
{
	ClearAll();
}

IZ_BOOL CShaderManager::Add(IShader* pShader)
{
	IZ_ASSERT(pShader != IZ_NULL);

	IZ_BOOL ret = m_ShaderHash.Add(pShader->GetHashItem());

	if (ret) {
		pShader->AddRef();
	}

	return ret;
}

void CShaderManager::ClearAll()
{
	CStdList<CShaderHashItem>::Item* pOrderItem = m_ShaderHash.GetOrderTop();

	while (pOrderItem != IZ_NULL) {
		CShaderHashItem* pHashItem = pOrderItem->GetData();
		IZ_ASSERT(pHashItem != IZ_NULL);

		IShader* pShader = pHashItem->GetData();
		SAFE_RELEASE(pShader);

		pOrderItem = pOrderItem->GetNext();
	}

	m_ShaderHash.Clear();
}

IZ_UINT CShaderManager::GetShaderNumByName(IZ_PCSTR name)
{
	IZ_UINT ret = GetShaderNumByKey(CKey::GenerateValue(name));
	return ret;
}

IZ_UINT CShaderManager::GetShaderNumByKey(IZ_UINT key)
{
	IZ_UINT ret = 0;

	CStdList<CShaderHashItem>::Item* pItem = m_ShaderHash.GetHashTop(key);

	while (pItem != IZ_NULL) {
		IShader* pShader = pItem->GetData()->GetData();
		IZ_ASSERT(pShader != IZ_NULL);

		if (pShader->GetKey() == key) {
			ret++;
		}

		pItem = pItem->GetNext();
	}

	return ret;
}

IShader* CShaderManager::GetShaderByName(IZ_PCSTR name)
{
	IShader* ret = GetShaderByKey(CKey::GenerateValue(name));
	return ret;
}

IShader* CShaderManager::GetShaderByKey(IZ_UINT key)
{
	// Return shader which is found firstly.
	IShader* ret = m_ShaderHash.FindData(key);
	return ret;
}

IShader* CShaderManager::GetShaderByAttr(
	IZ_PCSTR name,
	const CShaderAttr pAttrs[],
	IZ_UINT nAttrNum)
{
	IShader* ret = GetShaderByAttr(
					CKey::GenerateValue(name),
					pAttrs,
					nAttrNum);
	return ret;
}

IShader* CShaderManager::GetShaderByAttr(
	IZ_UINT key,
	const CShaderAttr pAttrs[],
	IZ_UINT nAttrNum)
{
	CStdList<CShaderHashItem>::Item* pItem = m_ShaderHash.GetHashTop(key);

	while (pItem != IZ_NULL) {
		IShader* pShader = pItem->GetData()->GetData();
		IZ_ASSERT(pShader != IZ_NULL);

		if (pShader->GetKey() == key) {
			IZ_BOOL bIsSame = IZ_TRUE;

			for (IZ_UINT i = 0; i < nAttrNum; i++) {
				const CShaderAttr& attr = pAttrs[i];

				if (!pShader->CmpAttr(attr)) {
					// Not match attribute.
					bIsSame = IZ_FALSE;
					break;
				}
			}

			if (bIsSame) {
				// Find shader.
				return pShader;
			}
		}

		pItem = pItem->GetNext();
	}

	return IZ_NULL;
}
