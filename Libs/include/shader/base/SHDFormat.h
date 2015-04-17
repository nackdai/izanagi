#if !defined(__IZANAGI_SHADER_SHD_FORMAT_H__)
#define __IZANAGI_SHADER_SHD_FORMAT_H__

#include "izGraph.h"
#include "ShaderDefs.h"

// SHD = SHaDer

namespace izanagi
{
namespace shader
{
    // File version of SHD file.
    enum E_SHD_VERSION {
        E_SHD_VERSION_0010 = 0x30303130,                // "0010"

        E_SHD_VERSION_CURRENT = E_SHD_VERSION_0010,     // current version
        E_SHD_VERSION_FORCE_INT32 = 0x7FFFFFFF,
    };

    enum {
        // TODO
        // Magic number of SHD file.
        SHD_MAGIC_NUMBER = 0x004b5053,  // " SHD"

        SHD_CHUNK_MAGIC_NUMBER_STRING = 0x00000001, // " STR"
        SHD_CHUNK_MAGIC_NUMBER_PARAM  = 0x00000002, // " PRM"
        SHD_CHUNK_MAGIC_NUMBER_TEX    = 0x00000003, // " TEX"
        SHD_CHUNK_MAGIC_NUMBER_SMPL   = 0x00000004, // "SMPL"
        SHD_CHUNK_MAGIC_NUMBER_PASS   = 0x00000005, // "PASS"
        SHD_CHUNK_MAGIC_NUMBER_TECH   = 0x00000006, // "TECH"
        SHD_CHUNK_MAGIC_NUMBER_PROG   = 0x00000007, // "PROG"
        SHD_CHUNK_MAGIC_NUMBER_ATTR   = 0x00000008, // "ATTR"

        SHD_PROGRAM_CHUNK_TERMINATER   = 0x7fffffff,
    };

    enum {
        SHD_ATTR_HASH_MAX = 5,
        SHD_NAME_LEN_MAX = 127,
    };

    //////////////////////////////////////////////////////////

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
    // |    属性値      |
    // +----------------+

    //////////////////////////////////////////////////////////

    // ヘッダ
    struct S_SHD_HEADER {
        IZ_UINT magic;
        E_SHD_VERSION version;

        IZ_UINT sizeFile;   // ファイル長
        IZ_UINT sizeHeader; // ヘッダ長

        // Shader name.
        CStdString<izChar, SHD_NAME_LEN_MAX> name;
        IZ_UINT nameKey;

        // 文字列バッファサイズ
        IZ_UINT sizeStringBuffer;

        IZ_UINT16 numPass;
        IZ_UINT16 numParam;
        IZ_UINT16 numSmpl;
        IZ_UINT16 numAttr;
        IZ_UINT16 numTexture;
        IZ_UINT16 padding;

        // プログラムの最大サイズ
        IZ_UINT maxProgamSize;

        // プルグラムの位置
        IZ_UINT posProgram;
    };

    struct S_SHD_CHUNK_HEADER {
        IZ_UINT magicChunk;
    };

    struct S_SHD_PARAM_HEADER {
        IZ_UINT16 numParameter;     // パラメータ数
        IZ_UINT16 numParamAnn;      // パラメータアノテーション数
        IZ_UINT sizeValueBuffer;    // パラメータ初期値バッファサイズ
    };

    struct S_SHD_TEXTRUE_HEADER {
        IZ_UINT16 numTexture;       // テクスチャ数
        IZ_UINT16 padding;
    };

    struct S_SHD_SAMPLER_HEADER {
        IZ_UINT16 numSampler;       // サンプラ数
        IZ_UINT16 padding;
    };

    struct S_SHD_PASS_HEADER {
        IZ_UINT16 numPass;          // パス数（総数）
        IZ_UINT16 padding;
    };

    struct S_SHD_TECH_HEADER {
        IZ_UINT16 numTechnique;     // テクニック数
        IZ_UINT16 padding;
    };

    struct S_SHD_ATTR_HEADER {
        IZ_UINT16 numAttrs;
        IZ_UINT16 padding;
    };

    //////////////////////////////////////////////////////////

    // パラメータアノテーション
    struct S_SHD_PARAM_ANN {
        IZ_UINT16 UIName;       // 名前位置
        IZ_UINT16 UIHelp;       // ヒント位置

        IZ_UINT16 UIWidget;     // UIコントロール名位置

        IZ_UINT16 numParam;         // パラメータ数

        IZ_FLOAT paramMax[4];       // max
        IZ_FLOAT paramMin[4];       // min
        IZ_FLOAT paramStep[4];      // step

        // 有効なパラメータフラグ
        struct {
            IZ_UINT hasUIName       : 1;
            IZ_UINT hasUIHelp       : 1;
            IZ_UINT hasUIWidget     : 1;
            IZ_UINT hasUIMax        : 1;
            IZ_UINT hasUIMin        : 1;
            IZ_UINT hasUIStep       : 1;
        };

        IZ_BOOL ExistName() const   { return hasUIName; }
        IZ_BOOL ExistHelp() const   { return hasUIHelp; }
        IZ_BOOL ExistWidget() const { return hasUIWidget; }
        IZ_BOOL ExistMax() const    { return hasUIMin; }
        IZ_BOOL ExistMin() const    { return hasUIMin; }
        IZ_BOOL ExistStep() const   { return hasUIStep; }
    };

    // パラメータ
    struct S_SHD_PARAMETER {
        IZ_UINT16 posName;
        IZ_UINT16 posSemantic;

