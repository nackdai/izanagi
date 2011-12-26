#include <algorithm>
#include "PostEffectConverterImpl.h"

#include "StringChunk.h"
#include "DataBuffer.h"
#include "TextureUtil.h"
#include "SamplerUtil.h"
#include "ParamUtil.h"
#include "PassUtil.h"
#include "PostEffectConverterUtil.h"

#include "CompileCmdCreator.h"

CPostEffectConverter CPostEffectConverter::s_cInstance;

/**
* コンストラクタ
*/
CPostEffectConverter::CPostEffectConverter()
{
	m_pCgContext = NULL;
	m_pCgEffect = NULL;
}

#define DESTROY(p, func)	if (p != NULL) { func(p); }

/**
* デストラクタ
*/
CPostEffectConverter::~CPostEffectConverter()
{
	DESTROY(m_pCgContext, ::cgDestroyContext);
	DESTROY(m_pCgEffect, ::cgDestroyEffect);
}

BOOL CPostEffectConverter::Begin(LPCSTR lpszShaderFile)
{
	// Cgコンテキスト作成
	m_pCgContext = cgCreateContext();

	// stateの保持のために必要
	// ただし、D3D9関係のヘッダ、ライブラリ、DLLが必要になる
	cgD3D9RegisterStates(m_pCgContext);

	// エフェクト作成
	m_pCgEffect = cgCreateEffectFromFile(
					m_pCgContext,
					lpszShaderFile,
					NULL);

	CGerror error = ::cgGetError();

	if (error != CG_NO_ERROR) {
		// TODO
		// エラー出力表示
		const char* pErrorStr = cgGetLastListing(m_pCgContext);
		if (pErrorStr != NULL) {
			printf("%s\n", pErrorStr);

#if 0
			// NOTE
			// 何故かcgCreateEffectFromFileでコンパイルが失敗してもNULLが返ってこないことがある・・・。
			// なので、エラー文字列を調べて "error" があったら失敗とみなす・・・。
			izanagi::tool::CString str(pErrorStr);
			if (str.Find(_T(" error ")) >= 0) {
				return FALSE;
			}
#endif
		}
	}

	if (m_pCgEffect == NULL) {
		// エフェクト作成失敗したので、コンバート失敗
		return FALSE;
	}

	return TRUE;
}

// NOTE
// フォーマット
// +----------------+
// | ファイルヘッダ |
// +----------------+
// |   パラメータ   |
// |    テーブル    |
// +----------------+
// | アノテーション |
// |  (パラメータ)  |
// |    テーブル    |
// +----------------+
// |パラメータ初期値|
// |    バッファ    |
// +----------------+
// |   テクスチャ   |
// |    テーブル    |
// +----------------+
// |サンプラテーブル|
// +----------------+
// |   テクニック   |
// |    テーブル    |
// +----------------+
// |  パステーブル  |
// +----------------+
// |パスで利用される|
// |  パラメータと  |
// |   サンプラの   |
// |  インデックス  |
// +----------------+
// | 文字列バッファ |
// +----------------+
// |   プログラム   |
// +----------------+-+
// | 頂点プログラム | |
// |     ヘッダ     | |
// +----------------+ +-- (1)
// | 頂点プログラム | |
// +----------------+-+
// |     (1)を      |
// | numVtxProgram  |
// |    繰り返す    |
// +----------------+

/**
* シェーダ解析
*/
BOOL CPostEffectConverter::Export(LPCSTR lpszOutFile)
{
	IZ_ASSERT(m_pCgEffect != NULL);

	VRETURN(m_Out.Open(lpszOutFile));

	// ファイルヘッダ
	{
		FILL_ZERO(&m_PesHeader, sizeof(m_PesHeader));

		// TODO
		// magic
		// version

		m_PesHeader.sizeHeadaer = sizeof(m_PesHeader);
	}

	// ファイルヘッダ分空ける
	VRETURN(m_Out.Seek(sizeof(m_PesHeader), izanagi::E_IO_STREAM_SEEK_POS_START));

	ExportParameter();
	ExportTexture();
	ExportSampler();
	ExportTechnique();
	
	ExportPass();
	ExportUsedParamAndSamplerIdxByPass();

	ExportStringBuffer();

	// プログラム位置
	m_PesHeader.posProgram = m_Out.GetCurPos();

	ExportPSProgram();

	ExportVSProgram();

	// ファイルサイズ取得
	m_PesHeader.sizeFile = m_Out.GetCurPos();

	// 先頭に戻る
	VRETURN(m_Out.Seek(0, izanagi::E_IO_STREAM_SEEK_POS_START));

	// ファイルヘッダ出力
	m_Out.Write(&m_PesHeader, 0, sizeof(m_PesHeader));

	m_Out.Finalize();
	
	return TRUE;
}

