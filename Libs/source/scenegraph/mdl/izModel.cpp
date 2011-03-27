#include "scenegraph/mdl/izModel.h"
#include "scenegraph/mdl/MDLFormat.h"
#include "izIo.h"

#include "scenegraph/msh/izMesh.h"
#include "scenegraph/skl/izSkeleton.h"

using namespace izanagi;

CModel* CModel::CreateModel(
	IMemoryAllocator* pAllocator,
	CMeshInstance* pMesh,
	CSkeletonInstance* pSkl,
	IMshRenderHandler* pRenderHandler)
{
	void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CModel));
	VRETURN_NULL(pBuf != IZ_NULL);

	CModel* pInstance = new(pBuf) CModel;
	{
		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;

		SAFE_REPLACE(pInstance->m_pMesh, pMesh);
		SAFE_REPLACE(pInstance->m_pSkeleton, pSkl);
		SAFE_REPLACE(pInstance->m_pRenderHandler, pRenderHandler);
	}

	return pInstance;
}

CModel* CModel::CreateModel(
	IMemoryAllocator* pAllocator,
	CMesh* pMesh,
	CSkeleton* pSkl,
	IMshRenderHandler* pRenderHandler)
{
	IZ_BOOL result = IZ_TRUE;

	CMeshInstance* pMeshInstance = IZ_NULL;
	CSkeletonInstance* pSklInstance = IZ_NULL;
	
	pMeshInstance = CMeshInstance::CreateMeshInstance(pAllocator, pMesh);
	VGOTO(result = (pMeshInstance != IZ_NULL), __EXIT__);

	pSklInstance = CSkeletonInstance::CreateSkeletonInstance(pAllocator, pSkl);
	VGOTO(result = (pSklInstance != IZ_NULL), __EXIT__);

	CModel* pMdl = CModel::CreateModel(
					pAllocator,
					pMeshInstance,
					pSklInstance,
					pRenderHandler);
	VGOTO(result = (pMdl != IZ_NULL), __EXIT__);

__EXIT__:
	SAFE_RELEASE(pMeshInstance);
	SAFE_RELEASE(pSklInstance);

	if (!result) {
		SAFE_RELEASE(pMdl);
	}

	return pMdl;
}

CModel* CModel::CreateModel(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice,
	IInputStream* pIn,
	IMshRenderHandler* pRenderHandler)
{
	S_MDL_HEADER sHeader;
	IZ_INPUT_READ_VRETURN(pIn, &sHeader, 0, sizeof(sHeader));

	// TODO
	// Check version, magic number...

	CModel* pInstance = IZ_NULL;
	IZ_BOOL result = IZ_FALSE;

	CMesh* pMesh = IZ_NULL;
	CSkeleton* pSkl = IZ_NULL;

	S_MDL_CHUNK_HEADER sChunkHeader;
	IZ_BOOL bIsEOF = IZ_FALSE;

	for (;;) {
		// Read chunk header.
		IZ_INPUT_READ_VRETURN(pIn, &sChunkHeader, 0, sizeof(sChunkHeader));

		switch (sChunkHeader.magicChunk) {
		case MDL_CHUNK_MAGIC_MESH:
			pMesh = CMesh::CreateMesh(pAllocator, pDevice, pIn);
			result = (pMesh != IZ_NULL);
			break;
		case MDL_CHUNK_MAGIC_SKELETON:
			pSkl = CSkeleton::CreateSkeleton(pAllocator, pIn);
			result = (pSkl != IZ_NULL);
			break;
		case MDL_CHUNK_MAGIC_TERMINATE:
			bIsEOF = IZ_TRUE;
			break;
		default:
			IZ_ASSERT(IZ_FALSE);
			break;
		}

		if (!result) {
			IZ_ASSERT(IZ_FALSE);
			break;
		}

		if (bIsEOF) {
			break;
		}
	}

	if (result) {
		pInstance = CreateModel(
						pAllocator,
						pMesh,
						pSkl,
						pRenderHandler);
		IZ_ASSERT(pInstance != IZ_NULL);
	}

	SAFE_RELEASE(pMesh);
	SAFE_RELEASE(pSkl);

	return pInstance;
}

CModel::CModel()
{
	m_pAllocator = IZ_NULL;

	m_pMesh = IZ_NULL;
	m_pSkeleton = IZ_NULL;

	m_pRenderHandler = IZ_NULL;
}

CModel::~CModel()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pSkeleton);
	SAFE_RELEASE(m_pRenderHandler);
}

IZ_BOOL CModel::Render()
{
	IZ_ASSERT(m_pMesh != IZ_NULL);

	IZ_BOOL ret = m_pMesh->Render(
					m_pSkeleton,
					m_pRenderHandler);

	return ret;
}

void CModel::Update()
{
	if (m_pSkeleton != IZ_NULL) {
		m_pSkeleton->BuildMatrix();

		if (m_pMesh != IZ_NULL) {
			m_pMesh->ApplySkeleton(m_pSkeleton);
		}
	}
}

void CModel::ApplyAnimation(
	IZ_FLOAT fTime,
	CAnimation* pAnm)
{
	if (m_pSkeleton != IZ_NULL) {
		m_pSkeleton->ApplyAnimation(fTime, pAnm);
	}
}

void CModel::ApplyAnimationByIdx(
	IZ_UINT nJointIdx,
	IZ_FLOAT fTime,
	CAnimation* pAnm)
{
	if (m_pSkeleton != IZ_NULL) {
		m_pSkeleton->ApplyAnimationByIdx(nJointIdx, fTime, pAnm);
	}
}

void CModel::ApplyAnimationByName(
	IZ_PCSTR pszJointName,
	IZ_FLOAT fTime,
	CAnimation* pAnm)
{
	if (m_pSkeleton != IZ_NULL) {
		m_pSkeleton->ApplyAnimationByName(pszJointName, fTime, pAnm);
	}
}

void CModel::ApplyAnimationByKey(
	IZ_UINT nJointKey,
	IZ_FLOAT fTime,
	CAnimation* pAnm)
{
	if (m_pSkeleton != IZ_NULL) {
		m_pSkeleton->ApplyAnimationByKey(nJointKey, fTime, pAnm);
	}
}
