#include <stdafx.h>
#include "ShaderConfig.h"
#include "izToolKit.h"

CShaderConfigManager CShaderConfigManager::s_cInstance;

//////////////////////////////////////////////////////////////////////
// For SAX

namespace attr_type {
	const char* SHD     = "shd";
	const char* INCLUDE = "include";
	const char* DEFINE  = "define";

	const char* NAME     = "name";
	const char* COMPILER = "compiler";
	const char* SRC      = "src";
	const char* OPTION   = "option";
	const char* IS_ASM   = "is_asm";
}	// namespace attr_type

#define IsAttr(str, type)	(memcmp((str), (type), strlen(type)) == 0)

void CShaderConfigManager::startElement(
	const XMLCh* const uri, 
	const XMLCh* const localname, 
	const XMLCh* const qname, 
	const xercesc::Attributes& attrs)
{
	CString name(XN(qname));

	if (IsAttr(name, attr_type::SHD)) {
		// shd
		BeginAnalysisShaderElement();

		AnalysisAttr(
			attrs,
			&CShaderConfigManager::AnalysisAttrSHD);
	}
	else if (IsAttr(name, attr_type::INCLUDE)) {
		// include
		AnalysisAttr(
			attrs,
			&CShaderConfigManager::AnalysisAttrINCLUDE);
	}
	else if (IsAttr(name, attr_type::DEFINE)) {
		// define
		AnalysisAttr(
			attrs,
			&CShaderConfigManager::AnalysisAttrDEFINE);
	}
}

void CShaderConfigManager::endElement(
	const XMLCh* const uri,
	const XMLCh* const localname,
	const XMLCh* const qname)
{
	CString name(XN(qname));

	if (IsAttr(name, attr_type::SHD)) {
		EndtAnalysisShaderElement();
	}
}

void CShaderConfigManager::BeginAnalysisShaderElement()
{
	m_tvConfigs.push_back(SShaderConfig());

	SShaderConfig& sConfig = m_tvConfigs[m_tvConfigs.size() - 1];
	m_pCurConfig = &sConfig;

	m_pCurConfig->includes.clear();
	m_pCurConfig->defines.clear();
}

namespace {
	CHAR s_BUF[1024]; 

	BOOL _AnalysisConfig(SShaderConfig& sConfig)
	{
		// 環境変数の展開
		if (!sConfig.shader.IsEmpty()) {
			VRETURN(
				izanagi::izanagi_tk::CEnvVarHelper::ExpandEnvStrings(
					s_BUF,
					sizeof(s_BUF),
					sConfig.shader));

			sConfig.shader.Format(_T("%s"), s_BUF);
			sConfig.shader.Replace('\\', '/');
		}

		if (sConfig.compiler.IsEmpty()) {
			sConfig.compiler.Format(_T("%%DXSDK_DIR%%Utilities/Bin/x86/fxc"));
		}

		if (!sConfig.compiler.IsEmpty()) {
			VRETURN(
				izanagi::izanagi_tk::CEnvVarHelper::ExpandEnvStrings(
					s_BUF,
					sizeof(s_BUF),
					sConfig.compiler));

			sConfig.compiler.Format(_T("%s"), s_BUF);
			sConfig.compiler.Replace('\\', '/');

			// コンパイルオプションとの結合
			CString tmp;
			tmp.Format(_T("\"\"%s\"\" %s"), sConfig.compiler, sConfig.compile_opt);
			sConfig.compiler = tmp;
		}

		if (sConfig.name.IsEmpty()) {
			VRETURN(
				izanagi::izanagi_tk::CFileUtility::GetFileNameFromPathWithoutExt(
					s_BUF,
					sizeof(s_BUF),
					sConfig.shader));

			sConfig.name.Format(_T("%s"), s_BUF);
		}

		// includeパスの環境変数の展開
		std::vector<CString>::iterator it = sConfig.includes.begin();
		while (it != sConfig.includes.end()) {
			CString& str = *it;

			VRETURN(
				izanagi::izanagi_tk::CEnvVarHelper::ExpandEnvStrings(
					s_BUF,
					sizeof(s_BUF),
					str));
			
			str.Format(_T("%s"), s_BUF);
			str.Replace('\\', '/');
			
			it++;
		}

		return TRUE;
	}
}	// namespace

void CShaderConfigManager::EndtAnalysisShaderElement()
{
	IZ_ASSERT(m_pCurConfig != NULL);

	IZ_ASSERT(_AnalysisConfig(*m_pCurConfig));
	m_pCurConfig = IZ_NULL;
}

void CShaderConfigManager::AnalysisAttr(
	const xercesc::Attributes& attrs,
	CShaderConfigManager::AnalysisAttrFunc func)
{
	IZ_ASSERT(func != IZ_NULL);

	UINT nAttrNum = (UINT)attrs.getLength();

	for (UINT i = 0; i < nAttrNum; i++) {
		CString strAttrName(XN(attrs.getQName(i)));

		CString val(XN(attrs.getValue(i)));

		(this->*func)(strAttrName, val);
	}
}

namespace {
	inline BOOL _CheckBoolValue(CString str)
	{
		str.MakeLower();

		BOOL ret = ((str == "1") || (str == "true"));
		return ret;
	}
}	// namespace

void CShaderConfigManager::AnalysisAttrSHD(const CString& strAttrName, const CString& strVal)
{
	if (IsAttr(strAttrName, attr_type::COMPILER)) {
		// compiler
		m_pCurConfig->compiler = strVal;
	}
	else if (IsAttr(strAttrName, attr_type::SRC)) {
		// src
#if 0
		m_pCurConfig->shader = strVal;
#else
		if (m_BaseDir.empty()) {
			m_pCurConfig->shader = strVal;
		}
		else {
			m_pCurConfig->shader.Format(_T("%s\\%s"), m_BaseDir.c_str(), strVal);
		}
#endif
	}
	else if (IsAttr(strAttrName, attr_type::OPTION)) {
		// option
		m_pCurConfig->compile_opt = strVal;
	}
	else if (IsAttr(strAttrName, attr_type::IS_ASM)) {
		// is_asm
		m_pCurConfig->isCompileAsm = _CheckBoolValue(strVal);
	}
	else if (IsAttr(strAttrName, attr_type::NAME)) {
		// name
		m_pCurConfig->name = strVal;
	}
}

void CShaderConfigManager::AnalysisAttrINCLUDE(const CString& strAttrName, const CString& strVal)
{
#if 0
	m_pCurConfig->includes.push_back(strVal);
#else
	m_pCurConfig->includes.push_back(CString());

	CString& str = m_pCurConfig->includes.back();
	if (m_BaseDir.empty()) {
		str = strVal;
	}
	else {
		str.Format(_T("%s\\%s"), m_BaseDir.c_str(), strVal);
	}
#endif
}

void CShaderConfigManager::AnalysisAttrDEFINE(const CString& strAttrName, const CString& strVal)
{
	m_pCurConfig->defines.push_back(strVal);
}

// 相対パスを絶対パスにするためのベースとなるディレクトリを設定
IZ_BOOL CShaderConfigManager::SetBaseDir(IZ_PCSTR pszDir)
{
	static izChar BUF[MAX_PATH];

	if (m_BaseDir.empty()) {
		CString str(pszDir);
		str.Replace("/", "\\");

		VRETURN(
			izanagi::izanagi_tk::CFileUtility::GetPathWithoutFileName(
				BUF,
				sizeof(BUF),
				str));

		//m_BaseDir.clear();
		m_BaseDir.append(BUF);
	}

	return IZ_TRUE;
}
