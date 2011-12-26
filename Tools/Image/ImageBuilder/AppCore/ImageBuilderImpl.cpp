#include "ImageBuilderImpl.h"
#include "ImageReaderImpl.h"
#include "izImageLib.h"
#include "AppError.h"
#include "izToolKit.h"

CImageBuilder CImageBuilder::s_cInstance;

void CImageBuilder::startDocument()
{
}

void CImageBuilder::endDeocument()
{
}

namespace attr_type {
	static const char* PLANE  = "plane";
	static const char* CUBE   = "cube";
	static const char* VOLUME = "volume";

	static const char* PATH       = "path";
	static const char* FMT        = "fmt";
	static const char* MIPMAP     = "mipmap";
	static const char* ADDR_U     = "addrU";
	static const char* ADDR_V     = "addrV";
	static const char* MIN_FILTER = "filterMin";
	static const char* MAG_FILTER = "filterMag";
	static const char* MIP_FILTER = "filterMip";
	static const char* ANISO_NUM  = "numAniso";

	static const char* TEX_TYPE   = "type";

	static const char* TEX_IDX    = "texIdx";
}	// namespace attr_type

#define IsAttr(str, type)	(memcmp((str), (type), strlen(type)) == 0)

void CImageBuilder::startElement(
	const XMLCh* const uri, 
	const XMLCh* const localname, 
	const XMLCh* const qname, 
	const xercesc::Attributes& attrs)
{
	izanagi::tool::CString name(XN(qname));

	if (IsAttr(name, attr_type::CUBE)) {
		// TODO
	}
	else if (IsAttr(name, attr_type::VOLUME)) {
		// TODO
	}
	else if (IsAttr(name, attr_type::PLANE)) {
		// file
		SetPlaneAttrs(attrs);
	}
}

void CImageBuilder::endElement(
	const XMLCh* const uri,
	const XMLCh* const localname,
	const XMLCh* const qname)
{
}

// 属性セット
void CImageBuilder::SetPlaneAttrs(const xercesc::Attributes& attrs)
{
	SImageInfo sImageInfo;

	SetCommonAttrs(&sImageInfo, attrs);

	m_ImageInfoList.push_back(sImageInfo);
}

namespace {
	// 比較方法
	enum CMP_TYPE {
		CMP_TYPE_LOWER = 0,
		CMP_TYPE_UPPER,
		CMP_TYPE_NORMAL,

		CMP_TYPE_NUM,
	};

	inline void _ConvString(izanagi::tool::CString& str, CMP_TYPE typeCmp)
	{
		switch (typeCmp) {
		case CMP_TYPE_LOWER:
			str.make_lower();
			break;
		case CMP_TYPE_UPPER:
			str.make_upper();
			break;
		case CMP_TYPE_NORMAL:
			break;
		}
	}

	template <typename _T>
	_T _ConvString(
		LPCSTR lpszStr,
		LPCSTR lpszTable[],
		UINT nTableNum,
		CMP_TYPE typeCmp)
	{
		_T ret = (_T)0;

		izanagi::tool::CString str(lpszStr);
		_ConvString(str, typeCmp);

		size_t nStrLen = strlen(lpszStr);

		for (UINT i = 0; i < nTableNum; i++) {
			izanagi::tool::CString tmp(lpszTable[i]);
			_ConvString(tmp, typeCmp);

			if (str == tmp) {
				ret = static_cast<_T>(i);
				break;
			}
		}

		return ret;
	}