template <typename _T>
BOOL CPostEffectConverter::ExportData(const _T& data)
{
	IZ_OUTPUT_WRITE_VRETURN(
		&m_Out,
		&data,
		0,
		sizeof(_T));

	return TRUE;
}

void CPostEffectConverter::SetPesId()
{
	CGparameter param = ::cgGetFirstEffectParameter(m_pCgEffect);

	while (param != NULL) {
		izanagi::tool::CString str(::cgGetParameterName(param));

		if (str == "__pes_ID__") {
			IZ_PCSTR p = ::cgGetStringParameterValue(param);
			if (p != NULL) {
				// IDは32文字まで・・・
				if (strlen(p) > izanagi::PES_ID_LEN) {
					// TODO
					// 警告を出す
				}

				memcpy(
					m_PesHeader.pesID, 
					p, 
					IZ_MIN(strlen(p), izanagi::PES_ID_LEN));

				m_PesHeader.hashKey = izanagi::CKey::GenerateValue(p);
			}

			break;
		}

		param = ::cgGetNextParameter(param);
	}
}

namespace {
	// テクニック内のパス数取得
	inline IZ_UINT _GetPassNum(CGtechnique tech)
	{
		IZ_UINT ret = 0;

		CGpass pass = ::cgGetFirstPass(tech);
		while (pass != NULL) {
			ret++;
			pass = ::cgGetNextPass(pass);
		}

		return ret;
	}
}	// namespace

/**
* テクニック解析
*/
BOOL CPostEffectConverter::ExportTechnique()
{
	UINT nPassPos = 0;

	// テクニック取得
	CGtechnique tech = ::cgGetFirstTechnique(m_pCgEffect);

	if (tech != NULL) {
		while (tech != NULL) {
			izanagi::S_PES_TECHNIQUE sTech;
			{
				FILL_ZERO(&sTech, sizeof(sTech));

				IZ_PCSTR name = ::cgGetTechniqueName(tech);
				IZ_UINT pos = CStringChunk::GetInstance().Register(name);

				//sTech.name = *(IZ_PCSTR*)(&pos);
				sTech.posName = pos;
				sTech.keyName = izanagi::CKey::GenerateValue(name);
				sTech.numPass = _GetPassNum(tech);
				sTech.posPass = nPassPos;
			}

			nPassPos += sTech.numPass;

			// 出力
			VRETURN(ExportData(sTech));

			tech = ::cgGetNextTechnique(tech);

			m_PesHeader.numTech++;
		}
	}
	else {
		// テクニックは必ず一つはないといけない
		IZ_ASSERT(FALSE);

		// TODO

		return FALSE;
	}

	return TRUE;
}

namespace {
	izanagi::E_POSTEFFECT_TEXTURE_TYPE _GetTexTypeFromSemantic(CGparameter param)
	{
		static IZ_PCSTR TexSemanticTbl[] = {
			"NONE",
			"INPUT_SCENE",
			"INPUT_DEPTH",
			"PRIVATE",
			"INPUT",
		};
		IZ_C_ASSERT(izanagi::E_POSTEFFECT_TEXTURE_TYPE_NUM == COUNTOF(TexSemanticTbl));

		IZ_PCSTR semantic = ::cgGetParameterSemantic(param);

		if (strlen(semantic) > 0) {
			for (IZ_UINT i = 0; i < izanagi::E_POSTEFFECT_TEXTURE_TYPE_NUM; i++) {
				if (izanagi::tool::CString::CmpStr(semantic, TexSemanticTbl[i])) {
					return static_cast<izanagi::E_POSTEFFECT_TEXTURE_TYPE>(i);
				}
			}

			IZ_ASSERT(IZ_FALSE);
		}

		return izanagi::E_POSTEFFECT_TEXTURE_TYPE_NONE;
	}
}	// namespace

