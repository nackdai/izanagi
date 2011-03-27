#if !defined(__IZANAGI_FONT_FNT_ENCODE_H__)
#define __IZANAGI_FONT_FNT_ENCODE_H__

#include "izDefs.h"
#include "graph/GraphDefs.h"
#include "FontDefs.h"

// FNT = FoNT

namespace izanagi {
	// FNTファイルバージョン
	enum E_FNT_VERSION {
		E_FNT_VERSION_0010 = 0x30303130,				// バージョン0010

		E_FNT_VERSION_CURRENT = E_FNT_VERSION_0010,		// 現在のバージョン
		E_FNT_VERSION_FORCE_INT32 = 0x7FFFFFFF,			// enum のサイズを IZ_INT と同じにするため.
	};

	enum {
		// "FNT "
		FNT_MAGIC_NUMBER = 0x464E5400,
	};

	enum {
		// フォントサイズ最大
		// これだけあればいいでしょ
		FNT_MAX_FONT_SIZE = 64,

		// 文字数最大
		// これだけあればいいでしょ
		FNT_MAX_CHAR_NUM = 65535,
	};

	// NOTE
	// フォーマット
	// +----------------+
	// | ファイルヘッダ |
	// +----------------+
	// |   マップ情報   |
	// +----------------+-+
	// |フォントイメージ| |
	// |      情報      | |
	// +----------------+ +-- データ個数並ぶ
	// |    フォント    | |
	// | イメージデータ | |
	// +----------------+-+

	// ファイルヘッダ
	struct S_FNT_HEADER {
		IZ_UINT magic;
		IZ_UINT version;

		IZ_UINT sizeFile;			// ファイル長
		IZ_UINT sizeHeader;			// ヘッダ長

		IZ_UINT16 numFont;			// フォント数
		IZ_UINT8 numTex;			// テクスチャ数
		IZ_UINT8 fontHeight;		// フォント高さ

		IZ_UINT sizeMaxImage;		// 最大イメージサイズ

		E_FONT_CHAR_ENCODE charEncode;	// 文字コード

		E_GRAPH_PIXEL_FMT texFmt;		// テクスチャフォーマット

		struct {
			IZ_UINT isFixedPitch	: 1;	// 固定ピッチかどうか
			IZ_UINT reserved		: 31;
		};
	};

	// フォントイメージ情報
	struct S_FNT_IMAGE {
		IZ_UINT sizeData;		// データサイズ

		IZ_UINT16 numFont;		// 描きこまれた文字数
		IZ_UINT16 pitch;		// ピッチ

		IZ_UINT16 width;
		IZ_UINT16 height;

		const IZ_UINT8* images;	// データ
	};

	// フォントマップ情報
	struct S_FNT_MAP {
		IZ_UINT16 idx;			// 自分は何番目か
		IZ_UINT16 padding;

		IZ_UINT code;			// 文字コード

		// イメージ位置
		IZ_UINT16 srcX;
		IZ_UINT16 srcY;

		IZ_UINT16 fontWidth;	// フォント幅
		IZ_UINT8 texID;			// 所属テクスチャ
		IZ_UINT8 reserved;
	};
}	// namespace izanagi

// FNTフォーマットかどうか
#define IS_FMT_ENCODE(f)			((f) == izanagi::FNT_MAGIC_NUMBER)

// 最新バージョンかどうか
#define IS_CURRENT_FNT_VERSION(v)	((v) == izanagi::E_FNT_VERSION_CURRENT)

#endif	// #if !defined(__IZANAGI_FONT_FNT_ENCODE_H__)
