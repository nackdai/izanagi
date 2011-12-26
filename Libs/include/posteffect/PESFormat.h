#if !defined(__IZANAGI_POSTEFFECT_PES_FORMAT_H__)
#define __IZANAGI_POSTEFFECT_PES_FORMAT_H__

#include "izStd.h"
#include "izGraph.h"
#include "PostEffectDefs.h"

// PES = PostEffect Shader

namespace izanagi {
	// PESファイルバージョン
	enum E_PES_VERSION {
		E_PES_VERSION_0010 = 0x30303130,				// バージョン0010

		E_PES_VERSION_CURRENT = E_PES_VERSION_0010,		// 現在のバージョン
		E_PES_VERSION_FORCE_INT32 = 0x7FFFFFFF,			// enum のサイズを IZ_INT と同じにするため.
	};

	enum {
		// マジックナンバー
		PES_MAGIC_NUMBER = 0x004b5053,	// " PES"
	};

	enum {
		PES_CONNECT_PASS_NUM = 4,		// 関連付けられたパスの最大数
		PES_FUNCTOR_ARGS_NUM = 4,		// ファンクタ引数の数

		PES_ID_LEN = 32,
	};

	// NOTE
	// フォーマット
	// +----------------+
	// | ファイルヘッダ |
	// +----------------+
	// |   パラメータ   |
	// |    テーブル    |
	// +----------------+
	// | アノテーション |
	// |  (パラメータ)  |
	// |    テーブル    |
	// +----------------+
	// |パラメータ初期値|
	// |    バッファ    |
	// +----------------+
	// |   テクスチャ   |
	// |    テーブル    |
	// +----------------+
	// |サンプラテーブル|
	// +----------------+
	// |   テクニック   |
	// |    テーブル    |
	// +----------------+
	// |  パステーブル  |
	// +----------------+
	// |パスで利用される|
	// |  パラメータと  |
	// |   サンプラの   |
	// |  インデックス  |
	// +----------------+
	// | 文字列バッファ |
	// +----------------+
	// |   プログラム   |
	// +----------------+-+
	// | 頂点プログラム | |
	// |     ヘッダ     | |
	// +----------------+ +-- (1)
	// | 頂点プログラム | |
	// +----------------+-+
	// |     (1)を      |
	// | numVtxProgram  |
	// |    繰り返す    |
	// +----------------+

	// PESファイルヘッダ
	struct S_PES_HEADER {
		// リソースヘッダ
		IZ_UINT magic;
		E_PES_VERSION version;

		IZ_UINT sizeFile;		// ファイルサイズ
		IZ_UINT sizeHeadaer;	// ヘッダサイズ

		// ID
		IZ_INT8 pesID[PES_ID_LEN];
		IZ_UINT hashKey;

		// 記述数
		IZ_UINT16 numParam;				// パラメータ
		IZ_UINT16 numTex;				// テクスチャ
		IZ_UINT16 numSampler;			// サンプラ
		IZ_UINT16 numPass;				// パス
		IZ_UINT16 numTech;				// テクニック

		// アノテーション数
		IZ_UINT16 numParamAnn;			// パラメータ

		// 頂点プログラム
		IZ_UINT numVtxProgram;			// 頂点プログラム数

		// 文字列バッファサイズ
		IZ_UINT sizeStringBuffer;

		// パス用割り当てバッファサイズ
		IZ_UINT sizePassBuffForParamIdx;

		// パラメータ初期値バッファサイズ
		IZ_UINT sizeValueBuffer;

		// プログラムの最大サイズ
		IZ_UINT maxProgamSize;

		// プログラム位置
		IZ_UINT posProgram;
	};

	///////////////////////////////////////////////////////
	// アノテーション

	// パラメータアノテーション
	struct S_PES_PARAM_ANN {
		IZ_UINT16 UIName;		// 名前位置
		IZ_UINT16 UIHelp;		// ヒント位置