BOOL CPostEffectConverter::ExportTexture()
{
	m_TexList.clear();

	CGparameter param = ::cgGetFirstEffectParameter(m_pCgEffect);

	while (param != NULL) {
		if (CParamUtil::IsTexture(param)) {
			izanagi::S_PES_TEXTURE sTex;
			{
				FILL_ZERO(&sTex, sizeof(sTex));

				CParamUtil::SetNameAndSemantic(sTex, param);

				sTex.type = _GetTexTypeFromSemantic(param);

				sTex.ann.isRenderTarget = IZ_TRUE;
				sTex.ann.isDynamic = IZ_FALSE;
				sTex.ann.typeRsc = izanagi::E_GRAPH_RSC_TYPE_STATIC;

				VRETURN(
					CTextureUtil::SetAnnValue(
						sTex.ann,
						param));
			}

			m_TexList.push_back(param);

			// 出力
			VRETURN(ExportData(sTex));

			m_PesHeader.numTex++;
		}

		param = ::cgGetNextParameter(param);
	}

	return TRUE;
}

BOOL CPostEffectConverter::ExportSampler()
{
	CGparameter param = ::cgGetFirstEffectParameter(m_pCgEffect);

	while (param != NULL) {
		if (::cgIsParameterUsed(param, m_pCgEffect)) {
			VRETURN(CParamUtil::IsValidParameter(param));

			if (CParamUtil::IsSampler(param)) {
				izanagi::S_PES_SAMPLER sSampler;
				{
					FILL_ZERO(&sSampler, sizeof(sSampler));

					sSampler.state.minFilter = izanagi::E_GRAPH_TEX_FILTER_LINEAR;
					sSampler.state.magFilter = izanagi::E_GRAPH_TEX_FILTER_LINEAR;
					sSampler.state.addrU = izanagi::E_GRAPH_TEX_ADDRESS_CLAMP;
					sSampler.state.addrV = izanagi::E_GRAPH_TEX_ADDRESS_CLAMP;

					CParamUtil::SetNameAndSemantic(sSampler, param);
					
					VRETURN(
						CSamplerUtil::SetStateValue(
							sSampler.state,
							param));

					VRETURN(
						CSamplerUtil::BindTexture(
							sSampler.state,
							param,
							m_TexList));
				}

				m_SamplerList.push_back(param);

				// 出力
				VRETURN(ExportData(sSampler));

				m_PesHeader.numSampler++;
			}
		}

		param = ::cgGetNextParameter(param);
	}

	return TRUE;
}

namespace {
	inline BOOL _IgnoreParameter(CGparameter param)
	{
		IZ_PCSTR name = ::cgGetParameterName(param);

		izanagi::tool::CString str(name);

		if (str == "__pes_ID__") {
			return TRUE;
		}
		
		return FALSE;
	}
}	// namespace

/**
* パラメータ解析
*/
BOOL CPostEffectConverter::ExportParameter()
{
	IZ_UINT nAnnIdx = 0;

	// For description.
	CGparameter param = ::cgGetFirstEffectParameter(m_pCgEffect);

	while (param != NULL) {
		if (CParamUtil::IsParameter(param)
			&& !_IgnoreParameter(param))
		{
			// For Debug...
			IZ_PCSTR name = ::cgGetParameterName(param);

			VRETURN(CParamUtil::IsValidParameter(param));

			izanagi::S_PES_PARAMETER sParam;
			FILL_ZERO(&sParam, sizeof(sParam));
			
			sParam.DoNotStrip = CParamUtil::DoNotStrip(param);
			
			if (sParam.DoNotStrip
				|| DoNotRemoveParam(param))
			{
				CParamUtil::SetNameAndSemantic(sParam, param);

				VRETURN(
					CParamUtil::SetDescValue(
						sParam,
						param));

				if (sParam.hasAnn) {
					sParam.AnnotationIdx = nAnnIdx++;
				}

				// Register initial value.
				VRETURN(
					CParamUtil::GetInitValue(
						sParam,
						param));

				m_ParamList.push_back(param);

				// 出力
				VRETURN(ExportData(sParam));

				m_PesHeader.numParam++;
			}
		}

		param = ::cgGetNextParameter(param);
	}

	VRETURN(ExportParamAnn(nAnnIdx));

	// Export initial value.
	m_PesHeader.sizeValueBuffer = CDataBuffer::GetInstance().GetBufferSize();
	if (m_PesHeader.sizeValueBuffer > 0) {
		const void* pBuf = CDataBuffer::GetInstance().GetBuffer();

		IZ_OUTPUT_WRITE_VRETURN(
			&m_Out,
			pBuf,
			0,
			m_PesHeader.sizeValueBuffer);
	}

	return TRUE;
}

