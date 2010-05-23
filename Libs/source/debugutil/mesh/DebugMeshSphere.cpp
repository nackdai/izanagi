#include "debugutil/mesh/DebugMeshSphere.h"

using namespace uranus;

// インスタンス作成
CDebugMeshSphere* CDebugMeshSphere::CreateDebugMeshSphere(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice,
	UN_UINT flag,
	UN_COLOR nColor,
	UN_FLOAT fRadius,
	UN_UINT nSlices,
	UN_UINT nStacks)
{
	flag |= E_DEBUG_MESH_VTX_FORM_POS;

	nSlices = MAX(3, nSlices);
	nStacks = MAX(2, nStacks);

	void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CDebugMeshSphere));
	VRETURN_VAL(pBuf != UN_NULL, UN_NULL);

	CDebugMeshSphere* pInstance = UN_NULL;
	UN_BOOL result = UN_FALSE;

	// インスタンス作成
	pInstance = CreateMesh<CDebugMeshSphere>(pBuf, pAllocator, pDevice, flag);
	VGOTO(result != (pInstance != UN_NULL), __EXIT__);

	// 初期化
	result = pInstance->Init(
				pDevice,
				flag,
				nSlices,
				nStacks);
	VGOTO(result, __EXIT__);

	// 頂点データセット
	result = pInstance->SetVtx(
				flag,
				nColor,
				fRadius,
				nSlices,
				nStacks);
	VGOTO(result, __EXIT__);

	// 面に頂点インデックスデータセット
	result = pInstance->SetIdx(
				nSlices,
				nStacks);
	VGOTO(result, __EXIT__);

	// データをVB、IBにコピーする
	result = pInstance->CopyDataToBuffer(flag);
	VGOTO(result, __EXIT__);

__EXIT__:
	if (!result) {
		SAFE_RELEASE(pInstance);
	}
	return pInstance;
}

namespace {
	// 頂点数計算
	inline UN_UINT _ComputeVtxNum(UN_UINT nSlices, UN_UINT nStacks)
	{
		// +nSlices * 2 -> 上端、下端の２点
		UN_UINT ret = (nSlices * 2) + (nSlices + 1) * (nStacks - 1);
		return ret;
	}

	// インデックス数計算
	inline UN_UINT _ComputeIdxNum(UN_UINT nSlices, UN_UINT nStacks)
	{
		// (nSlices * (3 * 2)) * (nStacks - 2)
		//		中央部 1スライス = 1矩形 = 2三角形 = 6頂点
		//		nStacks - 2 は上下二つを除いて
		// ((nSlices * 3) * 2)
		//		上下端 スライス数の三角形 = スライス数 * 3 頂点
		//		×２は上下二つなので
		UN_UINT ret = (nSlices * (3 * 2)) * (nStacks - 2) + ((nSlices * 3) * 2);
		return ret;
	}
}	// namespace

// 初期化
UN_BOOL CDebugMeshSphere::Init(
	CGraphicsDevice* pDevice,
	UN_UINT flag,
	UN_UINT nSlices,
	UN_UINT nStacks)
{
	UN_UINT nVtxNum = _ComputeVtxNum(nSlices, nStacks);
	UN_UINT nIdxNum = _ComputeIdxNum(nSlices, nStacks);

#if 0
	E_GRAPH_INDEX_BUFFER_FMT fmt = (nIdxNum > UN_UINT16_MAX 
									? E_GRAPH_INDEX_BUFFER_FMT_INDEX32 
									: E_GRAPH_INDEX_BUFFER_FMT_INDEX16);
#else
	E_GRAPH_INDEX_BUFFER_FMT fmt = E_GRAPH_INDEX_BUFFER_FMT_INDEX32;
#endif

	VRETURN(
		CreateVB(
			flag, 
			nVtxNum));

	VRETURN(
		CreateIB(
			nIdxNum, 
			fmt));

	VRETURN(CreateVD(flag));

	m_PrimType = E_GRAPH_PRIM_TYPE_TRIANGLELIST;
	m_nPrimCnt = nIdxNum / 3;

	VRETURN(
		CreateDataBuffer(
			nVtxNum,
			nIdxNum));

	VRETURN(
		CreateDebugAxis(
			nVtxNum,
			flag));

	return UN_TRUE;
}

