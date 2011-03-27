#include "scenegraph/mdl/ModelInstance.h"
#include "scenegraph/mdl/izModel.h"
#include "scenegraph/mdl/MdlGeometry.h"
#include "scenegraph/mdl/MdlJoint.h"

using namespace izanagi;

/**
*/
CModelInstance* CModelInstance::CreateModelInstance(
	IMemoryAllocator* pAllocator,
	CModel* pMdl,
	IMdlRenderHandler* pRenderHandler)
{
	IZ_ASSERT(pAllocator != IZ_NULL);
	IZ_ASSERT(pMdl != IZ_NULL);

	size_t nSize = sizeof(CModelInstance);

	IZ_UINT8* pBuf = reinterpret_cast<IZ_UINT8*>(ALLOC_ZERO(pAllocator, nSize));
	VRETURN_NULL(pBuf != IZ_NULL);

	IZ_BOOL result = IZ_TRUE;

	CModelInstance* pInstance = new(pBuf) CModelInstance;
	{
		pBuf += sizeof(CModelInstance);

		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;

		SAFE_REPLACE(pInstance->m_pMaster, pMdl);
		SAFE_REPLACE(pInstance->m_pRenderHandler, pRenderHandler);

		if (pMdl->GetJoint() != IZ_NULL) {
			pInstance->m_pSkeleton = CMdlSkeleton::CreateMdlSkeleton(
										pAllocator,
										pMdl->GetJoint());

			result = (pInstance->m_pSkeleton != IZ_NULL);
			IZ_ASSERT(result);
		}
	}

	if (!result) {
		SAFE_RELEASE(pInstance);
	}
	return pInstance;
}

CModelInstance::CModelInstance()
{
	m_pMaster = IZ_NULL;

	m_pAllocator = IZ_NULL;

	m_pSkeleton = IZ_NULL;

	m_pRenderHandler = IZ_NULL;
}

CModelInstance::~CModelInstance()
{
	SAFE_RELEASE(m_pMaster);
	SAFE_RELEASE(m_pSkeleton);
	SAFE_RELEASE(m_pRenderHandler);
}

IZ_BOOL CModelInstance::Render()
{
	IZ_ASSERT(m_pMaster != IZ_NULL);

	CGraphicsDevice* pDevice = m_pMaster->GetGraphicsDevice();
	VRETURN(pDevice != IZ_NULL);

	CMdlGeometry* pGeom = m_pMaster->GetGeometry();
	VRETURN(pGeom != IZ_NULL);

	IZ_BOOL ret = pGeom->Render(
					pDevice,
					m_pSkeleton,
					m_pRenderHandler);
	IZ_ASSERT(ret);

	return ret;
}

void CModelInstance::Update()
{
	if (m_pSkeleton != IZ_NULL) {
		m_pSkeleton->BuildMatrix();
	}
}

void CModelInstance::ApplyAnimation(
	IZ_FLOAT fTime,
	CAnimation* pAnm)
{
	if (m_pSkeleton != IZ_NULL) {
		m_pSkeleton->ApplyAnimation(fTime, pAnm);
	}
}

void CModelInstance::ApplyAnimationByIdx(
	IZ_UINT nJointIdx,
	IZ_FLOAT fTime,
	CAnimation* pAnm)
{
	if (m_pSkeleton != IZ_NULL) {
		m_pSkeleton->ApplyAnimationByIdx(nJointIdx, fTime, pAnm);
	}
}

void CModelInstance::ApplyAnimationByName(
	IZ_PCSTR pszJointName,
	IZ_FLOAT fTime,
	CAnimation* pAnm)
{
	if (m_pSkeleton != IZ_NULL) {
		m_pSkeleton->ApplyAnimationByName(pszJointName, fTime, pAnm);
	}
}

void CModelInstance::ApplyAnimationByKey(
	IZ_UINT nJointKey,
	IZ_FLOAT fTime,
	CAnimation* pAnm)
{
	if (m_pSkeleton != IZ_NULL) {
		m_pSkeleton->ApplyAnimationByKey(nJointKey, fTime, pAnm);
	}
}