BOOL CPostEffectConverter::ExportParamAnn(IZ_INT nAnnNum/*= -1*/)
{
	IZ_UINT nAnnCnt = 0;

	// For annotation.
	std::vector<CGparameter>::iterator it = m_ParamList.begin();

	for (; it != m_ParamList.end(); it++) {
		CGparameter param = *it;

		if (CParamUtil::HasAnn(param)) {
			izanagi::S_PES_PARAM_ANN sAnn;
			
			FILL_ZERO(&sAnn, sizeof(sAnn));

			VRETURN(
				CParamUtil::SetAnnValue(
					sAnn,
					param));

			// 出力
			VRETURN(ExportData(sAnn));

			nAnnCnt++;
		}
	}

	BOOL ret = (nAnnNum >= 0
				? (nAnnCnt == nAnnNum)
				: TRUE);

	m_PesHeader.numParamAnn = nAnnCnt;

	IZ_ASSERT(ret);
	return ret;
}

BOOL CPostEffectConverter::DoNotRemoveParam(CGparameter param)
{
	BOOL bIsUsedInEffect = ::cgIsParameterUsed(param, m_pCgEffect);

	izanagi::tool::CString strParamName(::cgGetParameterName(param));

	{
		// 頂点シェーダでしか利用されていない
		// -> ピクセルシェーダで利用されていたらセーフ
		CGtechnique tech = ::cgGetFirstTechnique(m_pCgEffect);
		while (tech != NULL) {
			CGpass pass = ::cgGetFirstPass(tech);

			while (pass != NULL) {
				// Check whether the parameter is use in pixel shader.
				CGprogram progPS = CPassUtil::GetPSProgram(pass);
				if (::cgIsParameterUsed(param, progPS)
					&& bIsUsedInEffect)
				{
					return TRUE;
				}

				// Check whether the parameter is texture offset.
				IZ_PCSTR pszTexOffsetParam = CPassUtil::GetTextureOffsetParameterName(pass);
				if (pszTexOffsetParam != IZ_NULL) {
					if (strParamName == pszTexOffsetParam) {
						return TRUE;
					}
				}

				pass = ::cgGetNextPass(pass);
			}

			tech = ::cgGetNextTechnique(tech);
		}
	}

	return FALSE;
}

namespace {
	inline IZ_UINT _GetUsedParamNum(
		const std::vector<CGparameter>& tvParamList,
		CGpass pass)
	{
		IZ_UINT ret = 0;

		std::vector<CGparameter>::const_iterator it = tvParamList.begin();
		for (; it != tvParamList.end(); it++) {
			CGparameter param = *it;

			if (::cgIsParameterUsed(param, pass)) {
				ret++;
			}
		}

		return ret;
	}

	inline IZ_INT _GetParamIdx(
		IZ_PCSTR pszParamName,
		const std::vector<CGparameter>& tvParamList,
		CGpass pass)
	{
		izanagi::tool::CString strName(pszParamName);

		std::vector<CGparameter>::const_iterator it = tvParamList.begin();
		for (IZ_INT i = 0; it != tvParamList.end(); it++, i++) {
			CGparameter param = *it;

			IZ_PCSTR name = ::cgGetParameterName(param);
			if (strName == name) {
				return i;
			}
		}

		return -1;
	}

	inline IZ_INT _GetRenderTargetIdx(
		const std::vector<CGparameter>& tvParamList,
		CGpass pass)
	{
		IZ_PCSTR pszRTName = CPassUtil::GetRenderTargetName(pass);
		if (pszRTName == NULL) {
			return -1;
		}

		IZ_INT ret = _GetParamIdx(
						pszRTName,
						tvParamList,
						pass);

		return ret;
	}

	inline IZ_INT _GetTexOffsetParamIdx(
		const std::vector<CGparameter>& tvParamList,
		CGpass pass)
	{
		IZ_PCSTR pszTexOffsetName = CPassUtil::GetTextureOffsetParameterName(pass);
		if (pszTexOffsetName == NULL) {
			return -1;
		}
		
		IZ_INT ret = _GetParamIdx(
						pszTexOffsetName,
						tvParamList,
						pass);

		return ret;
	}

