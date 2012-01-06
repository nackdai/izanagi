#if !defined(__IMAGE_BUILDER_IMPL_H__)
#define __IMAGE_BUILDER_IMPL_H__

#include <vector>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include "ImageBuilderDefs.h"
#include "izToolKit.h"

// イメージビルダー
class CImageBuilder : public xercesc::DefaultHandler {
private:
	static CImageBuilder s_cInstance;

public:
	static CImageBuilder& GetInstance() { return s_cInstance; }

private:
	CImageBuilder() {}
	~CImageBuilder() {}

	CImageBuilder(const CImageBuilder& rhs);
	const CImageBuilder& operator=(const CImageBuilder& rhs);

public:
	// For SAX

	void startDocument();
	void endDeocument();

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
	// 属性セット
	void SetPlaneAttrs(const xercesc::Attributes& attrs);

	// 共通属性セット
	void SetCommonAttrs(
		SImageInfo* pImageInfo,
		const xercesc::Attributes& attrs);

public:
	// IMGデータ作成
	BOOL BuildIMG(LPCSTR lpszExport);

	void SetBasePath(const char* path);

private:
	std::vector<SImageInfo> m_ImageInfoList;
	izanagi::tool::CString m_BasePath;

	// ルートが正しく設定されているかどうか
	IZ_BOOL m_IsStartRoot;
};

#endif	// #if !defined(__IMAGE_BUILDER_IMPL_H__)
