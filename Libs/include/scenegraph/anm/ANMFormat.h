#if !defined(__IZANAGI_SCENEGRAPH_ANM_FORMAT_H__)
#define __IZANAGI_SCENEGRAPH_ANM_FORMAT_H__

#include "izDefs.h"
#include "izStd.h"
#include "izMath.h"

// ANM = ANiMation

namespace izanagi {
    /** アニメーションの補間計算.
     */
    enum E_ANM_INTERP_TYPE {
        E_ANM_INTERP_TYPE_LINEAR = 0,
        E_ANM_INTERP_TYPE_BEZIER,
        E_ANM_INTERP_TYPE_HERMITE,
        E_ANM_INTERP_TYPE_SLERP,

        E_ANM_INTERP_TYPE_NUM,

        E_ANM_INTERP_TYPE_MASK = 0x7f,
        E_ANM_INTERP_TYPE_USER_CUSTOM = 1 << 7,
        
        E_ANM_INTERP_TYPE_FORCE_INT32 = 0x7fffffff,
    };
    IZ_C_ASSERT(E_ANM_INTERP_TYPE_NUM < IZ_UINT8_MAX);

    /** アニメーションを行うパラメータ.
     */
    enum E_ANM_TRANSFORM_TYPE {
        E_ANM_TRANSFORM_TYPE_X  = 1 << 0,
        E_ANM_TRANSFORM_TYPE_Y  = 1 << 1,
        E_ANM_TRANSFORM_TYPE_Z  = 1 << 2,
        E_ANM_TRANSFORM_TYPE_W  = 1 << 3,
        E_ANM_TRANSFORM_TYPE_XYZ  = E_ANM_TRANSFORM_TYPE_X | E_ANM_TRANSFORM_TYPE_Y | E_ANM_TRANSFORM_TYPE_Z,
        E_ANM_TRANSFORM_TYPE_XYZW = E_ANM_TRANSFORM_TYPE_X | E_ANM_TRANSFORM_TYPE_Y | E_ANM_TRANSFORM_TYPE_Z | E_ANM_TRANSFORM_TYPE_W,

        E_ANM_TRANSFORM_TYPE_PARAM_SHIFT = 0,
        E_ANM_TRANSFORM_TYPE_PARAM_MASK = 0x0f,

        E_ANM_TRANSFORM_TYPE_TRANSLATE  = 1 << 4,
        E_ANM_TRANSFORM_TYPE_ROTATE     = 1 << 5,
        E_ANM_TRANSFORM_TYPE_QUATERNION = 1 << 6,
        E_ANM_TRANSFORM_TYPE_SCALE      = 1 << 7,

        E_ANM_TRANSFORM_TYPE_TRANSFORM_SHIFT = 4,
        E_ANM_TRANSFORM_TYPE_TRANSFORM_MASK = 0x7ffffff0,

        E_ANM_TRANSFORM_TYPE_TRANSLATE_XYZ = E_ANM_TRANSFORM_TYPE_TRANSLATE | E_ANM_TRANSFORM_TYPE_XYZ,
        E_ANM_TRANSFORM_TYPE_TRANSLATE_X   = E_ANM_TRANSFORM_TYPE_TRANSLATE | E_ANM_TRANSFORM_TYPE_X,
        E_ANM_TRANSFORM_TYPE_TRANSLATE_Y   = E_ANM_TRANSFORM_TYPE_TRANSLATE | E_ANM_TRANSFORM_TYPE_Y,
        E_ANM_TRANSFORM_TYPE_TRANSLATE_Z   = E_ANM_TRANSFORM_TYPE_TRANSLATE | E_ANM_TRANSFORM_TYPE_Z,

        E_ANM_TRANSFORM_TYPE_ROTATE_XYZ = E_ANM_TRANSFORM_TYPE_ROTATE | E_ANM_TRANSFORM_TYPE_XYZ,
        E_ANM_TRANSFORM_TYPE_ROTATE_X   = E_ANM_TRANSFORM_TYPE_ROTATE | E_ANM_TRANSFORM_TYPE_X,
        E_ANM_TRANSFORM_TYPE_ROTATE_Y   = E_ANM_TRANSFORM_TYPE_ROTATE | E_ANM_TRANSFORM_TYPE_Y,
        E_ANM_TRANSFORM_TYPE_ROTATE_Z   = E_ANM_TRANSFORM_TYPE_ROTATE | E_ANM_TRANSFORM_TYPE_Z,
        
