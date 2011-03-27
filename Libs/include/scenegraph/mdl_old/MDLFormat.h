#if !defined(__IZANAGI_SCENEGRAPH_MDL_FORMAT_H__)
#define __IZANAGI_SCENEGRAPH_MDL_FORMAT_H__

#include "izDefs.h"
#include "izStd.h"
#include "izMath.h"
#include "izGraph.h"

// MDL = MoDeL

namespace izanagi {
	enum E_MDL_VTX_FMT_TYPE {
		E_MDL_VTX_FMT_TYPE_POS = 0,
		E_MDL_VTX_FMT_TYPE_NORMAL,
		E_MDL_VTX_FMT_TYPE_COLOR,
		E_MDL_VTX_FMT_TYPE_UV,
		E_MDL_VTX_FMT_TYPE_TANGENT,
		E_MDL_VTX_FMT_TYPE_BLENDINDICES,
		E_MDL_VTX_FMT_TYPE_BLENDWEIGHT,

		E_MDL_VTX_FMT_TYPE_NUM,
		E_MDL_VTX_FMT_TYPE_FORCE_INT32 = 0x7fffffff,
	};

	enum E_MDL_VTX_SIZE {
		E_MDL_VTX_SIZE_POS          = sizeof(IZ_FLOAT) * 4,	// 頂点位置
		E_MDL_VTX_SIZE_NORMAL       = sizeof(IZ_FLOAT) * 3,	// 法線
		E_MDL_VTX_SIZE_COLOR        = sizeof(IZ_COLOR),		// 頂点カラー
		E_MDL_VTX_SIZE_UV           = sizeof(IZ_FLOAT) * 2,	// UV座標
		E_MDL_VTX_SIZE_TANGENT      = sizeof(IZ_FLOAT) * 3,	// 接ベクトル
		E_MDL_VTX_SIZE_BLENDINDICES = sizeof(IZ_FLOAT) * 4,
		E_MDL_VTX_SIZE_BLENDWEIGHT  = sizeof(IZ_FLOAT) * 4,
	};

	enum E_MDL_JOINT_PARAM {
		E_MDL_JOINT_PARAM_TRANSLATE = 1 << 0,
		E_MDL_JOINT_PARAM_QUARTANION = 1 << 1,
		E_MDL_JOINT_PARAM_SCALE = 1 << 2,
	};

	enum {
		// TODO
		MDL_NAME_LEN = 31,
	};

	enum {
		// TODO
		MDL_CHUNK_MAGIC_GEOM       = 0x01234567,
		MDL_CHUNK_MAGIC_JOINT      = 0x12345678,
		MDL_CHUNK_MAGIC_TERMINATE  = 0x7fffffff,
	};

	// NOTE
	// フォーマット
	// +--------------------+
	// |   ファイルヘッダ   |
	// +--------------------+
	// | ジオメトリチャンク |
	// +--------------------+
	// | ジョイントチャンク |
	// +--------------------+
	// | マテリアルチャンク |
	// +--------------------+
	
	struct S_MDL_HEADER {
		IZ_UINT magic;
		IZ_UINT version;

		IZ_UINT sizeHeader;
		IZ_UINT sizeFile;

		IZ_FLOAT maxVtx[3];
		IZ_FLOAT minVtx[3];
	};

	struct S_MDL_CHUNK_HEADER {
		IZ_UINT magicChunk;
	};

	/////////////////////////////////////////////////////////

	// マテリアル情報
	struct S_MDL_MTRL {
		CStdString<izChar, MDL_NAME_LEN> name;
		IZ_UINT nameKey;
	};

	/////////////////////////////////////////////////////////
	// ジオメトリチャンク

	// ジオメトリチャンク
	// +------------------------+
	// |     チャンクヘッダ     |
	// +------------------------+
	// |   ジオメトリチャンク   |
	// |         ヘッダ         |
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
	// | メッシュセットテーブル |
	// +------------------------+

	// ジオメトリチャンク情報
	struct S_MDL_GEOMETRY_CHUNK_HEADER {
		IZ_UINT16 numVB;
		IZ_UINT16 numMesh;
		IZ_UINT16 numMeshSubset;
		IZ_UINT16 numMeshSet;

		IZ_UINT sizeChunk;
	};

	// 頂点データ情報
	struct S_MDL_VERTICES {
		IZ_UINT16 sizeVtx;
		IZ_UINT16 numVtx;
	};

	// メッシュ情報
	struct S_MDL_MESH {
		IZ_UINT16 numSubset;

		// 頂点フォーマット
		IZ_UINT16 fmt;

		IZ_BOOL isTranslucent;
		IZ_FLOAT alpha;
	};

	// メッシュサブセット情報
	struct S_MDL_MESH_SUBSET {
		E_GRAPH_PRIM_TYPE typePrim;

		IZ_UINT16 idxVB;
		IZ_UINT16 minIdx;
		IZ_UINT16 maxIdx;
		IZ_UINT16 padding;

		IZ_UINT numIdx;						// インデックス数
		E_GRAPH_INDEX_BUFFER_FMT fmtIdx;	// インデックスフォーマット

		IZ_INT16 joints[4];
	};

	// メッシュセット情報
	struct S_MDL_MESH_SET {
		IZ_UINT16 startMeshIdx;
		IZ_UINT16 endMeshIdx;
	};

	/////////////////////////////////////////////////////////
	// ジョイントチャンク

	// ジョイントチャンク
	// +----------------------+
	// |    チャンクヘッダ    |
	// +----------------------+
	// |  ジョイントチャンク  |
	// |       ヘッダ         |
	// +----------------------+
	// |  ジョイントテーブル  |
	// | +------------------+ |
	// | |     ジョイント   | |
	// | |+----------------+| |
	// | ||     ヘッダ     || |
	// | |+----------------+| |
	// | |+----------------+| |
	// | ||    変形データ  || |
	// | |+----------------+| |
	// | |       ・・・     | |
	// | +------------------+ |
	// |        ・・・        |
	// +----------------------+

	struct S_MDL_JOINT_CHUNK_HEADER {
		IZ_UINT16 numJoint;
	};

	struct S_MDL_JOINT_POSE {
		IZ_FLOAT trans[3];
		SQuat quat;
		IZ_FLOAT scale[3];
	};

	struct S_MDL_JOINT {
		CStdString<izChar, MDL_NAME_LEN> name;
		IZ_UINT nameKey;

		// If parent is -1, joint has no parent.
		IZ_INT16 parent;
		IZ_UINT16 idx;

		SMatrix mtxInvBind;

		IZ_UINT8 validParam;
		IZ_UINT8 reserved[3];

		S_MDL_JOINT_POSE pose;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_MDL_FORMAT_H__)
