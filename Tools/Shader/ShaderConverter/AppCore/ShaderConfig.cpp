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
	izanagi::izanagi_tk::CString name(XN(qname));

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
	izanagi::izanagi_tk::CString name(XN(qname));

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
		if (!sConfig.shader.empty()) {
			VRETURN(
				izanagi::izanagi_tk::CEnvVarHelper::ExpandEnvStrings(
					s_BUF,
					sizeof(s_BUF),
					sConfig.shader));

			sConfig.shader.format("%s", s_BUF);
			sConfig.shader.replace('\\', '/');
		}

		if (sConfig.compiler.empty()) {
			sConfig.compiler.format("%%DXSDK_DIR%%Utilities/Bin/x86/fxc");
		}

		if (!sConfig.compiler.empty()) {
			VRETURN(
				izanagi::izanagi_tk::CEnvVarHelper::ExpandEnvStrings(
					s_BUF,
					sizeof(s_BUF),
					sConfig.compiler));

			sConfig.compiler.format("%s", s_BUF);
			sConfig.compiler.replace('\\', '/');

			// コンパイルオプションとの結合
			izanagi::izanagi_tk::CString tmp;
			tmp.format("\"\"%s\"\" %s", sConfig.compiler.c_str(), sConfig.compile_opt.c_str());
			sConfig.compiler = tmp;
		}

		if (sConfig.name.empty()) {
			VRETURN(
				izanagi::izanagi_tk::CFileUtility::GetFileNameFromPathWithoutExt(
					s_BUF,
					sizeof(s_BUF),
					sConfig.shader));

			sConfig.name.format("%s", s_BUF);
		}

		// includeパスの環境変数の展開
		std::vector<izanagi::izanagi_tk::CString>::iterator it = sConfig.includes.begin();
		while (it != sConfig.includes.end()) {
			izanagi::izanagi_tk::CString& str = *it;

			VRETURN(
				izanagi::izanagi_tk::CEnvVarHelper::ExpandEnvStrings(
					s_BUF,
					sizeof(s_BUF),
					str));
			
			str.format("%s", s_BUF);
			str.replace('\\', '/');
			
			it++;
		}

		return TRUE;
	}
}	// namespace

void CShaderConfigManager::EndtAnalysisShaderElement()
{
	IZ_ASSERT(m_pCurConfig != NULL);

	IZ_VERIFY(_AnalysisConfig(*m_pCurConfig));
	m_pCurConfig = IZ_NULL;
}

void CShaderConfigManager::AnalysisAttr(
	const xercesc::Attributes& attrs,
	CShaderConfigManager::AnalysisAttrFunc func)
{
	IZ_ASSERT(func != IZ_NULL);

	UINT nAttrNum = (UINT)attrs.getLength();

	for (UINT i = 0; i < nAttrNum; i++) {
		izanagi::izanagi_tk::CString strAttrName(XN(attrs.getQName(i)));

		izanagi::izanagi_tk::CString val(XN(attrs.getValue(i)));

		(this->*func)(strAttrName, val);
	}
}

namespace {
	inline BOOL _CheckBoolValue(izanagi::izanagi_tk::CString str)
	{
		str.make_lower();

		BOOL ret = ((str == "1") || (str == "true"));
		return ret;
	}
}	// namespace

void CShaderConfigManager::AnalysisAttrSHD(const izanagi::izanagi_tk::CString& strAttrName, const izanagi::izanagi_tk::CString& strVal)
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
			m_pCurConfig->shader.format("%s\\%s", m_BaseDir.c_str(), strVal.c_str());
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

void CShaderConfigManager::AnalysisAttrINCLUDE(const izanagi::izanagi_tk::CString& strAttrName, const izanagi::izanagi_tk::CString& strVal)
{
#if 0
	m_pCurConfig->includes.push_back(strVal);
#else
	m_pCurConfig->includes.push_back(izanagi::izanagi_tk::CString());

	izanagi::izanagi_tk::CString& str = m_pCurConfig->includes.back();
	if (m_BaseDir.empty()) {
		str = strVal;
	}
	else {
		str.format("%s\\%s", m_BaseDir.c_str(), strVal.c_str());
	}
#endif
}

void CShaderConfigManager::AnalysisAttrDEFINE(const izanagi::izanagi_tk::CString& strAttrName, const izanagi::izanagi_tk::CString& strVal)
{
	m_pCurConfig->defines.push_back(strVal);
}

// 相対パスを絶対パスにするためのベースとなるディレクトリを設定
IZ_BOOL CShaderConfigManager::SetBaseDir(IZ_PCSTR pszDir)
{
	static izChar BUF[MAX_PATH];

	if (m_BaseDir.empty()) {
		izanagi::izanagi_tk::CString str(pszDir);
		str.replace('/', '\\');

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
