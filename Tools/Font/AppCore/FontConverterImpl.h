#if !defined(__FONT_CONVERTER_IMPL_H__)
#define __FONT_CONVERTER_IMPL_H__

#include <vector>
#include <set>
#include "izToolKit.h"
#include "izFont.h"

#define _TEX_FMT_A8_

class CFontConverter {
private:
	static CFontConverter s_cInstance;

public:
	static CFontConverter& GetInstance() { return s_cInstance; }

public:
	struct SOption {
		IZ_UINT fontSize;		// フォントサイズ
		IZ_UINT texWidth;		// テクスチャ幅
		IZ_UINT texHeight;		// テクスチャ高さ

		// アンチエイリアス
		IZ_UINT typeAA;

		izanagi::tool::CString fontFace;

		izanagi::E_FONT_CHAR_ENCODE charEncode;

		struct {
			IZ_UINT isFixedPitch	: 1;	// 固定ピッチ
			IZ_UINT isBold			: 1;	// Bold
			IZ_UINT isItalic		: 1;	// Italic
			IZ_UINT reserved		: 29;
		};
	};

protected:
	CFontConverter();
	~CFontConverter();

	CFontConverter(const CFontConverter& rhs);
	const CFontConverter& operator=(const CFontConverter& rhs);

public:
	// 開放
	void Release();

	// 出力
	BOOL ExportAsDDS();

	// 初期化
	BOOL Init(
		HDC hDC,
		const SOption& sOption);

	// フォントイメージ作成
	BOOL CreateFontImage(
		HDC hDC,
		const SOption& sOption);

	// 出力
	BOOL Export(
		izanagi::IOutputStream* pOut,
		const SOption& sOption);

public:
//	std::vector<LPDIRECT3DTEXTURE9>& GetTexList() { return m_TexList; }

protected:
	// ファイルフォーマットを指定してイメージ出力
	BOOL ExportAs(
		izanagi::tool::CTextureLite* pTex,
		LPCSTR pszName,
		D3DXIMAGE_FILEFORMAT nFileFmt);

	// イメージデータ部出力
	UINT ExportFontImageData(izanagi::IOutputStream* pOut);

private:
#ifdef _TEX_FMT_A8_
	static const D3DFORMAT D3D_TEX_FMT = D3DFMT_A8;
	static const izanagi::E_GRAPH_PIXEL_FMT IZ_TEX_FMT = izanagi::E_GRAPH_PIXEL_FMT_A8;
	static const UINT TEX_BPP = 1;
#else
	static const D3DFORMAT D3D_TEX_FMT = D3DFMT_A8B8G8R8;
	static const izanagi::E_GRAPH_PIXEL_FMT IZ_TEX_FMT = izanagi::E_GRAPH_PIXEL_FMT_BGRA8
	static const UINT TEX_BPP = 4;
#endif
	static const IZ_INT MARGIN = 0;
		
private:
	std::vector<izanagi::tool::CTextureLite*> m_TexList;

	HFONT m_hFont;
	HFONT m_hOldFont;

	struct {
		IZ_UINT height;
		IZ_UINT ascent;
	} m_TexMetrics;

	// フォントイメージ情報リスト
	std::vector<izanagi::S_FNT_IMAGE> m_FontImgList;

	// フォントマップ情報リスト
	std::vector<izanagi::S_FNT_MAP> m_FontMapList;
};

#endif	// #if !defined(__FONT_CONVERTER_IMPL_H__)
