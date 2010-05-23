#if !defined(__URANUS_FONT_FNT_ENCODE_H__)
#define __URANUS_FONT_FNT_ENCODE_H__

#include "unDefs.h"
#include "graph/GraphDefs.h"
#include "FontDefs.h"

// FNT = FoNT

namespace uranus {
	// FNTファイルバージョン
	enum E_FNT_VERSION {
		E_FNT_VERSION_0010 = 0x30303130,				// バージョン0010

		E_FNT_VERSION_CURRENT = E_FNT_VERSION_0010,		// 現在のバージョン
		E_FNT_VERSION_FORCE_INT32 = 0x7FFFFFFF,			// enum のサイズを UN_INT と同じにするため.
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
		UN_UINT magic;
		UN_UINT version;

		UN_UINT sizeFile;			// ファイル長
		UN_UINT sizeHeader;			// ヘッダ長

		UN_UINT16 numFont;			// フォント数
		UN_UINT8 numTex;			// テクスチャ数
		UN_UINT8 fontHeight;		// フォント高さ

		UN_UINT sizeMaxImage;		// 最大イメージサイズ

		E_FONT_CHAR_ENCODE charEncode;	// 文字コード

		E_GRAPH_PIXEL_FMT texFmt;		// テクスチャフォーマット

		struct {
			UN_UINT isFixedPitch	: 1;	// 固定ピッチかどうか
			UN_UINT reserved		: 31;
		};
	};

	// フォントイメージ情報
	struct S_FNT_IMAGE {
		UN_UINT sizeData;		// データサイズ

		UN_UINT16 numFont;		// 描きこまれた文字数
		UN_UINT16 pitch;		// ピッチ

		UN_UINT16 width;
		UN_UINT16 height;

		const UN_UINT8* images;	// データ
	};

	// フォントマップ情報
	struct S_FNT_MAP {
		UN_UINT16 idx;			// 自分は何番目か
		UN_UINT16 padding;

		UN_UINT code;			// 文字コード

		// イメージ位置
		UN_UINT16 srcX;
		UN_UINT16 srcY;

		UN_UINT16 fontWidth;	// フォント幅
		UN_UINT8 texID;			// 所属テクスチャ
		UN_UINT8 reserved;
	};
}	// namespace uranus

// FNTフォーマットかどうか
#define IS_FMT_ENCODE(f)			((f) == uranus::FNT_MAGIC_NUMBER)

// 最新バージョンかどうか
#define IS_CURRENT_FNT_VERSION(v)	((v) == uranus::E_FNT_VERSION_CURRENT)

#endif	// #if !defined(__URANUS_FONT_FNT_ENCODE_H__)