	inline IZ_UINT _GetFileSize(IZ_PCSTR pszFile)
	{
		izanagi::CFileInputStream in;
		VRETURN_VAL(in.Open(pszFile), 0);

		IZ_UINT ret = static_cast<IZ_UINT>(in.GetSize());

		return ret;
	}
}	// namespace

// パス解析
BOOL CPostEffectConverter::ExportPass()
{
	IZ_UINT nTechIdx = 0;
	IZ_UINT nPassIdx = 0;

	IZ_UINT nConstNum = 0;
	IZ_UINT nSamplerNum = 0;

	izanagi::tool::CString strPrevPassFunctoName;

	CGtechnique tech = ::cgGetFirstTechnique(m_pCgEffect);

	while (tech != NULL) {
		CGpass pass = ::cgGetFirstPass(tech);
		while (pass != NULL) {
			izanagi::S_PES_PASS sPass;
			{
				FILL_ZERO(&sPass, sizeof(sPass));

				IZ_PCSTR name = ::cgGetPassName(pass);
				IZ_UINT pos = CStringChunk::GetInstance().Register(name);

				//sPass.name = *(IZ_PCSTR*)(&pos);
				sPass.posName = pos;
				sPass.keyName = izanagi::CKey::GenerateValue(name);
				sPass.TechniqueIdx = nTechIdx;
				sPass.numConst = _GetUsedParamNum(m_ParamList, pass);
				sPass.numSampler = _GetUsedParamNum(m_SamplerList, pass);
				VRETURN(CPassUtil::SetVSType(sPass, pass));

				sPass.sizeProgram = _GetFileSize(m_CompiledPSList[nPassIdx]);

				sPass.ann.RenderTargetIdx = _GetRenderTargetIdx(m_TexList, pass);
				sPass.ann.TexOffsetParamIdx = _GetTexOffsetParamIdx(m_ParamList, pass);

				for (IZ_UINT i = 0; i < izanagi::PES_FUNCTOR_ARGS_NUM; i++) {
					sPass.ann.FunctorArgs[i] = -1;
				}

				nConstNum += sPass.numConst;
				nSamplerNum += sPass.numSampler;
			}

			VRETURN(
				CPassUtil::SetAnnValue(
					sPass.ann,
					pass));

			IZ_PCSTR pszFunctorName = CPassUtil::GetFunctorName(pass);

			if (pszFunctorName != IZ_NULL) {
				if (strPrevPassFunctoName == pszFunctorName) {
					sPass.ann.isSubPass = IZ_TRUE;
				}

				strPrevPassFunctoName = pszFunctorName;
			}

			// Stringで設定されているファンクタ引数
			std::vector<izanagi::tool::CString> tvFunctorArgSList;
			CPassUtil::GetFunctorArgsString(
				pass,
				tvFunctorArgSList);

			if (tvFunctorArgSList.size() > 0) {
				ConvertFunctorArgsStrToIndex(
					tvFunctorArgSList,
					sPass.ann);
			}

			VRETURN(
				CPassUtil::SetStateValue(
					sPass.state,
					pass));

			// 出力
			VRETURN(ExportData(sPass));

			pass = ::cgGetNextPass(pass);
			nPassIdx++;

			m_PesHeader.maxProgamSize = IZ_MAX(m_PesHeader.maxProgamSize, sPass.sizeProgram);
			m_PesHeader.numPass++;
		}

		tech = ::cgGetNextTechnique(tech);
		nTechIdx++;
	}

	// Compute size of buffer for indices for parameters which a pass uses.
	m_PesHeader.sizePassBuffForParamIdx = (IZ_UINT)izanagi::CPostEffect::ComputeSizePassBuffForParamIdx(
													nConstNum,
													nSamplerNum);

	return TRUE;
}

