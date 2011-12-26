#include "scenegraph/msh/MshPrimitiveSet.h"
#include "scenegraph/msh/MshRenderHandler.h"
#include "izGraph.h"
#include "izIo.h"

#include "scenegraph/skl/SkeletonInstance.h"

using namespace izanagi;

CPrimitiveSet::CPrimitiveSet()
{
	m_VB = IZ_NULL;
	m_IB = IZ_NULL;

	m_PrimNum = 0;

	m_IsTransform = IZ_FALSE;
}

CPrimitiveSet::~CPrimitiveSet()
{
	SAFE_RELEASE(m_VB);
	SAFE_RELEASE(m_IB);
}

namespace {
	// Create IndexBuffer.
	CIndexBuffer* _CreateIndexBuffer(
		CGraphicsDevice* pDevice,
		const S_MSH_PRIM_SET& sMeshSubset,
		IInputStream* pIn)
	{
		// Create IndexBuffer.
		CIndexBuffer* pIB = pDevice->CreateIndexBuffer(
								sMeshSubset.numIdx,
								sMeshSubset.fmtIdx,
								E_GRAPH_RSC_TYPE_STATIC);
		IZ_BOOL ret = (pIB != IZ_NULL);
		VGOTO(ret, __EXIT__);


		void* buf = IZ_NULL;
		ret = pIB->Lock(0, 0, &buf, IZ_FALSE);
		VGOTO(ret, __EXIT__);

		IZ_UINT nSize = sMeshSubset.numIdx;
		nSize *= (sMeshSubset.fmtIdx == E_GRAPH_INDEX_BUFFER_FMT_INDEX32
					? sizeof(IZ_UINT32)
					: sizeof(IZ_UINT16));

		IZ_INPUT_READ_ASSERT(pIn, buf, 0, nSize);

		pIB->Unlock();

__EXIT__:
		if (!ret) {
			SAFE_RELEASE(pIB);
		}
		return pIB;
	}
}	// namespace

IZ_UINT8* CPrimitiveSet::Read(
	CGraphicsDevice* pDevice,
	IInputStream* pIn,
	IZ_UINT8* buf)
{
	IZ_INPUT_READ_ASSERT(pIn, &m_Info, 0, sizeof(m_Info));

	// 所属関節へのインデックス
	if (m_Info.numJoints > 0) {
		// バッファ確保
		m_Info.joints = reinterpret_cast<IZ_INT16*>(buf);
		size_t size = sizeof(IZ_UINT16) * m_Info.numJoints;

		// 読み込み
		IZ_INPUT_READ_ASSERT(pIn, m_Info.joints, 0, size);

		buf += size;
	}

	m_IB = _CreateIndexBuffer(pDevice, m_Info, pIn);
	VRETURN_NULL(m_IB != IZ_NULL);

	// Compute count of primitive.
	switch (m_Info.typePrim) {
	case E_GRAPH_PRIM_TYPE_TRIANGLESTRIP:
		m_PrimNum = m_Info.numIdx - 2;
		break;
	case E_GRAPH_PRIM_TYPE_TRIANGLEFAN:
		// TODO
		IZ_ASSERT(IZ_FALSE);
		break;
	case E_GRAPH_PRIM_TYPE_TRIANGLELIST:
		m_PrimNum = m_Info.numIdx / 3;
		break;
	default:
		IZ_ASSERT(IZ_FALSE);
		break;
	}

	return buf;
}

