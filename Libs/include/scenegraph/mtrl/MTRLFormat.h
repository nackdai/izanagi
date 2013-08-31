#if !defined(__IZANAGI_SCENEGRAPH_MTRL_FORMAT_H__)
#define __IZANAGI_SCENEGRAPH_MTRL_FORMAT_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izShader.h"

// MTRL = MaTeRiaL

namespace izanagi {
    /** mtrlファイルバージョン
     */
    enum E_MTRL_VERSION {
        E_MTRL_VERSION_0010 = 0x30303130,               ///< バージョン0010

        E_MTRL_VERSION_CURRENT = E_MTRL_VERSION_0010,   ///< 現在のバージョン
        E_MTRL_VERSION_FORCE_INT32 = 0x7FFFFFFF,
    };

    enum {
        MTRL_MATERIAL_NAME_LEN = 63,
        MTRL_TEXTURE_NAME_LEN  = 63,
        MTRL_SHADER_NAME_LEN   = 63,
        MTRL_PARAM_NAME_LEN    = 63,
    };

    /** マテリアルパラメータタイプ.
     */
    enum E_MTRL_PARAM_TYPE {
        E_MTRL_PARAM_TYPE_FLOAT = 0,    ///< float
        E_MTRL_PARAM_TYPE_VECTOR,       ///< ベクトル.
        E_MTRL_PARAM_TYPE_MATRIX,       ///< マトリクス.
        E_MTRL_PARAM_TYPE_UINT,         ///< unsigned int.
        E_MTRL_PARAM_TYPE_BOOL,         ///< bool.

        E_MTRL_PARAM_TYPE_NUM,
        E_MTRL_PARAM_TYPE_FORCE_INT32 = 0x7fffffff,
    };

    // NOTE
    // +--------------------+
    // |       ヘッダ       |
    // +--------------------+
    // |  ジャンプテーブル  |
    // +--------------------+
    // | +----------------+ |
    // | | パラメータ情報 | |
    // | +----------------+ |
    // | | データバッファ | |
    // | +----------------+ |
    // |       ・・・       |
    // +--------------------+

    /** mtrlファイルヘッダ.
     */
    struct S_MTRL_HEADER {
        IZ_UINT magic;
        E_MTRL_VERSION version;

        IZ_UINT sizeFile;
        IZ_UINT sizeHeader;
    };

    /** マテリアル情報.
     */
    struct S_MTRL_MATERIAL {
        CStdString<izChar, MTRL_MATERIAL_NAME_LEN> name;
        IZ_UINT keyMaterial;

        IZ_UINT16 numTex;       ///< テクスチャ数.
        IZ_UINT16 numShader;    ///< シェーダ数.

        IZ_UINT16 numParam;     ///< パラメータ数.
        IZ_UINT16 paramBytes;   ///< パラメータの総バイト数.
    };

    /** マテリアルテクスチャタイプ.
     */
    union S_MTRL_TEXTURE_TYPE {
        struct {
            IZ_UINT isNormal        : 1;    ///< 法線マップ
            IZ_UINT isSpecular      : 1;    ///< スペキュラマップ
            IZ_UINT isEnvironment   : 1;    ///< 環境マップ
            IZ_UINT isVolume        : 1;    ///< ボリュームテクスチャ
            IZ_UINT isTranslucent   : 1;    ///< 半透明
        };
        IZ_UINT flags;
    };

    /** マテリアルテクスチャ情報.
     */
    struct S_MTRL_TEXTURE {
        CStdString<izChar, MTRL_TEXTURE_NAME_LEN> name; ///< テクスチャ名
        IZ_UINT key;

        S_MTRL_TEXTURE_TYPE type;   ///< テクスチャタイプ

    private:
        // テクスチャ
        graph::CBaseTexture* tex;

        friend class CMaterial;
    };

    /** マテリアルシェーダ情報.
     */
    struct S_MTRL_SHADER {
        CStdString<izChar, MTRL_SHADER_NAME_LEN> name;  ///< シェーダ名
        IZ_UINT key;

    private:
        // シェーダ
        shader::IShader* shader;

        // 使用するテクニックインデックス
        IZ_UINT tech_idx;

        friend class CMaterial;
    };

    /** マテリアルパラメータ情報.
     */
    struct S_MTRL_PARAM {
        CStdString<izChar, MTRL_PARAM_NAME_LEN> name;   ///< パラメータ名.
        IZ_UINT key;

        E_MTRL_PARAM_TYPE type; ///< パラメータタイプ.

        IZ_UINT16 elements;     ///< 要素数.
        IZ_UINT16 bytes;        ///< データサイズ.

    private:
        // パラメータ保持用バッファ
        void* param;

        // シェーダパラメータハンドル
        shader::IZ_SHADER_HANDLE handle;

        friend class CMaterial;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_MTRL_FORMAT_H__)