void CPostEffectConverter::ConvertFunctorArgsStrToIndex(
	const std::vector<izanagi::tool::CString>& tvFunctorArgSList,
	izanagi::S_PES_PASS_ANN& sParamAnn)
{
	std::vector<CGparameter> tvAllParamList;
	{
		tvAllParamList.reserve(
			m_TexList.size()
			+ m_SamplerList.size()
			+ m_ParamList.size());

		tvAllParamList.insert(tvAllParamList.end(), m_TexList.begin(), m_TexList.end());
		tvAllParamList.insert(tvAllParamList.end(), m_SamplerList.begin(), m_SamplerList.end());
		tvAllParamList.insert(tvAllParamList.end(), m_ParamList.begin(), m_ParamList.end());
	}

	IZ_UINT nPos = 0;

	IZ_UINT nParamIdx = 0;
	IZ_UINT nTexIdx = 0;
	IZ_UINT nSamplerIdx = 0;

	for (size_t nParamPos = 0; nParamPos < tvAllParamList.size(); nParamPos++) {
		CGparameter param = tvAllParamList[nParamPos];
		izanagi::tool::CString strParam(::cgGetParameterName(param));

		IZ_BOOL bIsFind = (std::find(
							tvFunctorArgSList.begin(),
							tvFunctorArgSList.end(),
							strParam) != tvFunctorArgSList.end());

		if (bIsFind) {
			if (CParamUtil::IsParameter(param)) {
				sParamAnn.FunctorArgs[nPos++] = static_cast<IZ_FLOAT>(nParamIdx);
			}
			else if (CParamUtil::IsTexture(param)) {
				sParamAnn.FunctorArgs[nPos++] = static_cast<IZ_FLOAT>(nTexIdx);
			}
			else if (CParamUtil::IsSampler(param)) {
				sParamAnn.FunctorArgs[nPos++] = static_cast<IZ_FLOAT>(nSamplerIdx);
			}
		}

		if (nPos >= izanagi::PES_FUNCTOR_ARGS_NUM) {
			goto __EXIT__;
		}

		if (CParamUtil::IsParameter(param)) {
			nParamIdx++;
		}
		else if (CParamUtil::IsTexture(param)) {
			nTexIdx++;
		}
		else if (CParamUtil::IsSampler(param)) {
			nSamplerIdx++;
		}
	}

__EXIT__:
	return;
}

namespace {
	inline IZ_BOOL _ExportUsedParamIdxByPass(
		izanagi::IOutputStream* pOut,
		const std::vector<CGparameter>& tvParamList,
		CGpass pass)
	{
		std::vector<CGparameter>::const_iterator it = tvParamList.begin();

		for (IZ_UINT nIdx = 0; it != tvParamList.end(); it++, nIdx++) {
			CGparameter param = *it;

			if (::cgIsParameterUsed(param, pass)) {
				IZ_OUTPUT_WRITE_VRETURN(pOut, &nIdx, 0, sizeof(nIdx));
			}
		}

		return IZ_TRUE;
	}
}	// namespace

BOOL CPostEffectConverter::ExportUsedParamAndSamplerIdxByPass()
{
	CGtechnique tech = ::cgGetFirstTechnique(m_pCgEffect);

	while (tech != NULL) {
		CGpass pass = ::cgGetFirstPass(tech);
		while (pass != NULL) {
			VRETURN(ExportUsedParamAndSamplerIdxByPass(pass));

			pass = ::cgGetNextPass(pass);
		}

		tech = ::cgGetNextTechnique(tech);
	}

	return TRUE;
}

BOOL CPostEffectConverter::ExportUsedParamAndSamplerIdxByPass(CGpass pass)
{
	VRETURN(
		_ExportUsedParamIdxByPass(
			&m_Out,
			m_ParamList,
			pass));

	VRETURN(
		_ExportUsedParamIdxByPass(
			&m_Out,
			m_SamplerList,
			pass));

	return TRUE;
}

namespace {
	BOOL _ExportFile(
		const izanagi::tool::CString& strIn,
		izanagi::IOutputStream* pOut)
	{
		static const IZ_UINT BUF_SIZE = 1024;
		static IZ_UINT8 BUF[BUF_SIZE];

		izanagi::CFileInputStream cIn;
		VRETURN(cIn.Open(strIn));

		for (;;) {
			IZ_UINT nReadSize = cIn.Read(BUF, 0, BUF_SIZE);

			IZ_OUTPUT_WRITE_VRETURN(pOut, BUF, 0, nReadSize);

			if (nReadSize != BUF_SIZE) {
				break;
			}
		}

		return TRUE;
	}
}	// namespace

BOOL CPostEffectConverter::ExportPSProgram()
{
	std::vector<izanagi::tool::CString>::const_iterator it = m_CompiledPSList.begin();

	for (; it != m_CompiledPSList.end(); it++) {
		const izanagi::tool::CString strPS = *it;

		VRETURN(_ExportFile(strPS, &m_Out));
	}

	return TRUE;
}

