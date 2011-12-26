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
		E_MSH_VTX_SIZE_POS          = sizeof(IZ_FLOAT) * 4,	// 頂点位置
		E_MSH_VTX_SIZE_NORMAL       = sizeof(IZ_FLOAT) * 3,	// 法線
		E_MSH_VTX_SIZE_COLOR        = sizeof(IZ_COLOR),		// 頂点カラー
		E_MSH_VTX_SIZE_UV           = sizeof(IZ_FLOAT) * 2,	// UV座標
		E_MSH_VTX_SIZE_TANGENT      = sizeof(IZ_FLOAT) * 3,	// 接ベクトル
		E_MSH_VTX_SIZE_BLENDINDICES = sizeof(IZ_FLOAT) * 4,
		E_MSH_VTX_SIZE_BLENDWEIGHT  = sizeof(IZ_FLOAT) * 4,
	};

	enum {
		// TODO
		MSH_NAME_LEN = 31,

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

		IZ_UINT numAllJointIndices;
	};

	/////////////////////////////////////////////////////////

	// マテリアル情報
	struct S_MSH_MTRL {
		CStdString<izChar, MSH_NAME_LEN> name;
		IZ_UINT nameKey;
	};

	/////////////////////////////////////////////////////////

	// 頂点データ情報
	struct S_MSH_VERTICES {
		IZ_UINT16 sizeVtx;
		IZ_UINT16 numVtx;
	};

	// メッシュグループ情報
	struct S_MSH_MESH_GROUP {
		IZ_UINT16 numVB;
		IZ_UINT16 numMeshSet;
	};

	// メッシュセット情報
	struct S_MSH_MESH_SET {
		IZ_UINT16 numSubset;

		// 頂点フォーマット
		IZ_UINT16 fmt;

		IZ_FLOAT center[3];

		S_MSH_MTRL mtrl;
	};

	// プリミティブセット情報
	struct S_MSH_PRIM_SET {
		E_GRAPH_PRIM_TYPE typePrim;

		IZ_UINT16 idxVB;
		IZ_UINT16 minIdx;
		IZ_UINT16 maxIdx;
		IZ_UINT16 numJoints;

		IZ_UINT numIdx;						// インデックス数
		E_GRAPH_INDEX_BUFFER_FMT fmtIdx;	// インデックスフォーマット

		IZ_INT16* joints;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_MSH_FORMAT_H__)
