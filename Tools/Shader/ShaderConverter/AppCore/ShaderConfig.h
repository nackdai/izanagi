#if !defined(__SHADER_CONVERTER_SHADER_CONFIG_H__)
#define __SHADER_CONVERTER_SHADER_CONFIG_H__

#include <vector>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include "izDefs.h"
#include "CompileCmdCreator.h"

/////////////////////////////////////////////

struct SShaderConfig {
	CString compile;		// コンパイルコマンド
	CString compile_opt;	// コンパイルオプション

	CString shader;			// コンパイルするシェーダ
	CString out;

	std::vector<CString> defines;	// -Dオプションで指定されたもの
	std::vector<CString> includes;	// -Iオプションで指定されたもの

	struct {
		UINT isCompileAsm	: 1;	// アセンブラ表示のためのコンパイルをするかどうか
	};

	CString preproc_file;	// プリプロセス済みファイル

	CString name;
};

/////////////////////////////////////////////

class CShaderConfigManager : public xercesc::DefaultHandler {
private:
	static CShaderConfigManager s_cInstance;

public:
	static CShaderConfigManager& GetInstance() { return s_cInstance; }

private:
	CShaderConfigManager() { m_pCurConfig = IZ_NULL; }
	~CShaderConfigManager() {}

	NO_COPIABLE(CShaderConfigManager);

public:
	UINT GetConfigNum() const { return static_cast<UINT>(m_tvConfigs.size()); }

	const SShaderConfig& GetConfig(UINT idx) const { return m_tvConfigs[idx]; }
	SShaderConfig& GetConfig(UINT idx) { return m_tvConfigs[idx]; }

public:
	// For SAX

	void startDocument() {}
	void endDeocument() {}

	void startElement(
		const XMLCh* const uri, 
		const XMLCh* const localname, 
		const XMLCh* const qname, 
		const xercesc::Attributes& attrs);

	void endElement(
		const XMLCh* const uri,
		const XMLCh* const localname,
		const XMLCh* const qname);

private:
	void BeginAnalysisShaderElement();
	void EndtAnalysisShaderElement();

	typedef void (CShaderConfigManager::*AnalysisAttrFunc)(const CString&, const CString&);
	inline void AnalysisAttr(
		const xercesc::Attributes& attrs,
		AnalysisAttrFunc func);

	void AnalysisAttrSHD(const CString& strAttrName, const CString& strVal);
	void AnalysisAttrINCLUDE(const CString& strAttrName, const CString& strVal);
	void AnalysisAttrDEFINE(const CString& strAttrName, const CString& strVal);

private:
	std::vector<SShaderConfig> m_tvConfigs;
	SShaderConfig* m_pCurConfig;
};

#endif	// #if !defined(__SHADER_CONVERTER_SHADER_CONFIG_H__)
