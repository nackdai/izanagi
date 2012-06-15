#if !defined(__MODEL_LIB_PMD_FORMAT_H__)
#define __MODEL_LIB_PMD_FORMAT_H__

#include <vector>
#include "PMDDefs.h"

// PMDフォーマット
// +---------------------+
// | ファイルヘッダ      |
// +---------------------+
// | 頂点チャンクヘッダ  |
// +---------------------+
// | 頂点データx頂点数   |
// +---------------------+
// | 面チャンクヘッダ    |
// +---------------------+
// | 頂点インデックス[3] |
// | x 面数              |
// +---------------------+
// | マテリアルヘッダ    |
// +---------------------+
// | マテリアルデータ    |
// | x マテリアル数      |
// +---------------------+
// |ボーンチャンクヘッダ |
// +---------------------+
// | ボーンデータ        |
// | x ボーン数          |
// +---------------------+
// |  IKチャンクヘッダ   |
// +---------------------+
// | IKデータ x データ数 |
// +---------------------+

// NOTE
// Some PMD structures are not aligned by 4...

#pragma pack(push,1)

// ファイルヘッダ.
struct SPmdHeader
{
	// "Pmd"
	char magic[3];

	float version;

	char modeName[20];
	char comment[256];
};

// 頂点チャンクヘッダ
struct SPmdVtxChunkHeader
{
	// 頂点数
	DWORD vtxNum;
};

// 頂点データ
struct SPmdVertex
{
	// 位置
	float pos[3];

	// 法線
	float normal[3];

	// UV
	float uv[2];
	
	// ボーンインデックス
	WORD boneIdx[2];

	// ボーン1に与える影響度 -> 0 - 100
	// ボーン2への影響度は、(100 - boneWeight)
	BYTE boneWeight;

	// 0:エッジ(輪郭)が有効、1:エッジ無効
	BYTE enableEdge;
};

// 面チャンクヘッダ
struct SPmdFaceChunkHeader
{
	// 面全体の頂点数
	// -> 頂点は重なるので、実際の頂点数より多くなる
	// -> １面あたり３頂点なので、面数 * 3 となる
	DWORD vtxNum;
};

// 面情報
struct SPmdFace
{
	WORD vtxIdx[3];
};

// マテリアルチャンクヘッダ
struct SPmdMtrlChunkHeader
{
	DWORD mtrlNum;
};

// マテリアル
struct SPmdMaterial
{
	// ディフューズ
	float diffuse[3];
	float alpha;

	float specularity;

	// スペキュラ
	float specular[3];

	// アンビエント
	float ambient[3];

	// トーンマップテクスチャへのインデックス
	// "toon??.bmp" の ?? の部分になる
	//  0.bmp:0xFF, 1(01).bmp:0x00 ・・・ 10.bmp:0x09
	BYTE idxToon;

	// 0:エッジ(輪郭)が有効、1:エッジ無効
	BYTE enableEdge;

	// 面頂点数
	// インデックスに変換する場合は、マテリアル[0]から順に加算
	DWORD faceVtxNum;

	// テクスチャファイル名またはスフィアファイル名
	char texFileName[20];
};

// ボーンチャンクヘッダ
struct SPmdBoneChunkHeader
{
	WORD  boneNum;
};

// ボーン
struct SPmdBone
{
	// ボーン名
	char boneName[20];

	// 親ボーン番号(ない場合は0xFFFF)
	WORD parentIdx;

	// tail位置のボーン番号(チェーン末端の場合は0xFFFF)
	// 親：子は1：多なので、主に位置決め用
	WORD tailBoneIdx;

	// ボーンの種類
	BYTE boneType;

	// IKボーン番号(影響IKボーン。ない場合は0)
	WORD idxIKParent;

	// ボーンのヘッドの位置
	float boneHeadPos[3];
};

// IKチャンクヘッダ
struct SPmdIkChunkHeader
{
	WORD ikNum;
};

// IKデータ
struct SPmdIk
{
	struct {
		// IKボーン番号
		WORD idx;

		// IKターゲットボーン番号
		// IKボーンが最初に接続するボーン
		WORD targetBoneIdx;	

		// IKチェーンの長さ(子の数)
		BYTE chainNum;

		WORD iterations;	// 再帰演算回数
		float weight;		// IKの影響度
	} param;

	// IK影響下のボーン番号
	std::vector<WORD> childBoneIdx;
};

#pragma pack(pop)

#endif	// #if !defined(__MODEL_LIB_PMD_FORMAT_H__)