		IZ_UINT16 UIWidget;		// UIコントロール名位置

		IZ_UINT16 numParam;			// パラメータ数

		IZ_FLOAT paramMax[4];		// max
		IZ_FLOAT paramMin[4];		// min
		IZ_FLOAT paramStep[4];		// step

		// 有効なパラメータフラグ
		struct {
			IZ_UINT UIName		: 1;
			IZ_UINT UIHelp		: 1;
			IZ_UINT UIWidget	: 1;
			IZ_UINT UIMax		: 1;
			IZ_UINT UIMin		: 1;
			IZ_UINT UIStep		: 1;
		} flagValid;

		IZ_BOOL ExistName() const { return flagValid.UIName; }
		IZ_BOOL ExistHelp() const { return flagValid.UIHelp; }
		IZ_BOOL ExistWidget() const { return flagValid.UIWidget; }
		IZ_BOOL ExistMax() const { return flagValid.UIMin; }
		IZ_BOOL ExistMin() const { return flagValid.UIMin; }
		IZ_BOOL ExistStep() const { return flagValid.UIStep; }
	};

	// テクスチャアノテーション
	struct S_PES_TEXTURE_ANN {
		E_GRAPH_PIXEL_FMT fmt;		// ピクセルフォーマット
		IZ_UINT16 Dimensions[2];	// テクスチャサイズ
		IZ_FLOAT ViewportRatio[2];	// テクスチャ比率

		E_GRAPH_RSC_TYPE typeRsc;

		struct {
			IZ_UINT32 isRenderTarget	: 1;	// レンダーターゲットかどうか
			IZ_UINT32 isDynamic			: 1;	// 動的かどうか
		};
	};

	// パスアノテーション
	struct S_PES_PASS_ANN {
		// ファンクタ名
		IZ_UINT posNameFunctor;
		IZ_UINT keyFunctor;

		// ファンクタ引数
		IZ_FLOAT FunctorArgs[PES_FUNCTOR_ARGS_NUM];

		// クリアカラー
		IZ_COLOR ClearColor;
		
		struct {
			IZ_UINT isClearColor	: 1;	// レンダーターゲットをカラークリアするかどうか
			IZ_UINT isSubPass		: 1;
		};

		// シザリング設定のinflate値
		IZ_INT16 ScissorRectInflate[2];

		// ソーステクスチャのinflate値
		IZ_INT16 SrcTexRectInflate[2];

		// テクスチャオフセットパラメータインデックス
		IZ_INT TexOffsetParamIdx;

		// レンダーターゲットテクスチャインデックス
		IZ_INT RenderTargetIdx;
	};

	///////////////////////////////////////////////////////
	// ステート

	// サンプラステート
	struct S_PES_SAMPLER_STATE {
		IZ_UINT BindTexIdx;			// バインドするテクスチャのインデックス
		E_GRAPH_TEX_FILTER minFilter;
		E_GRAPH_TEX_FILTER magFilter;
		E_GRAPH_TEX_ADDRESS addrU;
		E_GRAPH_TEX_ADDRESS addrV;
	};

	// パスステート
	struct S_PES_PASS_STATE {
		IZ_BOOL AlphaTestEnable;		// アルファテストの可否
		E_GRAPH_CMP_FUNC AlphaTestFunc;	// アルファテスト式
		IZ_UINT8 AlphaTestRef;			// アルファテスト閾値
		IZ_UINT8 reserved[3];

		IZ_BOOL AlphaBlendEnable;	// アルファブレンドの可否
		IZ_UINT AlphaBlendMethod;	// アルファブレンド式
	};

	///////////////////////////////////////////////////////
	// Description

	// パラメータ
	struct S_PES_PARAMETER {
		IZ_UINT16 posName;
		IZ_UINT16 posSemantic;

		IZ_UINT keyName;		// 名前ハッシュ値
		IZ_UINT keySemantic;	// セマンティックハッシュ値