	// 文字列 -> PixelFormat
	inline izanagi::E_GRAPH_PIXEL_FMT _ConvTextToPixelFormat(LPCSTR lpszStr)
	{
		static const char* str[] = {
			"rgba8",	// E_GRAPH_PIXEL_FMT_RGBA8
			"bgra8",	// E_GRAPH_PIXEL_FMT_BGRA8

			"rgba4",	// E_GRAPH_PIXEL_FMT_RGBA4
			"rgb10A2",	// E_GRAPH_PIXEL_FMT_RGB10A2
			"a8",		// E_GRAPH_PIXEL_FMT_A8
			
			"r32f",		// E_GRAPH_PIXEL_FMT_R32F
			"rgba16f",	// E_GRAPH_PIXEL_FMT_RGBA16F
			"rgba32f",	// E_GRAPH_PIXEL_FMT_RGBA32F

			"dxt1",		// E_GRAPH_PIXEL_FMT_DXT1
			"dxt3",		// E_GRAPH_PIXEL_FMT_DXT3
			"dxt5",		// E_GRAPH_PIXEL_FMT_DXT5

			"rgbx8",	// E_GRAPH_PIXEL_FMT_RGBX8
			"d24s8",	// E_GRAPH_PIXEL_FMT_D24S8
		};
		C_ASSERT(COUNTOF(str) == izanagi::E_GRAPH_PIXEL_FMT_NUM);

		izanagi::E_GRAPH_PIXEL_FMT ret = _ConvString<izanagi::E_GRAPH_PIXEL_FMT>(
											lpszStr,
											str,
											COUNTOF(str),
											CMP_TYPE_LOWER);
		return ret;
	}

	// 文字列 -> TexAddress
	inline izanagi::E_GRAPH_TEX_ADDRESS _ConvTextToTexAddr(LPCSTR lpszStr)
	{
		static const char* str[] = {
			"wrap",			// E_GRAPH_TEX_ADDRESS_WRAP
			"mirror",		// E_GRAPH_TEX_ADDRESS_MIRROR
			"clamp",		// E_GRAPH_TEX_ADDRESS_CLAMP
			"border",		// E_GRAPH_TEX_ADDRESS_BORDER
			"mirroronce",	// E_GRAPH_TEX_ADDRESS_MIRRORONCE
		};
		C_ASSERT(COUNTOF(str) == izanagi::E_GRAPH_TEX_ADDRESS_NUM);

		izanagi::E_GRAPH_TEX_ADDRESS ret = _ConvString<izanagi::E_GRAPH_TEX_ADDRESS>(
											lpszStr,
											str,
											COUNTOF(str),
											CMP_TYPE_LOWER);
		return ret;
	}

	// 文字列 -> TexFilter
	inline izanagi::E_GRAPH_TEX_FILTER _ConvTextToTexFilter(LPCSTR lpszStr)
	{
		static const char* str[] = {
			"none",			// E_GRAPH_TEX_FILTER_NONE
			"point",		// E_GRAPH_TEX_FILTER_POINT
			"linear",		// E_GRAPH_TEX_FILTER_LINEAR
			"aniso",		// E_GRAPH_TEX_FILTER_ANISOTROPIC
			"pyramidquad",	// E_GRAPH_TEX_FILTER_PYRAMIDALQUAD
			"gaussquad",	// E_GRAPH_TEX_FILTER_GAUSSIANQUAD
		};
		C_ASSERT(COUNTOF(str) == izanagi::E_GRAPH_TEX_FILTER_NUM);

		izanagi::E_GRAPH_TEX_FILTER ret = _ConvString<izanagi::E_GRAPH_TEX_FILTER>(
											lpszStr,
											str,
											COUNTOF(str),
											CMP_TYPE_LOWER);
		return ret;
	}

	// 文字列 -> TexType
	inline izanagi::E_GRAPH_TEX_TYPE _ConvTextToTexType(LPCSTR lpszStr)
	{
		static const char* str[] = {
			"plane",
			"cube",
			"volume",
		};
		C_ASSERT(COUNTOF(str) == izanagi::E_GRAPH_TEX_TYPE_NUM);

		izanagi::E_GRAPH_TEX_TYPE ret = _ConvString<izanagi::E_GRAPH_TEX_TYPE>(
											lpszStr,
											str,
											COUNTOF(str),
											CMP_TYPE_LOWER);
		return ret;
	}
}	// namespace

