#include <stdafx.h>
#include "ShaderConfig.h"
#include "izToolKit.h"
#include "ToolKitXmlDefs.h"

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
		BeginAnalysisShaderElement();

		AnalysisAttr(
			attrs,
			&CShaderConfigManager::AnalysisAttrSHD);
	}
	else if (IsAttr(name, attr_type::INCLUDE)) {
		AnalysisAttr(
			attrs,
			&CShaderConfigManager::AnalysisAttrINCLUDE);
	}
	else if (IsAttr(name, attr_type::DEFINE)) {
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
					sConfig.shader,
					s_BUF,
					sizeof(s_BUF)));

			sConfig.shader.Format(_T("%s"), s_BUF);
			sConfig.shader.Replace('\\', '/');
		}

		if (sConfig.compile.IsEmpty()) {
			sConfig.compile.Format(_T("%%DXSDK_DIR%%Utilities/Bin/x86/fxc"));
		}

		if (!sConfig.compile.IsEmpty()) {
			VRETURN(
				izanagi::izanagi_tk::CEnvVarHelper::ExpandEnvStrings(
					sConfig.compile,
					s_BUF,
					sizeof(s_BUF)));

			sConfig.compile.Format(_T("%s"), s_BUF);
			sConfig.compile.Replace('\\', '/');

			// コンパイルオプションとの結合
			CString tmp;
			tmp.Format(_T("\"\"%s\"\" %s"), sConfig.compile, sConfig.compile_opt);
			sConfig.compile = tmp;
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
					str,
					s_BUF,
					sizeof(s_BUF)));
			
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
		m_pCurConfig->compile = strVal;
	}
	else if (IsAttr(strAttrName, attr_type::SRC)) {
		m_pCurConfig->shader = strVal;
	}
	else if (IsAttr(strAttrName, attr_type::OPTION)) {
		m_pCurConfig->compile_opt = strVal;
	}
	else if (IsAttr(strAttrName, attr_type::IS_ASM)) {
		m_pCurConfig->isCompileAsm = _CheckBoolValue(strVal);
	}
	else if (IsAttr(strAttrName, attr_type::NAME)) {
		m_pCurConfig->name = strVal;
	}
}

void CShaderConfigManager::AnalysisAttrINCLUDE(const CString& strAttrName, const CString& strVal)
{
	m_pCurConfig->includes.push_back(strVal);
}

void CShaderConfigManager::AnalysisAttrDEFINE(const CString& strAttrName, const CString& strVal)
{
	m_pCurConfig->defines.push_back(strVal);
}
