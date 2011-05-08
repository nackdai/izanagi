#include "scenegraph/scene/GeometrySorter.h"
#include "scenegraph/msh/MeshInstance.h"
#include "scenegraph/mdl/izModel.h"

using namespace izanagi;

///////////////////////////////////

// Add mesh to list.
IZ_BOOL CGeometrySorter::CZList::Add(
	IZ_FLOAT fAddZ,
	CMeshSetInstance* pAddMesh)
{
	IZ_BOOL ret = IZ_FALSE;

	CStdList<CMeshSetInstance>::Item* p = m_List.GetTop();
	if (p == IZ_NULL) {
		// List is empty.
		ret = m_List.AddTop(pAddMesh->GetListItem());
		return ret;
	}

	// Sort by Z value.
	while (p != IZ_NULL) {
		const CMeshSetInstance* pMesh = p->GetData();

		IZ_FLOAT fZ = pMesh->GetZ();

		if (fAddZ >= fZ) {
			ret = pAddMesh->GetListItem()->JoinAfter(p);
			break;
		}

		p = p->GetNext();
	}

	if (!ret) {
		// Z value is larger than all items.
		ret = m_List.AddTail(pAddMesh->GetListItem());
	}

	IZ_ASSERT(ret);
	return ret;
}

///////////////////////////////////

// インスタンス作成
CGeometrySorter* CGeometrySorter::CreateGeometrySorter(
	IMemoryAllocator* pAllocator,
	IZ_UINT nZLevel)
{
	IZ_ASSERT(pAllocator != IZ_NULL);
	IZ_ASSERT(nZLevel > 0);

	size_t nInstanceSize = sizeof(CGeometrySorter);
	size_t size = nInstanceSize;
	size += sizeof(CZList*) * nZLevel * 2;
	size += sizeof(CZList) * nZLevel * 2;
	
	IZ_UINT8* buf = reinterpret_cast<IZ_UINT8*>(ALLOC_ZERO(pAllocator, size));
	VRETURN_NULL(buf != IZ_NULL);

	// 先頭位置を覚えておく
	IZ_UINT8* top = buf;

	CGeometrySorter* pInstance = new(buf) CGeometrySorter;
	{
		buf += nInstanceSize;

		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;

		pInstance->m_nZLevvel = nZLevel;

		pInstance->m_Opaque = reinterpret_cast<CZList**>(buf);
		buf += sizeof(CZList*) * nZLevel;

		pInstance->m_Translucent = reinterpret_cast<CZList**>(buf);
		buf += sizeof(CZList*) * nZLevel;

		for (IZ_UINT i = 0; i < nZLevel; i++) {
			pInstance->m_Opaque[i] = new(buf) CZList;
			buf += sizeof(CZList);

			pInstance->m_Translucent[i] = new(buf) CZList;
			buf += sizeof(CZList);
		}
	}

	IZ_ASSERT(CStdUtil::GetPtrDistance(buf, top) == size);
	
	return pInstance;
}

CGeometrySorter::CGeometrySorter()
{
	m_pAllocator = IZ_NULL;

	m_nZLevvel = 0;
	m_Opaque = IZ_NULL;
	m_Translucent = IZ_NULL;
}

CGeometrySorter::~CGeometrySorter()
{
	Clear();
}

// モデル登録
IZ_BOOL CGeometrySorter::Register(
	IZ_FLOAT z,
	CModel* pMdl,
	E_SCENE_REGISTER_TYPE type)
{
	IZ_UINT level = static_cast<IZ_UINT>(z * m_nZLevvel);
	return RegisterInternal(z, level, pMdl, type);
}

// 描画
IZ_BOOL CGeometrySorter::Render(
	CGraphicsDevice* pDevice,
	CScene* pScene,
	IMshRenderHandler* pRenderHandler)
{
	// Opaque
	for (IZ_UINT i = 0; i < m_nZLevvel; i++) {
		VRETURN(
			RenderInternal(
				pDevice, 
				pScene, 
				pRenderHandler, 
				*m_Opaque[i], 
				ERenderOrder_Forward));
	}

	// Tranlucent
	for (IZ_UINT i = 0; i < m_nZLevvel; i++) {
		VRETURN(
			RenderInternal(
				pDevice, 
				pScene, 
				pRenderHandler, 
				*m_Translucent[i], 
				ERenderOrder_Reverse));
	}

	return IZ_TRUE;
}

// 登録内部処理
IZ_BOOL CGeometrySorter::RegisterInternal(
	IZ_FLOAT z,
	IZ_UINT level,
	CModel* pMdl,
	E_SCENE_REGISTER_TYPE type)
{
	IZ_ASSERT(level < m_nZLevvel);

	IZ_BOOL ret = IZ_TRUE;

	IZ_UINT nMshSetNum = pMdl->GetMeshSetNum();

	for (IZ_UINT i = 0; i < nMshSetNum; i++) {
		CMeshSetInstance* pMesh = pMdl->GetMeshSet(i);

		// TODO
		pMesh->SetZ(z);

		if (type == E_SCENE_REGISTER_TYPE_NORMAL) {
			type = (pMesh->IsTranslucent()
					? E_SCENE_REGISTER_TYPE_TRANSLUCENT
					: E_SCENE_REGISTER_TYPE_OPACITY);
		}

		switch (type) {
		case E_SCENE_REGISTER_TYPE_OPACITY:
			ret = m_Opaque[level]->Add(z, pMesh);
			break;
		case E_SCENE_REGISTER_TYPE_TRANSLUCENT:
			ret = m_Translucent[level]->Add(z, pMesh);
			break;
		}

		VRETURN(ret);
	}

	return ret;
}

// 描画内部処理
IZ_BOOL CGeometrySorter::RenderInternal(
	CGraphicsDevice* pDevice,
	CScene* pScene,
	IMshRenderHandler* pRenderHandler,
	CZList& cList,
	ERenderOrder nOrder)
{
	// Get top or tail of ZList.
	CStdList<CMeshSetInstance>::Item* pItem = (nOrder == ERenderOrder_Forward
												? cList.GetList().GetTop()
												: cList.GetList().GetTail());

	for (; pItem != IZ_NULL;) {
		CMeshSetInstance* pMesh = pItem->GetData();
		IZ_ASSERT(pMesh != IZ_NULL);

		IZ_UINT nPassNum = pScene->BeginRender(pMesh);

		// Keep loop top.
		CStdList<CMeshSetInstance>::Item* pItemBegin = pItem;

		for (IZ_UINT nPass = 0; nPass < nPassNum; nPass++) {
			while (pItem != IZ_NULL) {
				pMesh = pItem->GetData();

				if (pScene->IterRender(
						pDevice, 
						nPass, 
						pMesh,
						pRenderHandler))
				{
					pItem = (nOrder == ERenderOrder_Forward
								? pItem->GetNext()
								: pItem->GetPrev());
				}
				else {
					//if (!result && (nPass == nPassNum - 1)) {
					if (nPass == nPassNum - 1) {
						// Change shader.
						VRETURN(pScene->EndRender());
						break;
					}

					// Return to loop top.
					pItem = pItemBegin;
				}
			}
		}
	}

	return IZ_TRUE;
}