// 頂点データセット
UN_BOOL CDebugMeshSphere::SetVtx(
	UN_UINT flag,
	UN_COLOR nColor,
	UN_FLOAT fRadius,
	UN_UINT nSlices,
	UN_UINT nStacks)
{
	SMeshVtx* pVtx = GetVtx();

	// 上
	for (UN_UINT i = 0; i < nSlices; i++) {
		ComputeVtx(
			pVtx,
			flag,
			nColor,
			fRadius,
			UN_MATH_PI2 * i / (nSlices -1),
			0.0f);
		pVtx++;
	}

	UN_FLOAT fLatitudeStep = UN_MATH_PI / nStacks;		// 緯度ステップ
	UN_FLOAT fLongitudeStep = UN_MATH_PI2 / nSlices;	// 経度ステップ

	// 真ん中
	for (UN_UINT stack = 1; stack < nStacks; stack++) {
		UN_FLOAT fLatitude = fLatitudeStep * stack;

		for (UN_UINT slice = 0; slice < nSlices; slice++) {
			UN_FLOAT fLongitude = fLongitudeStep * slice;

			ComputeVtx(
				pVtx,
				flag,
				nColor,
				fRadius,
				fLongitude,
				fLatitude);
			pVtx++;
		}

#if 1
		// 最後にひとつ重ねる
		{
			UN_FLOAT fLongitude = 0.0f;

			ComputeVtx(
				pVtx,
				flag & (~E_DEBUG_MESH_VTX_FORM_UV),	// UVをセットさせないflag,
				nColor,
				fRadius,
				fLongitude,
				fLatitude);

			// ここでUV座標をセットする
			if (IsUV(flag)) {
				pVtx->uv[0] = 1.0f;
				pVtx->uv[1] = fLatitude / UN_MATH_PI;
			}

			pVtx++;
		}
#endif
	}

	// 下
	for (UN_UINT i = 0; i < nSlices; i++) {
		ComputeVtx(
			pVtx,
			flag,
			nColor,
			fRadius,
			UN_MATH_PI2 * i / (nSlices -1),
			UN_MATH_PI);
		pVtx++;
	}

	UN_ASSERT(CStdUtil::GetPtrAdvanced(GetVtx(), pVtx) == GetVtxNum());

	return UN_TRUE;
}

// 頂点データ計算
void CDebugMeshSphere::ComputeVtx(
	SMeshVtx* pVtx,
	UN_UINT flag,
	UN_COLOR nColor,
	UN_FLOAT fRadius,
	UN_FLOAT fLongitude,	// 経度
	UN_FLOAT fLatitude)		// 緯度
{
	UN_FLOAT fSinLat = CMath::SinF(fLatitude);
	UN_FLOAT fCosLat = CMath::CosF(fLatitude);
	UN_FLOAT fSinLong = CMath::SinF(fLongitude);
	UN_FLOAT fCosLong = CMath::CosF(fLongitude);

	SVector vNml;
	vNml.x = fSinLat * fCosLong;
	vNml.y = fCosLat;
	vNml.z = fSinLat * fSinLong;
	vNml.w = 1.0f;

	// 位置
	if (IsPos(flag)) {
		SVector vPos;
		ScaleVector(vPos, vNml, fRadius);

		pVtx->pos.v[0] = vPos.x;
		pVtx->pos.v[1] = vPos.y;
		pVtx->pos.v[2] = vPos.z;
		pVtx->pos.v[3] = 1.0f;
	}

	// 法線
	if (IsNormal(flag)) {
		pVtx->nml.v[0] = vNml.x;
		pVtx->nml.v[1] = vNml.y;
		pVtx->nml.v[2] = vNml.z;
		pVtx->nml.v[3] = 0.0f;
	}

	// 頂点カラー
	if (IsColor(flag)) {
		pVtx->clr = nColor;
	}

	// UV座標
	if (IsUV(flag)) {
		pVtx->uv[0] = fLongitude / UN_MATH_PI2;
		pVtx->uv[1] = fLatitude / UN_MATH_PI;
	}
}

