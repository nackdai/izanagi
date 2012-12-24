#if !defined(__IZANAGI_SCENEGRAPH_MSH_FORMAT_H__)
#define __IZANAGI_SCENEGRAPH_MSH_FORMAT_H__

#include "izDefs.h"
#include "izStd.h"
#include "izMath.h"
#include "izGraph.h"

// MSH = MoDeL

namespace izanagi {
    enum E_MSH_VTX_FMT_TYPE {
        E_MSH_VTX_FMT_TYPE_POS = 0,
        E_MSH_VTX_FMT_TYPE_NORMAL,
        E_MSH_VTX_FMT_TYPE_COLOR,
        E_MSH_VTX_FMT_TYPE_UV,
        E_MSH_VTX_FMT_TYPE_TANGENT,
        E_MSH_VTX_FMT_TYPE_BLENDINDICES,
        E_MSH_VTX_FMT_TYPE_BLENDWEIGHT,

        E_MSH_VTX_FMT_TYPE_NUM,
        E_MSH_VTX_FMT_TYPE_FORCE_INT32 = 0x7fffffff,
    };

    enum E_MSH_VTX_SIZE {
        E_MSH_VTX_SIZE_POS          = sizeof(IZ_FLOAT) * 4, ///< 頂点位置
        E_MSH_VTX_SIZE_NORMAL       = sizeof(IZ_FLOAT) * 3, ///< 法線
        E_MSH_VTX_SIZE_COLOR        = sizeof(IZ_COLOR),     ///< 頂点カラー
        E_MSH_VTX_SIZE_UV           = sizeof(IZ_FLOAT) * 2, ///< UV座標
        E_MSH_VTX_SIZE_TANGENT      = sizeof(IZ_FLOAT) * 3, ///< 接ベクトル
        E_MSH_VTX_SIZE_BLENDINDICES = sizeof(IZ_FLOAT) * 4, ///< ブレンドマトリクスインデックス
        E_MSH_VTX_SIZE_BLENDWEIGHT  = sizeof(IZ_FLOAT) * 4, ///< ブレンドウエイト
    };

    enum {
        // TODO
        MSH_NAME_LEN = 31,  ///< マテリアル名最大長

        MSH_BELONGED_JOINT_MIN = 4,
    };

    // フォーマット
    // +------------------------+
    // |         ヘッダ     　　|
    // +------------------------+
    // |    メッシュグループ    |
    // +------------------------+

    // メッシュグループ
    // +------------------------+
    // |     グループヘッダ     |
    // +------------------------+
    // |   頂点データテーブル   |
    // | +--------------------+ |
    // | |      ヘッダ        | |
    // | +--------------------+ |
    // | |     頂点データ     | |
    // | +--------------------+ |
    // |         ・・・         |
    // +------------------------+
    // |    メッシュテーブル    |
    // | +--------------------+ |
    // | |      メッシュ      | |
    // | |+------------------+| |
    // | ||     ヘッダ       || |
    // | |+------------------+| |
    // | |                    | |
    // | |     サブセット     | |
    // | |+------------------+| |
    // | ||     ヘッダ       || |
    // | |+------------------+| |
    // | ||インデックスデータ|| |
    // | |+------------------+| |
    // | |      ・・・        | |
    // | +--------------------+ |
    // |        ・・・          |
    // +------------------------+
    
    struct S_MSH_HEADER {
        IZ_UINT magic;
        IZ_UINT version;

        IZ_UINT sizeHeader;
        IZ_UINT sizeFile;

        IZ_FLOAT maxVtx[3];
        IZ_FLOAT minVtx[3];

        IZ_UINT16 numVB;
        IZ_UINT16 numMeshGroup;
        IZ_UINT16 numMeshSet;
        IZ_UINT16 numMeshSubset;

        IZ_UINT numAllJointIndices; ///< ジョイントインデックス総数
    };

    /////////////////////////////////////////////////////////

    // マテリアル情報
    struct S_MSH_MTRL {
        CStdString<izChar, MSH_NAME_LEN> name;  ///< マテリアル名
        IZ_UINT nameKey;    ///< マテリアル名キー
    };

    /////////////////////////////////////////////////////////

    // 頂点データ情報
    struct S_MSH_VERTICES {
        IZ_UINT16 sizeVtx;  ///< １頂点あたりのサイズ
        IZ_UINT16 numVtx;   ///< 頂点数
    };

    // メッシュグループ情報
    struct S_MSH_MESH_GROUP {
        IZ_UINT16 numVB;        ///< 頂点バッファ数
        IZ_UINT16 numMeshSet;   ///< メッシュセット数
    };

    // メッシュセット情報
    struct S_MSH_MESH_SET {
        IZ_UINT16 numSubset;

        IZ_UINT16 fmt;      ///< 頂点フォーマット

        IZ_FLOAT center[3]; ///< 重心位置

        S_MSH_MTRL mtrl;    ///< マテリアル情報
    };

    // プリミティブセット情報
    struct S_MSH_PRIM_SET {
        graph::E_GRAPH_PRIM_TYPE typePrim;

        IZ_UINT16 idxVB;        ///< 利用する頂点バッファのインデックス
        IZ_UINT16 minIdx;
        IZ_UINT16 maxIdx;
        IZ_UINT16 numJoints;    ///< ジョイント数

        IZ_UINT numIdx;                     ///< インデックス数
        graph::E_GRAPH_INDEX_BUFFER_FMT fmtIdx; ///< インデックスフォーマット

        IZ_INT16* joints;   ///< ジョイントインデックスバッファ
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_MSH_FORMAT_H__)
