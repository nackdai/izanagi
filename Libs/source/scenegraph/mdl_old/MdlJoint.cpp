#include "scenegraph/mdl/MdlJoint.h"
#include "izIo.h"

using namespace izanagi;

CMdlJoint* CMdlJoint::CreateMdlJoint(
	IMemoryAllocator* pAllocator,
	IInputStream* pIn)
{
	S_MDL_JOINT_CHUNK_HEADER sHeader;
	IZ_INPUT_READ_VRETURN(pIn, &sHeader, 0, sizeof(sHeader));

	// Compute buffer size.
	IZ_UINT nSize = ComputeBufferSize(sHeader);

	// Allocate buffer.
	IZ_UINT8* pBuf = reinterpret_cast<IZ_UINT8*>(ALLOC_ZERO(pAllocator, nSize));
	VRETURN_NULL(pBuf != IZ_NULL);

	IZ_UINT8* pTmpBuf = pBuf;

	CMdlJoint* pInstance = new(pBuf) CMdlJoint;
	{
		pBuf += sizeof(CMdlJoint);

		pInstance->m_pAllocator = pAllocator;
		pInstance->AddRef();

		memcpy(&pInstance->m_Header, &sHeader, sizeof(sHeader));

		pBuf = pInstance->ReadJoint(pBuf, pIn);
	}

	IZ_ASSERT(CStdUtil::GetPtrDiff(pBuf, pTmpBuf) == nSize);

	return pInstance;
}

IZ_UINT CMdlJoint::ComputeBufferSize(
	const S_MDL_JOINT_CHUNK_HEADER& sHeader)
{
	IZ_UINT nSize = sizeof(CMdlJoint);
	{
		nSize += sizeof(S_MDL_JOINT) * sHeader.numJoint;
	}

	return nSize; 
}

CMdlJoint::CMdlJoint()
{
	m_pAllocator = IZ_NULL;

	FILL_ZERO(&m_Header, sizeof(m_Header));

	m_pJoint = IZ_NULL;
}

CMdlJoint::~CMdlJoint()
{
	FREE(m_pAllocator, this);
}

IZ_UINT8* CMdlJoint::ReadJoint(
	IZ_UINT8* pBuf,
	IInputStream* pIn)
{
	m_pJoint = reinterpret_cast<S_MDL_JOINT*>(pBuf);
	pBuf += sizeof(S_MDL_JOINT) * m_Header.numJoint;

	IZ_INPUT_READ_ASSERT(pIn, m_pJoint, 0, sizeof(S_MDL_JOINT) * m_Header.numJoint);

	return pBuf;
}

const S_MDL_JOINT* CMdlJoint::GetJoint(IZ_INT idx) const
{
	if (idx < 0) {
		return IZ_NULL;
	}
	IZ_ASSERT((IZ_UINT)idx < m_Header.numJoint);
	return &m_pJoint[idx];
}
