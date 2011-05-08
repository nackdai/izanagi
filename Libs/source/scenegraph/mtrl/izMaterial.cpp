#include "scenegraph/mtrl/izMaterial.h"
#include "izIo.h"
#include "izMath.h"
#include "izGraph.h"

using namespace izanagi;

/**
*/
CMaterial* CMaterial::CreateMaterial(
	IMemoryAllocator* pAllocator,
	IInputStream* pIn)
{
	// Read mtrl's header.
	S_MTRL_HEADER sHeader;
	VRETURN_NULL(IZ_INPUT_READ(pIn, &sHeader, 0, sizeof(sHeader)));

	// TODO
	// Check magic number and version...

#if 0
	// Skip jump table.
	VRETURN(pIn->Seek(sizeof(IZ_UINT) * sHeader.numMtrl, E_IO_STREAM_SEEK_POS_CUR));
#endif

	IZ_BOOL result = IZ_TRUE;

#if 0
	for (IZ_UINT i = 0; i < sHeader.numMtrl; i++) {
#else
	CMaterial* pMtrl = IZ_NULL;
	{
#endif
		S_MTRL_MATERIAL sMtrlInfo;
		result = IZ_INPUT_READ(pIn, &sMtrlInfo, 0, sizeof(sMtrlInfo));
		VGOTO(result, __EXIT__);

		// NOTE
		// Material must have "a" shader.
		VGOTO(result = (sMtrlInfo.numShader == 1), __EXIT__);

		// Create instance.
#if 0
		CMaterial* pMtrl = CreateMaterial(
							sMtrlInfo.numTex,
							sMtrlInfo.numParam,
							sMtrlInfo.paramBytes,
							pAllocator);
#else
		pMtrl = CreateMaterial(
					sMtrlInfo.numTex,
					sMtrlInfo.numParam,
					sMtrlInfo.paramBytes,
					pAllocator);
#endif
		VGOTO(result = (pMtrl != IZ_NULL), __EXIT__);
		
		memcpy(
			&pMtrl->m_Header,
			&sMtrlInfo,
			sizeof(sMtrlInfo));

		// Read datas.
		result = pMtrl->Read(pIn);
		if (!result) {
			SAFE_RELEASE(pMtrl);
			VGOTO(result, __EXIT__);
		}

		pMtrl->AttachParamBuf();

#if 0
		// Add to list.
		cMtrlList.AddTail(pMtrl->GetListItem());
#else
#endif
	}

__EXIT__:
	if (!result) {
		// Release materials...
#if 0
		CStdList<CMaterial>::Item* pItem = cMtrlList.GetTop();
		while (pItem != IZ_NULL) {
			CMaterial* pMtrl = pItem->GetData();
			SAFE_RELEASE(pMtrl);
			pItem = pItem->GetNext();
		}
#else
		SAFE_RELEASE(pMtrl);
#endif
	}

	return pMtrl;
}

/**
*/
CMaterial* CMaterial::CreateMaterial(
	IMemoryAllocator* pAllocator,
	IZ_PCSTR pszName,
	IZ_UINT nTexNum,
	IZ_UINT nParamNum,
	IZ_UINT nParamBytes)
{
	// Create instance.
	CMaterial* pInstance = CreateMaterial(
							nTexNum,
							nParamNum,
							nParamBytes,
							pAllocator);
	VRETURN_NULL(pInstance != IZ_NULL);

	pInstance->m_Header.name.SetString(pszName);
	pInstance->m_Header.keyMaterial = pInstance->m_Header.name.GetKeyValue();

	pInstance->m_Header.numTex = nTexNum;

	// NOTE
	// Material must have "a" shader.
	pInstance->m_Header.numShader = 1;

	pInstance->m_Header.numParam = nParamNum;
	pInstance->m_Header.paramBytes = nParamBytes;

	pInstance->m_IsFromMtrlFile = IZ_FALSE;

	return pInstance;
}

CMaterial* CMaterial::CreateMaterial(
	IZ_UINT nTexNum,
	IZ_UINT nParamNum,
	IZ_UINT nParamBytes,
	IMemoryAllocator* pAllocator)
{
	const IZ_UINT nShaderNum = 1;

	// Compute size of allocating memory.
	size_t nSize = sizeof(CMaterial);
	size_t nTexInfoSize = sizeof(S_MTRL_TEXTURE) * nTexNum;
	size_t nShaderInfoSize = sizeof(S_MTRL_SHADER) * nShaderNum;
	size_t nParamInfoSize = sizeof(S_MTRL_PARAM) * nParamNum;
	size_t nParamDataSize = nParamBytes;

	nSize += nTexInfoSize;
	nSize += nShaderInfoSize;
	nSize += nParamInfoSize;
	nSize += nParamDataSize;

	// Allocate memory.
	IZ_UINT8* pBuf = reinterpret_cast<IZ_UINT8*>(ALLOC_ZERO(pAllocator, nSize));
	VRETURN_NULL(pBuf != IZ_NULL);

	IZ_UINT8* pTop = pBuf;

	//CMaterial* pInstance = reinterpret_cast<CMaterial*>(pBuf);
	CMaterial* pInstance = new(pBuf) CMaterial;
	{
		pBuf += sizeof(CMaterial);

		pInstance->AddRef();

		pInstance->m_pAllocator = pAllocator;
		pInstance->m_IsFromMtrlFile = IZ_TRUE;

		pInstance->m_pTexInfo = reinterpret_cast<S_MTRL_TEXTURE*>(pBuf);
		pBuf += nTexInfoSize;

		pInstance->m_pShaderInfo = reinterpret_cast<S_MTRL_SHADER*>(pBuf);
		pBuf += nShaderInfoSize;

		pInstance->m_pParamInfo = reinterpret_cast<S_MTRL_PARAM*>(pBuf);
		pBuf += nParamInfoSize;

		pInstance->m_pParamBuf = reinterpret_cast<IZ_UINT8*>(pBuf);
		pBuf += nParamDataSize;
	}

	IZ_ASSERT(CStdUtil::GetPtrDistance(pBuf, pTop) == nSize);

	pInstance->m_Header.numTex = nTexNum;
	pInstance->m_Header.numShader = nShaderNum;
	pInstance->m_Header.numParam = nParamNum;
	pInstance->m_Header.paramBytes = nParamBytes;

	return pInstance;
}

CMaterial::CMaterial()
{
	m_pAllocator = IZ_NULL;

	m_IsFromMtrlFile = IZ_FALSE;
	m_nAttachBufPos = 0;

	FILL_ZERO(&m_Header, sizeof(m_Header));

	m_pTexInfo = IZ_NULL;

	m_pShaderInfo = IZ_NULL;

	m_pParamInfo = IZ_NULL;
	m_pParamBuf = IZ_NULL;

	m_ListItem.Init(this);
}

CMaterial::~CMaterial()
{
	for (IZ_UINT i = 0; i < m_Header.numTex; ++i) {
		SAFE_RELEASE(m_pTexInfo[i].tex);
	}

	for (IZ_UINT i = 0; i < m_Header.numShader; ++i) {
		SAFE_RELEASE(m_pShaderInfo[i].shader);
	}
}

IZ_BOOL CMaterial::Read(IInputStream* pIn)
{
	IZ_INPUT_READ_VRETURN(pIn, m_pTexInfo, 0, sizeof(S_MTRL_TEXTURE) * m_Header.numTex);
	IZ_INPUT_READ_VRETURN(pIn, m_pShaderInfo, 0, sizeof(S_MTRL_SHADER) * m_Header.numShader);
	
	IZ_INPUT_READ_VRETURN(pIn, m_pParamInfo, 0, sizeof(S_MTRL_PARAM) * m_Header.numParam);
	IZ_INPUT_READ_VRETURN(pIn, m_pParamBuf, 0, m_Header.paramBytes);

	return IZ_TRUE;
}

void CMaterial::AttachParamBuf()
{
	IZ_UINT nPos = 0;

	for (IZ_UINT i = 0; i < m_Header.numParam; ++i) {
		S_MTRL_PARAM& sParam = m_pParamInfo[i];

		if (sParam.bytes > 0) {
			sParam.param = m_pParamBuf + nPos;
			nPos += sParam.bytes;
		}
	}

	if (m_Header.paramBytes > 0) {
		IZ_ASSERT(nPos == m_Header.paramBytes);
		m_nAttachBufPos = nPos;
	}
}

// 描画準備
IZ_BOOL CMaterial::Prepare(CGraphicsDevice* pDevice)
{
	IZ_ASSERT(pDevice != IZ_NULL);

	// TODO
	IZ_ASSERT((m_Header.numShader == 1) && (m_pShaderInfo != IZ_NULL));
	IZ_ASSERT(m_pShaderInfo->shader != IZ_NULL);
	izanagi::CShader* pShader = m_pShaderInfo->shader;

	// Textures.
	for (IZ_UINT i = 0; i < m_Header.numTex; i++) {
		if (m_pTexInfo[i].tex != IZ_NULL) {
			pDevice->SetTexture(i, m_pTexInfo[i].tex);
		}
	}

	// Parameters.
	for (IZ_UINT i = 0; i < m_Header.numParam; i++) {
		S_MTRL_PARAM& sParamInfo = m_pParamInfo[i];
		
		if (sParamInfo.handle == 0) {
			sParamInfo.handle = pShader->GetParameterByName(sParamInfo.name.GetString());
		}

		if (sParamInfo.handle > 0) {
			pShader->SetParamValue(
				sParamInfo.handle,
				sParamInfo.param,
				sParamInfo.bytes);
		}
	}

	return IZ_TRUE;
}

namespace {
	template <typename _T>
	_T* _GetEmpty(
		_T* pList,
		IZ_UINT nNum)
	{
		for (IZ_UINT i = 0; i < nNum; ++i) {
			_T& s = pList[i];
			if (s.key == 0) {
				return &s;
			}
		}
		return IZ_NULL;
	}

	template <typename _T>
	_T* _Find(
		_T* pList,
		IZ_UINT nNum,
		IZ_UINT key)
	{
		for (IZ_UINT i = 0; i < nNum; ++i) {
			_T& s = pList[i];
			if (s.key == key) {
				return &s;
			}
		}
		return IZ_NULL;
	}
}	// namespace

IZ_BOOL CMaterial::AddTexture(
	IZ_PCSTR pszName,
	const S_MTRL_TEXTURE_TYPE& type,
	CBaseTexture* pTex)
{
	IZ_BOOL ret = IZ_TRUE;

	// Check whether there is not specified texture.
	S_MTRL_TEXTURE* pInfo = const_cast<S_MTRL_TEXTURE*>(GetTexInfoByName(pszName));

	if (pInfo == IZ_NULL) {
		// If there is not specified texture...
		VRETURN(!m_IsFromMtrlFile);

		// Find empty texture's info.
		pInfo = _GetEmpty(m_pTexInfo, m_Header.numTex);
		ret = (pInfo != IZ_NULL);
		if (ret) {
			// Update texture's info.
			pInfo->name.SetString(pszName);
			pInfo->key = pInfo->name.GetKeyValue();

			pInfo->type.flags = type.flags;
			SAFE_REPLACE(pInfo->tex, pTex);
		}
	}

	return ret;
}

/**
*/
IZ_BOOL CMaterial::SetTexture(
	IZ_PCSTR pszName, 
	CBaseTexture* pTex)
{
	return SetTexture(CKey(pszName), pTex);
}

/**
*/
IZ_BOOL CMaterial::SetTexture(
	IZ_UINT nKey,
	CBaseTexture* pTex)
{
	// Find specified texture's info.
	S_MTRL_TEXTURE* pTexInfo = _Find(
								m_pTexInfo,
								m_Header.numTex,
								nKey);
	VRETURN(pTexInfo != IZ_NULL);
	
	SAFE_REPLACE(pTexInfo->tex, pTex);

	return IZ_TRUE;
}

/**
*/
IZ_BOOL CMaterial::AddShader(CShader* pShader)
{
	IZ_BOOL ret = IZ_TRUE;

#if 0
	IZ_PCSTR pszShaderName = pShader->GetName();

	// Check whether there is not specified shader.
	S_MTRL_SHADER* pInfo = const_cast<S_MTRL_SHADER*>(GetShaderInfoByName(pszShaderName));

	if (pInfo == IZ_NULL) {
		// If there is not specified shader.
		VRETURN(!m_IsFromMtrlFile);

		// Find empty shader's info.
		pInfo = _GetEmpty(m_pShaderInfo, m_Header.numShader);
		ret = (pInfo != IZ_NULL);
		if (ret) {
			// Update shader's info.
			pInfo->name.SetString(pszShaderName);
			pInfo->key = pShader->GetKey();

			SAFE_REPLACE(pInfo->shader, pShader);
		}
	}
#else
	IZ_ASSERT(m_pShaderInfo != IZ_NULL);

	// there is not specified shader.
	VRETURN(!m_IsFromMtrlFile);

	// Update shader's info.
	m_pShaderInfo->name.SetString(pShader->GetName());
	m_pShaderInfo->key = pShader->GetKey();

	SAFE_REPLACE(m_pShaderInfo->shader, pShader);
#endif

	return ret;
}

/**
*/
IZ_BOOL CMaterial::SetShader(CShader* pShader)
{
	IZ_ASSERT(pShader != IZ_NULL);

	IZ_UINT nKey = pShader->GetKey();

	// Find specified shader's info.
	S_MTRL_SHADER* pShaderInfo = _Find(
									m_pShaderInfo,
									m_Header.numShader,
									nKey);
	VRETURN(pShaderInfo != IZ_NULL);

	SAFE_REPLACE(pShaderInfo->shader, pShader);

	return IZ_TRUE;
}

namespace {
	inline size_t _GetSize(E_MTRL_PARAM_TYPE type)
	{
		switch (type) {
		case E_MTRL_PARAM_TYPE_FLOAT:
			return sizeof(IZ_FLOAT);
		case E_MTRL_PARAM_TYPE_VECTOR:
			return sizeof(SVector);
		case E_MTRL_PARAM_TYPE_MATRIX:
			return sizeof(SMatrix);
		case E_MTRL_PARAM_TYPE_UINT:
			return sizeof(IZ_UINT);
		case E_MTRL_PARAM_TYPE_BOOL:
			return sizeof(IZ_BOOL);
		}

		IZ_ASSERT(IZ_FALSE);
		return 0;
	}

}	// namespace

/**
*/
IZ_BOOL CMaterial::AddParameter(
	IZ_PCSTR pszName,
	E_MTRL_PARAM_TYPE type,
	IZ_UINT nNum,
	const void* pValue)
{
	IZ_BOOL ret = IZ_TRUE;

	// Check whether there is not specified parameter.
	S_MTRL_PARAM* pInfo = const_cast<S_MTRL_PARAM*>(GetParamInfoByName(pszName));

	if (pInfo == IZ_NULL) {
		// If there is not specified parameter...
		VRETURN(!m_IsFromMtrlFile);

		// Find empty parameter's info.
		pInfo = _GetEmpty(m_pParamInfo, m_Header.numParam);
		ret = (pInfo != IZ_NULL);
		if (ret) {
			// Update paramter's info.
			pInfo->name.SetString(pszName);
			pInfo->key = pInfo->name.GetKeyValue();

			pInfo->type = type;
			pInfo->elements = nNum;

			size_t size = _GetSize(type);
			pInfo->bytes = static_cast<IZ_UINT>(nNum * size);

			ret = (m_nAttachBufPos + pInfo->bytes < m_Header.paramBytes);
			VRETURN(ret);

			// Attach buffer for parameter.
			pInfo->param = m_pParamBuf + m_nAttachBufPos;

			// Advance position of buffer.
			m_nAttachBufPos += pInfo->bytes;
				
			memcpy(pInfo->param, pValue, pInfo->bytes);
		}
	}

	return ret;
}

/**
*/
IZ_BOOL CMaterial::SetParameter(
	IZ_PCSTR pszName,
	IZ_UINT nBytes,
	const void* pValue)
{
	return SetParameter(CKey(pszName), nBytes, pValue);
}

/**
*/
IZ_BOOL CMaterial::SetParameter(
	IZ_UINT nKey,
	IZ_UINT nBytes,
	const void* pValue)
{
	IZ_ASSERT(pValue != IZ_NULL);

	// Find specified parameter's info.
	S_MTRL_PARAM* pParamInfo = _Find(
								m_pParamInfo,
								m_Header.numParam,
								nKey);
	VRETURN(pParamInfo != IZ_NULL);
	VRETURN(pParamInfo->bytes == nBytes);
	VRETURN(pParamInfo->param != IZ_NULL);

	memcpy(pParamInfo->param, pValue, nBytes);

	return IZ_TRUE;
}

// Return whether materal has textures, shaders that are specified.
IZ_BOOL CMaterial::IsValid() const
{
	for (IZ_UINT i = 0; i < m_Header.numTex; i++) {
		if (m_pTexInfo[i].tex == IZ_NULL) {
			return IZ_FALSE;
		}
	}

	for (IZ_UINT i = 0; i < m_Header.numShader; i++) {
		if (m_pShaderInfo[i].shader == IZ_NULL) {
			return IZ_FALSE;
		}
	}

	return IZ_TRUE;
}

// マテリアル名変更
void CMaterial::SetName(IZ_PCSTR pszName)
{
	m_Header.name.SetString(pszName);
	m_Header.keyMaterial = m_Header.name.GetKeyValue();
}

namespace {
	template <typename _T>
	const _T* _GetInfoByKey(
		const _T* pInfo,
		IZ_UINT nNum,
		IZ_UINT key)
	{
		for (IZ_UINT i = 0; i < nNum; ++i) {
			const _T* p = &pInfo[i];
			if (p->key == key) {
				return p;
			}
		}

		// IZ_ASSERT(IZ_FALSE);
		return IZ_NULL;
	}
}	// namespace

const S_MTRL_TEXTURE* CMaterial::GetTexInfoByIdx(IZ_UINT idx) const
{
	VRETURN_NULL(idx < m_Header.numTex);
	return &m_pTexInfo[idx];
}

const S_MTRL_TEXTURE* CMaterial::GetTexInfoByName(IZ_PCSTR pszName) const
{
	const S_MTRL_TEXTURE* ret = _GetInfoByKey(m_pTexInfo, m_Header.numTex, CKey(pszName));
	return ret;
}

const S_MTRL_TEXTURE* CMaterial::GetTexInfoByKey(const CKey& key) const
{
	const S_MTRL_TEXTURE* ret = _GetInfoByKey(m_pTexInfo, m_Header.numTex, key);
	return ret;
}

#if 0
const S_MTRL_SHADER* CMaterial::GetShaderInfoByIdx(IZ_UINT idx) const
{
	VRETURN_NULL(idx < m_Header.numShader);
	return &m_pShaderInfo[idx];
}

const S_MTRL_SHADER* CMaterial::GetShaderInfoByName(IZ_PCSTR pszName) const
{
	const S_MTRL_SHADER* ret = _GetInfoByKey(m_pShaderInfo, m_Header.numTex, CKey(pszName));
	return ret;
}

const S_MTRL_SHADER* CMaterial::GetShaderInfoByKey(const CKey& key) const
{
	const S_MTRL_SHADER* ret = _GetInfoByKey(m_pShaderInfo, m_Header.numTex, key);
	return ret;
}
#endif

const S_MTRL_PARAM* CMaterial::GetParamInfoByIdx(IZ_UINT idx) const
{
	VRETURN_NULL(idx < m_Header.numParam);
	return &m_pParamInfo[idx];
}

const S_MTRL_PARAM* CMaterial::GetParamInfoByName(IZ_PCSTR pszName) const
{
	const S_MTRL_PARAM* ret = _GetInfoByKey(m_pParamInfo, m_Header.numTex, CKey(pszName));
	return ret;
}

const S_MTRL_PARAM* CMaterial::GetParamInfoByKey(const CKey& key) const
{
	const S_MTRL_PARAM* ret = _GetInfoByKey(m_pParamInfo, m_Header.numTex, key);
	return ret;
}

CBaseTexture* CMaterial::GetTextureByIdx(IZ_UINT idx)
{
	S_MTRL_TEXTURE* pInfo = const_cast<S_MTRL_TEXTURE*>(GetTexInfoByIdx(idx));
	if (pInfo != IZ_NULL) {
		return pInfo->tex;
	}
	return IZ_NULL;
}

CBaseTexture* CMaterial::GetTextureByName(IZ_PCSTR pszName)
{
	S_MTRL_TEXTURE* pInfo = const_cast<S_MTRL_TEXTURE*>(GetTexInfoByName(pszName));
	if (pInfo != IZ_NULL) {
		return pInfo->tex;
	}
	return IZ_NULL;
}

CBaseTexture* CMaterial::GetTextureByKey(const CKey& key)
{
	S_MTRL_TEXTURE* pInfo = const_cast<S_MTRL_TEXTURE*>(GetTexInfoByKey(key));
	if (pInfo != IZ_NULL) {
		return pInfo->tex;
	}
	return IZ_NULL;
}

#if 0
CShader* CMaterial::GetShaderByIdx(IZ_UINT idx)
{
	S_MTRL_SHADER* pInfo = const_cast<S_MTRL_SHADER*>(GetShaderInfoByIdx(idx));
	if (pInfo != IZ_NULL) {
		return pInfo->shader;
	}
	return IZ_NULL;
}

CShader* CMaterial::GetShaderByName(IZ_PCSTR pszName)
{
	S_MTRL_SHADER* pInfo = const_cast<S_MTRL_SHADER*>(GetShaderInfoByName(pszName));
	if (pInfo != IZ_NULL) {
		return pInfo->shader;
	}
	return IZ_NULL;
}

CShader* CMaterial::GetShaderByKey(const CKey& key)
{
	S_MTRL_SHADER* pInfo = const_cast<S_MTRL_SHADER*>(GetShaderInfoByKey(key));
	if (pInfo != IZ_NULL) {
		return pInfo->shader;
	}
	return IZ_NULL;
}
#endif

/**
* Set technique index of shader.
*/
IZ_BOOL CMaterial::SetShaderTechnique(IZ_UINT idx)
{
	IZ_ASSERT(m_pShaderInfo != IZ_NULL);

	// Material must have a shader.
	VRETURN(m_pShaderInfo->shader != IZ_NULL);
	VRETURN(m_pShaderInfo->shader->GetTechNum() > idx);

	m_pShaderInfo->tech_idx = idx;

	return IZ_TRUE;
}

/**
* Return technique index of shader.
*/
IZ_INT CMaterial::GetShaderTechnique() const
{
	IZ_ASSERT(m_pShaderInfo != IZ_NULL);

	VRETURN_VAL(m_pShaderInfo->shader != IZ_NULL, -1);

	return m_pShaderInfo->tech_idx;
}

const void* CMaterial::GetParamByIdx(IZ_UINT idx) const
{
	const S_MTRL_PARAM* pInfo = GetParamInfoByIdx(idx);
	if (pInfo != IZ_NULL) {
		return pInfo->param;
	}
	return IZ_NULL;
}

const void* CMaterial::GetParamByName(IZ_PCSTR pszName) const
{
	const S_MTRL_PARAM* pInfo = GetParamInfoByName(pszName);
	if (pInfo != IZ_NULL) {
		return pInfo->param;
	}
	return IZ_NULL;
}

const void* CMaterial::GetParamByKey(const CKey& key) const
{
	const S_MTRL_PARAM* pInfo = GetParamInfoByKey(key);
	if (pInfo != IZ_NULL) {
		return pInfo->param;
	}
	return IZ_NULL;
}