        E_ANM_TRANSFORM_TYPE_QUATERNION_XYZW = E_ANM_TRANSFORM_TYPE_QUATERNION | E_ANM_TRANSFORM_TYPE_XYZW,
        E_ANM_TRANSFORM_TYPE_QUATERNION_XYZ  = E_ANM_TRANSFORM_TYPE_QUATERNION | E_ANM_TRANSFORM_TYPE_XYZ,
        E_ANM_TRANSFORM_TYPE_QUATERNION_W    = E_ANM_TRANSFORM_TYPE_QUATERNION | E_ANM_TRANSFORM_TYPE_W,
        
        E_ANM_TRANSFORM_TYPE_SCALE_XYZ = E_ANM_TRANSFORM_TYPE_SCALE | E_ANM_TRANSFORM_TYPE_XYZ,
        E_ANM_TRANSFORM_TYPE_SCALE_X   = E_ANM_TRANSFORM_TYPE_SCALE | E_ANM_TRANSFORM_TYPE_X,
        E_ANM_TRANSFORM_TYPE_SCALE_Y   = E_ANM_TRANSFORM_TYPE_SCALE | E_ANM_TRANSFORM_TYPE_Y,
        E_ANM_TRANSFORM_TYPE_SCALE_Z   = E_ANM_TRANSFORM_TYPE_SCALE | E_ANM_TRANSFORM_TYPE_Z,
    };

    /** アニメーションキーフレーム.
     */
    enum E_ANM_KEY_TYPE {
        E_ANM_KEY_TYPE_TIME = 0,    ///< 時間ベース.
        E_ANM_KEY_TYPE_FRAME,       ///< フレームベース.

        E_ANM_KEY_TYPE_FORCE_INT32 = 0x7fffffff,
    };

    enum {
        // TODO
        ANM_NAME_LEN = 31,
    };

    // NOTE
    // フォーマット
    // +----------------+
    // | ファイルヘッダ |
    // +----------------+
    // | アニメーション |
    // |     ノード     |
    // +----------------+
    // | アニメーション |
    // |   チャンネル   |
    // +----------------+
    // | アニメーション |
    // |      キー      |
    // +----------------+

    /**
    */
    struct S_ANM_HEADER {
        IZ_UINT magic;
        IZ_UINT version;

        IZ_UINT sizeHeader;
        IZ_UINT sizeFile;

        IZ_UINT numNodes;
        IZ_UINT numChannels;
        IZ_UINT numKeys;

        E_ANM_KEY_TYPE keyType; ///< アニメーションキーフレーム.
        IZ_UINT reserved;

        IZ_FLOAT time;
    };

    /** キーフレーム情報.
     *
     * キーフレームあたりのジョイントのパラメータに適用するパラメータ.
     */
    struct S_ANM_KEY {
        IZ_FLOAT keyTime;       ///< キー時間.
        
        IZ_UINT8 numParams;     ///< アニメーションパラメータ数。位置、回転、スケールによって異なる.
        IZ_UINT8 stride;        ///< １パラメータあたりのバイトサイズ.
        IZ_UINT8 reserved[2];

        // TODO
        // Work around...
        union {
            IZ_UINT64 value;
            IZ_FLOAT* params;
        };
    };

    /** アニメーションチャンネル
     *
     * ジョイントのパラメータ（ex. 位置、回転など）ごとのアニメーション情報
     */
    struct S_ANM_CHANNEL {
        IZ_UINT8 interp;    ///< 補間計算のタイプ.
        IZ_UINT8 stride;
        IZ_UINT16 numKeys;  ///< キーフレーム情報数.

        IZ_UINT keyIdx;

        IZ_UINT type;       ///< アニメーションを行うパラメータのタイプ.
    };

    /** アニメーションノード.
     *
     * 適用ジョイントの情報
     */
    struct S_ANM_NODE {
        CStdString<izChar, ANM_NAME_LEN> target;    ///< 適用対象のジョイント名.
        IZ_UINT targetKey;                          ///< 適用対象のジョイントのキー値.

        IZ_UINT16 targetIdx;    ///< 適用対象のジョイントのインデックス.
        IZ_UINT16 numChannels;  ///< チャンネル数.

        IZ_UINT channelIdx;
    };

    /** スカラー値の補間処理.
     */
    typedef IZ_FLOAT (*FuncInterpScalar)(IZ_FLOAT time, IZ_UINT keyNum, IZ_UINT paramPos, const S_ANM_KEY** keys);

    /** ベクターの補間処理.
     */
    typedef void (*FuncInterpVector)(math::SVector& ret, IZ_FLOAT time, IZ_UINT keyNum, IZ_UINT paramPos, const S_ANM_KEY** keys);

}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_ANM_FORMAT_H__)
