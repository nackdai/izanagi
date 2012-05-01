#include "scenegraph/skl/izSkeleton.h"
#include "izIo.h"

using namespace izanagi;

CSkeleton* CSkeleton::CreateSkeleton(
	IMemoryAllocator* pAllocator,
	IInputStream* pIn)
{
	S_SKL_HEADER sHeader;
	IZ_INPUT_READ_VRETURN(pIn, &sHeader, 0, sizeof(sHeader));

	// Compute buffer size.
	IZ_UINT nSize = ComputeBufferSize(sHeader);

	// Allocate buffer.
	IZ_UINT8* pBuf = reinterpret_cast<IZ_UINT8*>(ALLOC_ZERO(pAllocator, nSize));
	VRETURN_NULL(pBuf != IZ_NULL);

	IZ_UINT8* pTmpBuf = pBuf;

	CSkeleton* pInstance = new(pBuf) CSkeleton;
	{
		pBuf += sizeof(CSkeleton);

		pInstance->m_Allocator = pAllocator;
		pInstance->AddRef();

		pInstance->m_nJointNum = sHeader.numJoint;

		pBuf = pInstance->ReadJoint(pBuf, pIn);
	}

	IZ_ASSERT(CStdUtil::GetPtrDistance(pBuf, pTmpBuf) == nSize);

	return pInstance;
}

IZ_UINT CSkeleton::ComputeBufferSize(
	const S_SKL_HEADER& sHeader)
{
	IZ_UINT nSize = sizeof(CSkeleton);
	{
		nSize += sizeof(S_SKL_JOINT) * sHeader.numJoint;
	}

	return nSize; 
}

CSkeleton::CSkeleton()
{
	m_Allocator = IZ_NULL;

	m_nJointNum = 0;
	m_pJoint = IZ_NULL;
}

CSkeleton::~CSkeleton()
{
	FREE(m_Allocator, this);
}

IZ_UINT8* CSkeleton::ReadJoint(
	IZ_UINT8* pBuf,
	IInputStream* pIn)
{
	m_pJoint = reinterpret_cast<S_SKL_JOINT*>(pBuf);
	pBuf += sizeof(S_SKL_JOINT) * m_nJointNum;

	IZ_INPUT_READ_ASSERT(pIn, m_pJoint, 0, sizeof(S_SKL_JOINT) * m_nJointNum);

	return pBuf;
}

const S_SKL_JOINT* CSkeleton::GetJoint(IZ_INT idx) const
{
	if (idx < 0) {
		return IZ_NULL;
	}
	IZ_ASSERT((IZ_UINT)idx < m_nJointNum);
	return &m_pJoint[idx];
}

S_SKL_JOINT* CSkeleton::GetJoint(IZ_INT idx)
{
	if (idx < 0) {
		return IZ_NULL;
	}
	IZ_ASSERT((IZ_UINT)idx < m_nJointNum);
	return &m_pJoint[idx];
}