// 共通属性セット
void CImageBuilder::SetCommonAttrs(
	SImageInfo* pImageInfo,
	const xercesc::Attributes& attrs)
{
	UINT nAttrNum = (UINT)attrs.getLength();

	for (UINT i = 0; i < nAttrNum; i++) {
		izanagi::tool::CString strAttrName(XN(attrs.getQName(i)));

		izanagi::tool::CString val(XN(attrs.getValue(i)));

		if (IsAttr(strAttrName, attr_type::PATH)) {
			// path
#if 0
			pImageInfo->path = val;
#else
			if (m_BasePath.empty()) {
				pImageInfo->path = val;
			}
			else {
				pImageInfo->path.format(
					"%s\\%s",
					m_BasePath.c_str(),
					val);
			}
#endif
		}
		else {
			// 小文字にする
			val = val.make_lower();

			if (IsAttr(strAttrName, attr_type::FMT)) {
				// fmt
				pImageInfo->info.fmt = _ConvTextToPixelFormat(val);
			}
			else if (IsAttr(strAttrName, attr_type::MIPMAP)) {
				// mipmap
				pImageInfo->info.level = atoi(val);
				if (pImageInfo->info.level == 0) {
					// ０はダメ
					pImageInfo->info.level = 1;
				}
			}
			else if (IsAttr(strAttrName, attr_type::ADDR_U)) {
				// addrU
				pImageInfo->info.addressU = _ConvTextToTexAddr(val);
			}
			else if (IsAttr(strAttrName, attr_type::ADDR_V)) {
				// addrV
				pImageInfo->info.addressV = _ConvTextToTexAddr(val);
			}
			else if (IsAttr(strAttrName, attr_type::MIN_FILTER)) {
				// filterMin
				pImageInfo->info.minFilter = _ConvTextToTexFilter(val);
			}
			else if (IsAttr(strAttrName, attr_type::MAG_FILTER)) {
				// filterMag
				pImageInfo->info.magFilter = _ConvTextToTexFilter(val);
			}
			else if (IsAttr(strAttrName, attr_type::MIP_FILTER)) {
				// filterMip
				pImageInfo->info.mipFilter = _ConvTextToTexFilter(val);
			}
			else if (IsAttr(strAttrName, attr_type::ANISO_NUM)) {
				// numAniso
				pImageInfo->info.anisoNum = atoi(val);
			}
			else if (IsAttr(strAttrName, attr_type::ANISO_NUM)) {
				// texIdx
				pImageInfo->tex_idx = atoi(val);
			}
			else if (IsAttr(strAttrName, attr_type::TEX_TYPE)) {
				// type
				pImageInfo->info.type = _ConvTextToTexType(val);
			}
		}
	}
}

namespace {
	// テクスチャ出力
	BOOL _ExportTex(
		izanagi::IOutputStream* pOut,
		izanagi::S_IMG_HEADER* pHeader,
		const SImageInfo& sImageInfo,
		izanagi::tool::CIMGTexture* pTex)
	{
		IZ_ASSERT(pOut != NULL);
		IZ_ASSERT(pHeader != NULL);

		BOOL result = TRUE;

		// フォーマット変換
		result = pTex->ConvertPixelFormat(sImageInfo.info.fmt);
		THROW_EXCEPTION(
			result,
			CErrorLog,
			"Failed : ConvertPixelFormat(%d)",
			sImageInfo.info.fmt);

		// MIPMAP作成
		result = pTex->CreateMipMap(sImageInfo.info.level);
		THROW_EXCEPTION(
			result,
			CErrorLog,
			"Failed : CreateMipMap(%d)",
			sImageInfo.info.level);

		pTex->SetTexAddress(
			sImageInfo.info.addressU, 
			sImageInfo.info.addressV);
		pTex->SetTexFilter(
			sImageInfo.info.minFilter,
			sImageInfo.info.magFilter,
			sImageInfo.info.mipFilter);

		// 最大サイズ
		IZ_UINT nSize = pTex->GetExportSize();
		pHeader->sizeMax = IZ_MAX(nSize, pHeader->sizeMax);

		// 出力
		result = pTex->Write(pOut);
		THROW_EXCEPTION(
			result,
			CErrorLog,
			"Failed : Export",
			sImageInfo.info.level);

		return result;
	}
}	// namespace

