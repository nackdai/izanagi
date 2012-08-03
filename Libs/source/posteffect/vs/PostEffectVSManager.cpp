#include "posteffect/vs/PostEffectVSManager.h"
#include "posteffect/PESFormat.h"
#include "posteffect/vs/PostEffectVS.h"
#include "posteffect/vs/PostEffectVSSampling.h"

using namespace izanagi;

// 頂点シェーダ作成
IZ_BOOL CPostEffectVSManager::CreateVS(
	IZ_UINT nVsProgramNum,
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice,
	IZ_UINT8* pReadBuffer,
	IInputStream* in)
{
	IZ_BOOL ret = IZ_FALSE;

	// 頂点プログラムヘッダ
	S_PES_VS_HEADER sPesVsHeader;

	for (IZ_UINT i = 0; i < nVsProgramNum; ++i) {
		// 頂点プログラムヘッダ読み取り
		IZ_INPUT_READ_VRETURN(in, (IZ_UINT8*)&sPesVsHeader, 0, sizeof(sPesVsHeader));

		// 頂点プログラム読み取り
		IZ_INPUT_READ_VRETURN(in, pReadBuffer, 0, sPesVsHeader.sizeProgram);

		// 頂点シェーダ作成
		ret = CreateVS(
				sPesVsHeader.type,
				pAllocator,
				pDevice,
				pReadBuffer);
		if (!ret) {
			//IZ_ASSERT(IZ_FALSE);
			break;
		}
	}

	return ret;
}

namespace {
	template <class _T>
	CPostEffectVS* _CreateVS(
		IMemoryAllocator* pAllocator,
		CGraphicsDevice* pDevice,
		IZ_UINT8* pProgram)
	{
		_T* p = CPostEffectVS::Create<_T>(
					pAllocator,
					pDevice,
					pProgram);
		return p;
	}
}	// namespace

// 頂点シェーダ作成
IZ_BOOL CPostEffectVSManager::CreateVS(
	E_POSTEFFECT_VTX_SHADER type,
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice,
	IZ_UINT8* pProgram)
{
	VRETURN(type < E_POSTEFFECT_VTX_SHADER_NUM);

	if (m_pVS[type] != IZ_NULL) {
		// 既に作成済み
		return IZ_TRUE;
	}

	typedef CPostEffectVS* (*CreateFunc)(IMemoryAllocator*, CGraphicsDevice*, IZ_UINT8*);
	static CreateFunc func[] = {
		_CreateVS<CPostEffectVSSampling_1>,
		_CreateVS<CPostEffectVSSampling_4>,
		_CreateVS<CPostEffectVSSampling_8>,
		_CreateVS<CPostEffectVSSampling_16>,
		_CreateVS<CPostEffectVSSampling_5>,
		_CreateVS<CPostEffectVSSampling_7>,
		_CreateVS<CPostEffectVSSampling_9>,
		_CreateVS<CPostEffectVSSampling_13>,
	};

	IZ_C_ASSERT(COUNTOF(func) == E_POSTEFFECT_VTX_SHADER_NUM);

	m_pVS[type] = (*func[type])(pAllocator, pDevice, pProgram);

	IZ_BOOL result = (m_pVS[type] != IZ_NULL);
	if (result) {
		m_pVS[type]->SetVertexBuffer(m_pVertexBuffer);
		m_pVS[type]->SetVertexDeclaration(m_pVertexDecl);
	}

	return result;
}

// 解放
void CPostEffectVSManager::Release()
{
	for (IZ_UINT i = 0; i < E_POSTEFFECT_VTX_SHADER_NUM; ++i) {
		SAFE_RELEASE(m_pVS[i]);
	}

	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pVertexDecl);
}

// レンダラ初期化
IZ_BOOL CPostEffectVSManager::InitRenderer(CGraphicsDevice* pDevice)
{
	// 頂点宣言
	static const SVertexElement VERTEX_ELEMENT[] = {
		{0,  0, E_GRAPH_VTX_DECL_TYPE_FLOAT4, E_GRAPH_VTX_DECL_USAGE_POSITION, 0},	// 座標
		{0, 16, E_GRAPH_VTX_DECL_TYPE_FLOAT2, E_GRAPH_VTX_DECL_USAGE_TEXCOORD, 0},	// テクスチャ座標
	};

	// 頂点バッファ作成
	{
		m_pVertexBuffer = pDevice->CreateVertexBuffer(
							sizeof(CUSTOM_FVF),
							VERTEX_NUM,
							E_GRAPH_RSC_TYPE_STATIC);

		IZ_ASSERT(m_pVertexBuffer != IZ_NULL);
	}

	// 頂点バッファにデータセット
	if (!SetVertexBuffer()) {
		return IZ_FALSE;
	}

	// 頂点宣言作成
	{
		m_pVertexDecl = pDevice->CreateVertexDeclaration(
							VERTEX_ELEMENT, 
							COUNTOF(VERTEX_ELEMENT));

		VRETURN(m_pVertexBuffer != IZ_NULL);
	}

	return IZ_TRUE;
}

// 頂点バッファにデータセット
IZ_BOOL CPostEffectVSManager::SetVertexBuffer()
{
	IZ_ASSERT(m_pVertexBuffer != IZ_NULL);

	// ４頂点
	CUSTOM_FVF sFVF[VERTEX_NUM];

	{
#ifdef IZ_COORD_LEFT_HAND
        sFVF[0].vecPos.Set(0.0f, 0.0f, 0.5f, 1.0f);
        sFVF[1].vecPos.Set(1.0f, 0.0f, 0.5f, 1.0f);
        sFVF[2].vecPos.Set(0.0f, 1.0f, 0.5f, 1.0f);
        sFVF[3].vecPos.Set(1.0f, 1.0f, 0.5f, 1.0f);

        sFVF[0].uv[0] = 0.0f; sFVF[0].uv[1] = 0.0f;
        sFVF[1].uv[0] = 1.0f; sFVF[1].uv[1] = 0.0f;
        sFVF[2].uv[0] = 0.0f; sFVF[2].uv[1] = 1.0f;
        sFVF[3].uv[0] = 1.0f; sFVF[3].uv[1] = 1.0f;
#else
		sFVF[0].vecPos.Set(0.0f, 0.0f, 0.5f, 1.0f);
		sFVF[1].vecPos.Set(0.0f, 1.0f, 0.5f, 1.0f);
		sFVF[2].vecPos.Set(1.0f, 0.0f, 0.5f, 1.0f);
		sFVF[3].vecPos.Set(1.0f, 1.0f, 0.5f, 1.0f);

		sFVF[0].uv[0] = 0.0f; sFVF[0].uv[1] = 0.0f;
		sFVF[1].uv[0] = 0.0f; sFVF[1].uv[1] = 1.0f;
		sFVF[2].uv[0] = 1.0f; sFVF[2].uv[1] = 0.0f;
		sFVF[3].uv[0] = 1.0f; sFVF[3].uv[1] = 1.0f;
#endif
	}

	void* pvoid = IZ_NULL;
	m_pVertexBuffer->Lock(
		0, 0,
		&pvoid,
		IZ_FALSE);

	CUSTOM_FVF* pVertexBuf = static_cast<CUSTOM_FVF*>(pvoid);

	VRETURN(pVertexBuf != IZ_NULL);

	// データセット
	memcpy(pVertexBuf, &sFVF, sizeof(sFVF));

	m_pVertexBuffer->Unlock();

	return IZ_TRUE;
}
