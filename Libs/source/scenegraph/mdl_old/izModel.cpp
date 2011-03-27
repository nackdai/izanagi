#include "scenegraph/mdl/izModel.h"
#include "scenegraph/mdl/MdlGeometry.h"
#include "scenegraph/mdl/MdlJoint.h"
#include "izIo.h"

using namespace izanagi;

CModel* CModel::CreateModel(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice,
	IInputStream* pIn)
{
	IZ_ASSERT_NULL(pAllocator);
	IZ_ASSERT_NULL(pDevice);
	IZ_ASSERT_NULL(pIn);

	IZ_BOOL result = IZ_FALSE;

	void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CModel));
	VRETURN(pBuf != IZ_NULL);

	CModel* pInstance = new(pBuf) CModel;
	{
		pInstance->AddRef();
		
		pInstance->m_pAllocator = pAllocator;
		SAFE_REPLACE(pInstance->m_pDevice, pDevice);

		// Read file header.
		result = IZ_INPUT_READ(
					pIn,
					&pInstance->m_sHeader,
					0,
					sizeof(S_MDL_HEADER));
		VGOTO(result, __EXIT__);

		// TODO
		// Check magic number...
	}

	result = pInstance->Read(
				pAllocator,
				pIn);
	VGOTO(result, __EXIT__);
	
__EXIT__:
	if (!result) {
		SAFE_RELEASE(pInstance);
	}
	return pInstance;
}

CModel::CModel()
{
	m_pAllocator = IZ_NULL;
	m_pDevice = IZ_NULL;

	m_pGeom = IZ_NULL; 
	m_pJoint = IZ_NULL;
}

CModel::~CModel()
{
	SAFE_RELEASE(m_pDevice);

	SAFE_RELEASE(m_pGeom);
	SAFE_DELETE(m_pJoint);
}

IZ_BOOL CModel::Read(
	IMemoryAllocator* pAllocator,
	IInputStream* pIn)
{
	IZ_BOOL result = IZ_FALSE;
	S_MDL_CHUNK_HEADER sChunkHeader;

	IZ_BOOL bIsEndRead = IZ_FALSE;

	for (;;) {
		// Read chunk header.
		IZ_INPUT_READ_VRETURN(pIn, &sChunkHeader, 0, sizeof(sChunkHeader));

		switch (sChunkHeader.magicChunk) {
		case MDL_CHUNK_MAGIC_GEOM:
			// Geometry chunk.
			m_pGeom = CMdlGeometry::CreateMdlGeometry(
							m_pAllocator, 
							m_pDevice, 
							pIn);
			result = (m_pGeom != IZ_NULL);
			break;
		case MDL_CHUNK_MAGIC_JOINT:
			// Joint chunuk.
			m_pJoint = CMdlJoint::CreateMdlJoint(
								pAllocator,
								pIn);
			result = (m_pJoint != IZ_NULL);
			break;
		case MDL_CHUNK_MAGIC_TERMINATE:
			bIsEndRead = IZ_TRUE;
			break;
		default:
			IZ_ASSERT(IZ_FALSE);
			break;
		}

		if (bIsEndRead) {
			break;
		}

		VRETURN(result);
	}

	return IZ_TRUE;
}