IZ_BOOL CPrimitiveSet::Render(
	CGraphicsDevice* pDevice,
	CSkeletonInstance* pSkeleton,
	IMshRenderHandler* pRenderHandler)
{
	if (pRenderHandler != IZ_NULL) {
		if (pSkeleton != IZ_NULL) {
			for (IZ_UINT i = 0; i < m_Info.numJoints; ++i) {
				if (m_Info.joints[i] >= 0) {
					IZ_UINT16 idx = m_Info.joints[i];
					const SMatrix* pMtx = pSkeleton->GetJointMtx(idx);

					pRenderHandler->SetJointMatrix(
						i,
						*pMtx);
				}
			}
		}

		pRenderHandler->CommitChanges();
	}

	IZ_UINT nStride = m_VB->GetStride();

	VRETURN(
		pDevice->SetVertexBuffer(
			0,
			0,
			nStride,
			m_VB));	

	VRETURN(pDevice->SetIndexBuffer(m_IB));

	IZ_ASSERT(m_PrimNum > 0);

	// NOTE
	// インデックスバッファはメッシュ単位で完結するので
	// StartIndex は 0 のまま

	IZ_UINT nVtxNum = m_Info.maxIdx - m_Info.minIdx + 1;

	VRETURN(
		pDevice->DrawIndexedPrimitive(
			m_Info.typePrim,
			0,				// BaseVertexIndex
			m_Info.minIdx,	// MinIndex
			nVtxNum,		// NumVertices
			0,				// StartIndex
			m_PrimNum));

	return IZ_TRUE;
}

IZ_BOOL CPrimitiveSet::DebugRender(
	CGraphicsDevice* pDevice,
	const CSkeletonInstance* pSkeleton,
	IMshRenderHandler* pRenderHandler)
{
	IZ_ASSERT(m_VB != IZ_NULL);

	struct SVtx {
		SVector pos;
		float nml[3];
		float uv[2];
		float idx[4];
		float weight[4];
	};

	const SMatrix* tblJointMtx[4] = {
		IZ_NULL, IZ_NULL, IZ_NULL, IZ_NULL,
	};

	for (IZ_UINT i = 0; i < m_Info.numJoints; ++i) {
		IZ_INT idx = m_Info.joints[i];
		if (idx >= 0) {
			tblJointMtx[i] = pSkeleton->GetJointMtx(idx);
		}
	}

	if (!m_IsTransform) {
		m_IsTransform = IZ_TRUE;

		SVtx* pVtx = IZ_NULL;
		VRETURN(m_VB->Lock(0, 0, (void**)&pVtx, IZ_FALSE, IZ_TRUE));

		IZ_UINT nVtxNum = m_VB->GetVtxNum();
		
		for (IZ_UINT i = 0; i < nVtxNum; ++i) {
			SVtx& sVtx = pVtx[i];

			SVector vPosTmp;
			vPosTmp.Set(0.0f, 0.0f, 0.0f, 0.0f);

			float fTotalW = 0.0f;

			for (IZ_UINT n = 0; n < 4; ++n) {
				int idx = (int)sVtx.idx[n];
				float weight = sVtx.weight[n];

				if ((idx >= 0) && (tblJointMtx[idx] != IZ_NULL)) {
					const SMatrix& mtx = *tblJointMtx[idx];

					SVector vec;
					SMatrix::Apply(vec, sVtx.pos, mtx);

					vPosTmp.x += vec.x * weight;
					vPosTmp.y += vec.y * weight;
					vPosTmp.z += vec.z * weight;
				}
			}

			sVtx.pos.x = vPosTmp.x;
			sVtx.pos.y = vPosTmp.y;
			sVtx.pos.z = vPosTmp.z;
		}
		m_VB->Unlock();
	}

	IZ_UINT nStride = m_VB->GetStride();

	VRETURN(
		pDevice->SetVertexBuffer(
			0,
			0,
			nStride,
			m_VB));	

	VRETURN(pDevice->SetIndexBuffer(m_IB));

	IZ_ASSERT(m_PrimNum > 0);

	// NOTE
	// インデックスバッファはメッシュ単位で完結するので
	// StartIndex は 0 のまま

	IZ_UINT nVtxNum = m_Info.maxIdx - m_Info.minIdx + 1;

	VRETURN(
		pDevice->DrawIndexedPrimitive(
			m_Info.typePrim,
			0,				// BaseVertexIndex
			m_Info.minIdx,	// MinIndex
			nVtxNum,		// NumVertices
			0,				// StartIndex
			m_PrimNum));

	return IZ_TRUE;
}