BOOL CPostEffectConverter::ExportVSProgram()
{
	std::vector<SVSInfo>::const_iterator it = m_CompiledVSList.begin();

	std::vector<izanagi::E_POSTEFFECT_VTX_SHADER> tvExportedVSType;
	tvExportedVSType.reserve(m_CompiledVSList.size());

	std::vector<izanagi::E_POSTEFFECT_VTX_SHADER>::const_iterator itType;

	for (; it != m_CompiledVSList.end(); it++) {
		const SVSInfo& sVSInfo = *it;

		// Check whether vertex program is exported.
		itType = std::find(
					tvExportedVSType.begin(),
					tvExportedVSType.end(),
					sVSInfo.type);

		if (itType == tvExportedVSType.end()) {
			// If not exported.
			izanagi::S_PES_VS_HEADER sVSHeader;
			sVSHeader.type = sVSInfo.type;
			sVSHeader.sizeProgram = _GetFileSize(sVSInfo.file);

			IZ_OUTPUT_WRITE_VRETURN(&m_Out, &sVSHeader, 0, sizeof(sVSHeader));

			VRETURN(_ExportFile(sVSInfo.file, &m_Out));

			tvExportedVSType.push_back(sVSInfo.type);

			m_PesHeader.numVtxProgram++;
			m_PesHeader.maxProgamSize = IZ_MAX(m_PesHeader.maxProgamSize, sVSHeader.sizeProgram);
		}
	}

	return TRUE;
}

