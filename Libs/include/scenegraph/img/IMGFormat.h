#if !defined(__IZANAGI_SCENEGRAPH_IMG_FORMAT_H__)
#define __IZANAGI_SCENEGRAPH_IMG_FORMAT_H__

#include "izDefs.h"
#include "graph/GraphDefs.h"

// IMG = IMaGe

namespace izanagi {
	// IMGファイルバージョン
	enum E_IMG_VERSION {
		E_IMG_VERSION_0010 = 0x30303130,				// バージョン0010

		E_IMG_VERSION_CURRENT = E_IMG_VERSION_0010,		// 現在のバージョン
		E_IMG_VERSION_FORCE_INT32 = 0x7FFFFFFF,			// enum のサイズを IZ_INT と同じにするため.
	};

	enum {
		// TODO
		// "IMG "
		IMG_MAGIC_NUMBER = 0x494d4700,
	};

	// NOTE
	// フォーマット
	// +----------------+
	// | ファイルヘッダ |
	// +----------------+
	// |    ジャンプ    |
	// |    テーブル    |
	// +----------------+--+
	// |   テクスチャ   |  |
	// |     ヘッダ     |  |
	// +----------------+  + - ファイル数分
	// | イメージデータ |  |
	// +----------------+--+

	/**
	* ファイルヘッダ
	*/
	struct S_IMG_HEADER {
		IZ_UINT magic;
		IZ_UINT version;

		IZ_UINT sizeFile;			// ファイル長
		IZ_UINT sizeHeader;			// ヘッダ長

		// テクスチャ数
		IZ_UINT numTextures;

		// 最大サイズ
		IZ_UINT sizeMax;

		// ジャンプテーブルサイズ
		IZ_UINT sizeJumpTbl;
	};

	/**
	* テクスチャヘッダ
	*/
	struct S_IMG_TEX_HEADER {
		IZ_UINT size;

		E_GRAPH_PIXEL_FMT fmt;	// ピクセルフォーマット

		struct {
			IZ_UINT type		: 2;	// テクスチャタイプ
			IZ_UINT w			: 4;	// 幅（２のべき乗の指数）
			IZ_UINT h			: 4;	// 高さ（２のべき乗の指数）
			IZ_UINT level		: 4;	// MIPMAPレベル
			IZ_UINT anisoNum	: 4;	// 異方性フィルタサンプル数
			IZ_UINT depth		: 8;	// ボリュームテクスチャの深さ
		};

		struct {
			IZ_UINT addressU	: 4;
			IZ_UINT addressV	: 4;
			IZ_UINT minFilter	: 4;
			IZ_UINT magFilter	: 4;
			IZ_UINT mipFilter	: 4;
		};
	};
}	// namespace izanagi

// IMGフォーマットかどうか
#define IS_IMG_FORMAT(f)			((f) == izanagi::IMG_MAGIC_NUMBER)

// 最新バージョンかどうか
#define IS_CURRENT_IMG_VERSION(v)	((v) == izanagi::E_IMG_VERSION_CURRENT)

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_IMG_FORMAT_H__)