		E_POSTEFFECT_PARAMETER_CLASS Class;	// パラメータのクラス
		E_POSTEFFECT_PARAMETER_TYPE Type;	// パラメータのタイプ

		IZ_UINT8 Rows;				// 配列内の行の数
		IZ_UINT8 Columns;			// 配列内の列の数
		IZ_UINT8 Elements;			// 配列内の要素の数
		IZ_UINT8 StructMembers;		// 構造体のメンバの数（未使用）

		// 本来はアノテーションだが、こっちにないと困るので・・・
		struct {
			IZ_UINT32 Public		: 1;	// ユーザに公開するパラメータか
			IZ_UINT32 DoNotStrip	: 1;	// エフェクト内では使用しないけど削除しない
			IZ_UINT32 hasAnn		: 1;	// アノテーションを持つか
			IZ_UINT32 isDirty		: 1;
		};

		IZ_UINT16 AnnotationIdx;	// アノテーションインデックス
		IZ_UINT16 reserved;

		IZ_UINT Bytes;				// パラメータのサイズ(バイト単位)
		IZ_UINT Pos;				// パラメータバッファ位置(バイト単位)
	};

	// テクスチャ
	struct S_PES_TEXTURE {
		IZ_UINT16 posName;
		IZ_UINT16 posSemantic;

		IZ_UINT keyName;		// 名前ハッシュ値
		IZ_UINT keySemantic;	// セマンティックハッシュ値

		E_POSTEFFECT_TEXTURE_TYPE type;	// テクスチャタイプ

		S_PES_TEXTURE_ANN ann;
	};

	// サンプラ
	struct S_PES_SAMPLER {
		IZ_UINT16 posName;
		IZ_UINT16 posSemantic;

		IZ_UINT keyName;		// 名前ハッシュ値
		IZ_UINT keySemantic;	// セマンティックハッシュ値

		S_PES_SAMPLER_STATE state;
	};

	// パス
	struct S_PES_PASS {
		IZ_UINT posName;
		IZ_UINT keyName;		// 名前ハッシュ値

		IZ_UINT sizeProgram;	// プログラムサイズ

		// 頂点シェーダタイプ
		E_POSTEFFECT_VTX_SHADER VSType;

		IZ_UINT TechniqueIdx;	// 所属テクニックインデックス

		IZ_UINT16 numConst;
		IZ_UINT16 numSampler;

		S_PES_PASS_STATE state;
		S_PES_PASS_ANN ann;

	private:
		friend class CPostEffectPassTable;
		friend class CPostEffectShader;
		friend class CPostEffect;

		IZ_UINT* ptrConst;
		IZ_UINT* ptrSampler;

		IZ_UINT GetConstIdx(IZ_UINT idx) const { return ptrConst[idx]; }
		IZ_UINT GetSmplIdx(IZ_UINT idx) const { return ptrSampler[idx]; }
	};

	// テクニック
	struct S_PES_TECHNIQUE {
		IZ_UINT posName;
		IZ_UINT keyName;		// 名前ハッシュ値

		IZ_UINT16 posPass;		// パス位置
		IZ_UINT16 numPass;		// パス数
	};

	///////////////////////////////////////////////////////
	// その他

	// 頂点プログラムヘッダ
	struct S_PES_VS_HEADER {
		E_POSTEFFECT_VTX_SHADER type;		// 頂点シェーダタイプ
		IZ_UINT sizeProgram;				// プログラムサイズ
	};
}	// namespace izanagi

// PESフォーマットかどうか
#define IS_PES_FORMAT(f)			((f) == izanagi::PES_MAGIC_NUMBER)

// 最新バージョンかどうか
#define IS_CURRENT_PES_VERSION(v)	((v) == izanagi::E_PES_VERSION_CURRENT)

#endif	// #if !defined(__IZANAGI_POSTEFFECT_PES_FORMAT_H__)