// 面に頂点インデックスデータセット
UN_BOOL CDebugMeshSphere::SetIdx(
	UN_UINT nSlices,
	UN_UINT nStacks)
{
	// 頂点数
	const UN_UINT nVtxNum = GetVtxNum();

	SMeshFace* pFace = GetFace();
	VRETURN_NULL(pFace);

	// 上
	for (UN_UINT slice = 0; slice < nSlices; slice++) {
		pFace->idx[0] = slice;	// 一番上の点
		pFace->idx[1] = slice + nSlices;
		pFace->idx[2] = slice + 1 + nSlices;

		if (slice + 1 >= nSlices) {
			SetOverlapVtx(
				(slice + 1) % nSlices + nSlices,
				slice + 1 + nSlices);
		}

		BindFaceToVtx(pFace);
		pFace++;
	}

	// 真ん中
	// ex) nSlices = 4 の場合
	//	1___2___3___4___1
	//	|  /|  /|  /|  /|
	//	| / | / | / | / |
	//	|/__|/__|/__|/__|
	//  5   6   7   8   5
	//
	//  1-5-2 / 2-5-6
	//  2-6-3 / 3-6-7

	for (UN_UINT stack = 0; stack < nStacks - 2; stack++) {
		for (UN_UINT slice = 0; slice < nSlices; slice++) {
			{
				pFace->idx[0] = slice + stack * (nSlices + 1) + nSlices;
				pFace->idx[1] = slice + (stack + 1) * (nSlices + 1) + nSlices;
				pFace->idx[2] = (slice + 1) + stack * (nSlices + 1) + nSlices;

				if (slice + 1 >= nSlices) {
					SetOverlapVtx(
						(slice + 1) % nSlices + stack * (nSlices + 1) + nSlices,
						(slice + 1) + stack * (nSlices + 1) + nSlices);
				}

				BindFaceToVtx(pFace);
				pFace++;
			}

			{
				pFace->idx[0] = (slice + 1) + stack * (nSlices + 1) + nSlices;
				pFace->idx[1] = slice + (stack + 1) * (nSlices + 1) + nSlices;
				pFace->idx[2] = (slice + 1) + (stack + 1) * (nSlices + 1) + nSlices;

				if (slice + 1 >= nSlices) {
					SetOverlapVtx(
						(slice + 1) % nSlices + (stack + 1) * (nSlices + 1) + nSlices,
						(slice + 1) + (stack + 1) * (nSlices + 1) + nSlices);
				}

				BindFaceToVtx(pFace);
				pFace++;
			}

		}
	}

	// 下
	for (UN_UINT slice = 0; slice < nSlices; slice++) {
		pFace->idx[0] = nVtxNum - nSlices + slice;	// 一番下の点
		pFace->idx[1] = nVtxNum - nSlices - ((nSlices + 1) - (slice + 1));
		pFace->idx[2] = nVtxNum - nSlices - ((nSlices + 1) - slice);

		if (slice + 1 >= nSlices) {
			SetOverlapVtx(
				nVtxNum - nSlices - ((nSlices + 1) - (slice + 1) % nSlices),
				nVtxNum - nSlices - ((nSlices + 1) - (slice + 1)));
		}

		BindFaceToVtx(pFace);
		pFace++;
	}

	UN_ASSERT(CStdUtil::GetPtrAdvanced(GetFace(), pFace) == m_nPrimCnt);

	return UN_TRUE;
}