BOOL CPostEffectConverter::ExportStringBuffer()
{
	m_PesHeader.sizeStringBuffer = CStringChunk::GetInstance().GetBufferSize();

	if (m_PesHeader.sizeStringBuffer > 0) {
		IZ_PCSTR pszStrBuf = CStringChunk::GetInstance().GetBuffer();

		IZ_OUTPUT_WRITE_VRETURN(
			&m_Out,
			pszStrBuf,
			0,
			m_PesHeader.sizeStringBuffer);
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////

namespace {
	// コンパイラ実行
	BOOL _CompileShaderInternal(
		izanagi::tool::CString& out,
		COMPILE_TYPE type,
		LPCSTR lpszCompileCommand,
		LPCSTR lpszShaderFile,
		LPCSTR lpszEntryPoint,
		CGprofile profile)
	{
		// コマンド作成
		izanagi::tool::CString cmd;
		CCompileCmdCreator::GetInstance().CreateCompileCommand(
			cmd, out,
			type,
			lpszCompileCommand,
			lpszShaderFile,
			lpszEntryPoint,
			profile);

		int result = 0;

		if (!cmd.empty()) {
			// TODO
			FILE* fp = _popen(cmd, "w");
			if (fp == NULL) {
				IZ_ASSERT(FALSE);

				// TODO

				return FALSE;
			}

			result = _pclose(fp);
			IZ_ASSERT(result == 0);
		}

		return (result == 0);
	}

	// コンパイラ実行
	BOOL _CompileShader(
		izanagi::tool::CString& out,
		BOOL bIsAsm,
		LPCSTR lpszCompileCommand,
		LPCSTR lpszShaderFile,
		LPCSTR lpszEntryPoint,
		CGprofile profile)
	{
		izanagi::tool::CString tmp = out;

		BOOL ret = FALSE;

		// 通常
		ret = _CompileShaderInternal(
				out,
				COMPILE_TYPE_SHADER,
				lpszCompileCommand,
				lpszShaderFile,
				lpszEntryPoint,
				profile);

		if (ret && bIsAsm) {
			// アセンブラ出力
			ret = _CompileShaderInternal(
					tmp,
					COMPILE_TYPE_SHADER_ASM,
					lpszCompileCommand,
					lpszShaderFile,
					lpszEntryPoint,
					profile);
		}

		return ret;
	}

	// 出力ファイル名を作成
	inline void _MakeOutFile(
		izanagi::tool::CString& strOut,
		UINT nID,
		LPCSTR lpszShaderFile,
		LPCSTR lpszObjDir)
	{
		static CHAR BUF[1024];

		izanagi::tool::CString tmp0;
		tmp0.format("%s", lpszShaderFile);

		// ファイル名取得
		LPSTR file_name = PathFindFileName(tmp0);

		// 拡張子削除
		PathRemoveExtension(file_name);

		izanagi::tool::CString tmp1;
		tmp1.format("%s_%d", file_name, nID);

		if (strlen(lpszObjDir) > 0) {
			// 中間ディレクトリに出力する
			PathCombine(
				BUF,
				lpszObjDir,
				tmp1);

			strOut.format("%s", BUF);
		}
		else {
			strOut.format("%s", tmp1.c_str());
		}
	}
}	// namespace

/**
* シェーダコンパイル
*/
BOOL CPostEffectConverter::CompileShader(
	BOOL bIsAsm,
	LPCSTR lpszCompileCommand,
	LPCSTR lpszShaderFile,
	LPCSTR lpszObjDir)
{
	// ピクセルプログラムをコンパイル
	BOOL b0 = CompilePixelProgram(
				bIsAsm,
				lpszCompileCommand,
				lpszShaderFile,
				lpszObjDir);

	// 頂点プログラムをコンパイル
	BOOL b1 = CompileVertexProgram(
				bIsAsm,
				lpszCompileCommand,
				lpszShaderFile,
				lpszObjDir);

	return (b0 && b1);
}

/**
* ピクセルプログラムをコンパイル
*/
BOOL CPostEffectConverter::CompilePixelProgram(
	BOOL bIsAsm,
	LPCSTR lpszCompileCommand,
	LPCSTR lpszShaderFile,
	LPCSTR lpszObjDir)
{
	CCompileCmdCreator::GetInstance().SetIsVS(FALSE);

	IZ_UINT nPassCnt = 0;

	CGtechnique tech = ::cgGetFirstTechnique(m_pCgEffect);

	while (tech != NULL) {
		CGpass pass = ::cgGetFirstPass(tech);

		while (pass != NULL) {
			// 出力ファイル名を作成
			izanagi::tool::CString strOut;
			_MakeOutFile(
				strOut,
				nPassCnt,
				lpszShaderFile,
				lpszObjDir);

			CGprogram progPS = CPassUtil::GetPSProgram(pass);

			IZ_PCSTR pszEntryPoint = ::cgGetProgramString(progPS, CG_PROGRAM_ENTRY);
			
			IZ_PCSTR pszProfile = ::cgGetProgramString(progPS, CG_PROGRAM_PROFILE);
			CGprofile profile = ::cgGetProfile(pszProfile);

			// コンパイル
			if (!_CompileShader(
					strOut,
					bIsAsm,
					lpszCompileCommand,
					lpszShaderFile,
					pszEntryPoint,
					profile))
			{
				IZ_ASSERT(FALSE);

				// TODO

				return FALSE;
			}

			m_CompiledPSList.push_back(strOut);

			nPassCnt++;
			pass = ::cgGetNextPass(pass);
		}

		tech = ::cgGetNextTechnique(tech);
	}

	return TRUE;
}

/**
* 頂点プログラムをコンパイル
*/
BOOL CPostEffectConverter::CompileVertexProgram(
	BOOL bIsAsm,
	LPCSTR lpszCompileCommand,
	LPCSTR lpszShaderFile,
	LPCSTR lpszObjDir)
{
	CCompileCmdCreator::GetInstance().SetIsVS(TRUE);

	IZ_UINT nPassCnt = 0;

	CGtechnique tech = ::cgGetFirstTechnique(m_pCgEffect);
	while (tech != NULL) {
		CGpass pass = ::cgGetFirstPass(tech);

		while (pass != NULL) {
			SVSInfo sVSInfo;

			// 出力ファイル名を作成
			_MakeOutFile(
				sVSInfo.file,
				nPassCnt,
				lpszShaderFile,
				lpszObjDir);

			CGprogram progVS = CPassUtil::GetVSProgram(pass);

			IZ_PCSTR pszEntryPoint = ::cgGetProgramString(progVS, CG_PROGRAM_ENTRY);
			
			IZ_PCSTR pszProfile = ::cgGetProgramString(progVS, CG_PROGRAM_PROFILE);
			CGprofile profile = ::cgGetProfile(pszProfile);

			// コンパイル
			if (!_CompileShader(
					sVSInfo.file,
					bIsAsm,
					lpszCompileCommand,
					lpszShaderFile,
					pszEntryPoint,
					profile))
			{
				IZ_ASSERT(FALSE);

				// TODO

				return FALSE;
			}

			sVSInfo.type = CPostEffectConvUtil::GetVtxShaderType(pszEntryPoint);

			m_CompiledVSList.push_back(sVSInfo);

			nPassCnt++;
			pass = ::cgGetNextPass(pass);
		}

		tech = ::cgGetNextTechnique(tech);
	}

	return TRUE;
}
