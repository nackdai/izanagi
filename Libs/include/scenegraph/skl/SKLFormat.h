#if !defined(__IZANAGI_SCENEGRAPH_SKL_FORMAT_H__)
#define __IZANAGI_SCENEGRAPH_SKL_FORMAT_H__

#include "izDefs.h"
#include "izStd.h"
#include "izMath.h"
#include "izGraph.h"

// SKL = MoDeL

namespace izanagi {
	enum E_SKL_JOINT_PARAM {
		E_SKL_JOINT_PARAM_TRANSLATE  = 1 << 0,
		E_SKL_JOINT_PARAM_QUARTANION = 1 << 1,
		E_SKL_JOINT_PARAM_SCALE      = 1 << 2,
	};

	enum {
		// TODO
		SKL_NAME_LEN = 31,
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
	
	struct S_SKL_HEADER {
		IZ_UINT magic;
		IZ_UINT version;

		IZ_UINT sizeHeader;
		IZ_UINT sizeFile;

		IZ_UINT numJoint;
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

	struct S_SKL_JOINT_POSE {
		IZ_FLOAT trans[3];
		SQuat quat;
		IZ_FLOAT scale[3];
	};

	struct S_SKL_JOINT {
		CStdString<izChar, SKL_NAME_LEN> name;
		IZ_UINT nameKey;

		// If parent is -1, joint has no parent.
		IZ_INT16 parent;
		IZ_UINT16 idx;

		SMatrix mtxInvBind;

		IZ_UINT8 validParam;
		IZ_UINT8 validAnmParam;
		IZ_UINT8 reserved[2];

		S_SKL_JOINT_POSE pose;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_SKL_FORMAT_H__)
