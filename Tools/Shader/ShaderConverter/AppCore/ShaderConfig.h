#if !defined(__SHADER_CONVERTER_SHADER_CONFIG_H__)
#define __SHADER_CONVERTER_SHADER_CONFIG_H__

#include <vector>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include "izDefs.h"
#include "CompileCmdCreator.h"

/////////////////////////////////////////////

struct SShaderConfig {
	izanagi::tool::CString compiler;		// コンパイルコマンド
	izanagi::tool::CString compile_opt;	// コンパイルオプション

	izanagi::tool::CString shader;			// コンパイルするシェーダ
	izanagi::tool::CString out;

	std::vector<izanagi::tool::CString> defines;	// -Dオプションで指定されたもの
	std::vector<izanagi::tool::CString> includes;	// -Iオプションで指定されたもの

	struct {
		UINT isCompileAsm	: 1;	// アセンブラ表示のためのコンパイルをするかどうか
	};

	izanagi::tool::CString preproc_file;	// プリプロセス済みファイル

	izanagi::tool::CString name;
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

public:
	// 相対パスを絶対パスにするためのベースとなるディレクトリを設定
	IZ_BOOL SetBaseDir(IZ_PCSTR pszDir);

private:
	void BeginAnalysisShaderElement();
	void EndtAnalysisShaderElement();

	typedef void (CShaderConfigManager::*AnalysisAttrFunc)(const izanagi::tool::CString&, const izanagi::tool::CString&);
	inline void AnalysisAttr(
		const xercesc::Attributes& attrs,
		AnalysisAttrFunc func);

	void AnalysisAttrSHD(const izanagi::tool::CString& strAttrName, const izanagi::tool::CString& strVal);
	void AnalysisAttrINCLUDE(const izanagi::tool::CString& strAttrName, const izanagi::tool::CString& strVal);
	void AnalysisAttrDEFINE(const izanagi::tool::CString& strAttrName, const izanagi::tool::CString& strVal);

private:
	std::string m_BaseDir;

	std::vector<SShaderConfig> m_tvConfigs;
	SShaderConfig* m_pCurConfig;
};

#endif	// #if !defined(__SHADER_CONVERTER_SHADER_CONFIG_H__)