        IZ_UINT keyName;        // 名前ハッシュ値
        IZ_UINT keySemantic;    // セマンティックハッシュ値

        E_SHADER_PARAMETER_CLASS Class; // パラメータのクラス
        E_SHADER_PARAMETER_TYPE Type;   // パラメータのタイプ

        E_SHADER_PARAMETER_TYPE originalType;

        IZ_UINT8 Rows;              // 配列内の行の数
        IZ_UINT8 Columns;           // 配列内の列の数
        IZ_UINT8 Elements;          // 配列内の要素の数
        IZ_UINT8 StructMembers;     // 構造体のメンバの数（未使用）

        // 本来はアノテーションだが、こっちにないと困るので・・・
        struct {
            IZ_UINT32 Public            : 1;    // ユーザに公開するパラメータか
            IZ_UINT32 DoNotStrip        : 1;    // エフェクト内では使用しないけど削除しない
            IZ_UINT32 hasAnn            : 1;    // アノテーションを持つか
            IZ_UINT32 hasDefaultValue   : 1;    // 初期値を持つか
            IZ_UINT32 isDirty           : 1;
        };

        IZ_UINT16 AnnotationIdx;    // アノテーションインデックス
        IZ_UINT16 reserved;

        IZ_UINT Bytes;              // パラメータのサイズ(バイト単位)
        IZ_UINT Pos;                // パラメータバッファ位置(バイト単位)
    };

    //////////////////////////////////////////////////////////

#if 0
    // テクスチャアノテーション
    struct S_SHD_TEXTURE_ANN {
        graph::E_GRAPH_PIXEL_FMT fmt;       // ピクセルフォーマット
        IZ_UINT16 Dimensions[2];    // テクスチャサイズ
        IZ_FLOAT ViewportRatio[2];  // テクスチャ比率

        graph::E_GRAPH_RSC_USAGE typeRsc;

        struct {
            IZ_UINT32 isRenderTarget    : 1;    // レンダーターゲットかどうか
            IZ_UINT32 isDynamic         : 1;    // 動的かどうか
        };
    };
#endif

    // テクスチャ
    struct S_SHD_TEXTURE {
        IZ_UINT16 posName;
        IZ_UINT16 posSemantic;

        IZ_UINT keyName;        // 名前ハッシュ値
        IZ_UINT keySemantic;    // セマンティックハッシュ値

        //S_SHD_TEXTURE_ANN ann;        
    };

    //////////////////////////////////////////////////////////

    // サンプラ
    struct S_SHD_SAMPLER {
        IZ_UINT16 posName;
        IZ_UINT16 posSemantic;

        IZ_UINT keyName;        // 名前ハッシュ値
        IZ_UINT keySemantic;    // セマンティックハッシュ値

        // バインドするテクスチャのインデックス
        // If BindTexIdx is -1, specified texture is nothing.
        IZ_INT BindTexIdx;

        IZ_INT resource_id;

        E_SHADER_PARAMETER_TYPE Type;

        graph::S_SAMPLER_STATE state;
    };

    //////////////////////////////////////////////////////////

    // パスステート
    struct S_SHD_PASS_STATE {
        IZ_UINT AlphaBlendMethod;   // アルファブレンド式
        graph::E_GRAPH_CMP_FUNC AlphaTestFunc;  // アルファテスト式

        IZ_UINT8 AlphaTestRef;          // アルファテスト閾値
        IZ_UINT8 reserved[2];
        struct {
            IZ_UINT8 AlphaBlendEnable   : 1;
            IZ_UINT8 AlphaTestEnable    : 1;
            IZ_UINT8 ZWriteEnable       : 1;
            IZ_UINT8 ZEnable            : 1;
        };

        graph::E_GRAPH_CMP_FUNC ZFunc;      // Zテスト式
    };

    struct S_SHD_PARAM_IDX {
        IZ_UINT isVS    : 1;
        IZ_UINT isPS    : 1;
        IZ_UINT idx     : 30;
    };

    // パス
    struct S_SHD_PASS {
        IZ_UINT posName;
        IZ_UINT keyName;        // 名前ハッシュ値

        // プログラムサイズ
        IZ_UINT16 sizeVS;
        IZ_UINT16 sizePS;

        IZ_UINT TechniqueIdx;   // 所属テクニックインデックス

        IZ_UINT16 numConst;
        IZ_UINT16 numSampler;

        S_SHD_PASS_STATE state;
    };

    //////////////////////////////////////////////////////////

    // テクニック
    struct S_SHD_TECHNIQUE {
        IZ_UINT posName;
        IZ_UINT keyName;        // 名前ハッシュ値

        IZ_UINT16 posPass;      // パス位置
        IZ_UINT16 numPass;      // パス数
    };

    //////////////////////////////////////////////////////////

    struct S_SHD_ATTRIBUTE {
        IZ_UINT posName;
        IZ_UINT keyName;

        E_SHADER_PARAMETER_TYPE type;   // float or int or bool
        IZ_UINT8 param[4];
    };
}   // namespace shader
}   // namespace izanagi

// Returns whether format is SHD file's format.
#define IS_SHD_FORMAT(f)            ((f) == izanagi::SHD_MAGIC_NUMBER)

// Returns whether version is current.
#define IS_CURRENT_SHD_VERSION(v)   ((v) == izanagi::E_SHD_VERSION_CURRENT)

#endif  // #if !defined(__IZANAGI_SHADER_SHD_FORMAT_H__)