// IMGデータ作成
BOOL CImageBuilder::BuildIMG(LPCSTR lpszExport)
{
	izanagi::CFileOutputStream cOut;
	THROW_EXCEPTION(
		cOut.Open(lpszExport),
		CErrorLog,
		"Failed : Open File(%s)",
		lpszExport);

	// DirectXテクスチャ読み込み処理をセット
	izanagi::tool::CImageReader::GetInstance().SetReadFunc(
		CImageReaderImpl::Read);

	BOOL ret = FALSE;

	// テクスチャ数
	IZ_UINT nTexNum = static_cast<IZ_UINT>(m_ImageInfoList.size());

	// ヘッダ
	izanagi::S_IMG_HEADER sHeader;
	izanagi::tool::CImageUtil::SetImageHeader(
		&sHeader,
		nTexNum);

	// ジャンプテーブル
	std::vector<UINT> tJumpTable;
	tJumpTable.reserve(nTexNum);

	// ヘッダ + ジャンプテーブル分空ける
	ret = cOut.Seek(
			sizeof(izanagi::S_IMG_HEADER) + sizeof(IZ_UINT) * nTexNum, 
			izanagi::E_IO_STREAM_SEEK_POS_START);
	VGOTO(ret, __EXIT__);

	for (UINT i = 0; i < m_ImageInfoList.size(); i++) {
		const SImageInfo& sImageInfo = m_ImageInfoList[i];

		// ジャンプテーブル
		// 出力先の現在位置がイメージデータの位置（ジャンプ先）になる
		tJumpTable.push_back(cOut.GetSize());

		// テクスチャ読み込み
		izanagi::tool::CIMGBody* pImgBody = NULL;
		pImgBody = izanagi::tool::CImageReader::GetInstance().Read(
					sImageInfo.path,
					static_cast<izanagi::E_GRAPH_TEX_TYPE>(sImageInfo.info.type));

		if (pImgBody == IZ_NULL) {
			continue;
		}

		UINT nNum = pImgBody->GetTexNum();
		
		if (sImageInfo.tex_idx >= 0) {
			// テクスチャ位置指定

			INT nTexIdx = IZ_MAX(nNum - 1, sImageInfo.tex_idx);
			izanagi::tool::CIMGTexture* pTex = pImgBody->GetTexture(nTexIdx);

			if (pTex != IZ_NULL) {
				// テクスチャ出力
				ret = _ExportTex(
						&cOut,
						&sHeader,
						sImageInfo,
						pTex);
				VGOTO(ret, __EXIT__);
			}
		}
		else {
			// 全テクスチャ

			for (UINT n = 0; n < nNum; n++) {
				izanagi::tool::CIMGTexture* pTex = pImgBody->GetTexture(n);

				if (pTex != IZ_NULL) {
					// テクスチャ出力
					ret = _ExportTex(
							&cOut,
							&sHeader,
							sImageInfo,
							pTex);
					VGOTO(ret, __EXIT__);
				}
			}
		}

		// 削除
		izanagi::tool::CImageReader::GetInstance().Delete(pImgBody);
	}

	// ファイルサイズ取得
	sHeader.sizeFile = cOut.GetCurPos();

	// 先頭に戻る
	ret = cOut.Seek(0, izanagi::E_IO_STREAM_SEEK_POS_START);
	VGOTO(ret, __EXIT__);

	// ヘッダ出力
	ret = IZ_OUTPUT_WRITE(&cOut, &sHeader, 0, sizeof(sHeader));
	VGOTO(ret, __EXIT__);

	// ジャンプテーブル出力
	if (tJumpTable.size() > 0) {
		ret = IZ_OUTPUT_WRITE(&cOut, &tJumpTable[0], 0, sizeof(IZ_UINT) * tJumpTable.size());
		VGOTO(ret, __EXIT__);
	}

__EXIT__:
	cOut.Finalize();

	return ret;
}

void CImageBuilder::SetBasePath(const char* path)
{
	m_BasePath.format("%s", path);
}
