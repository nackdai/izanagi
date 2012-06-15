#if !defined(__MODEL_LIB_PMD_FORMAT_H__)
#define __MODEL_LIB_PMD_FORMAT_H__

#include <vector>
#include "PMDDefs.h"

// PMD�t�H�[�}�b�g
// +---------------------+
// | �t�@�C���w�b�_      |
// +---------------------+
// | ���_�`�����N�w�b�_  |
// +---------------------+
// | ���_�f�[�^x���_��   |
// +---------------------+
// | �ʃ`�����N�w�b�_    |
// +---------------------+
// | ���_�C���f�b�N�X[3] |
// | x �ʐ�              |
// +---------------------+
// | �}�e���A���w�b�_    |
// +---------------------+
// | �}�e���A���f�[�^    |
// | x �}�e���A����      |
// +---------------------+
// |�{�[���`�����N�w�b�_ |
// +---------------------+
// | �{�[���f�[�^        |
// | x �{�[����          |
// +---------------------+
// |  IK�`�����N�w�b�_   |
// +---------------------+
// | IK�f�[�^ x �f�[�^�� |
// +---------------------+

// NOTE
// Some PMD structures are not aligned by 4...

#pragma pack(push,1)

// �t�@�C���w�b�_.
struct SPmdHeader
{
	// "Pmd"
	char magic[3];

	float version;

	char modeName[20];
	char comment[256];
};

// ���_�`�����N�w�b�_
struct SPmdVtxChunkHeader
{
	// ���_��
	DWORD vtxNum;
};

// ���_�f�[�^
struct SPmdVertex
{
	// �ʒu
	float pos[3];

	// �@��
	float normal[3];

	// UV
	float uv[2];
	
	// �{�[���C���f�b�N�X
	WORD boneIdx[2];

	// �{�[��1�ɗ^����e���x -> 0 - 100
	// �{�[��2�ւ̉e���x�́A(100 - boneWeight)
	BYTE boneWeight;

	// 0:�G�b�W(�֊s)���L���A1:�G�b�W����
	BYTE enableEdge;
};

// �ʃ`�����N�w�b�_
struct SPmdFaceChunkHeader
{
	// �ʑS�̂̒��_��
	// -> ���_�͏d�Ȃ�̂ŁA���ۂ̒��_����葽���Ȃ�
	// -> �P�ʂ�����R���_�Ȃ̂ŁA�ʐ� * 3 �ƂȂ�
	DWORD vtxNum;
};

// �ʏ��
struct SPmdFace
{
	WORD vtxIdx[3];
};

// �}�e���A���`�����N�w�b�_
struct SPmdMtrlChunkHeader
{
	DWORD mtrlNum;
};

// �}�e���A��
struct SPmdMaterial
{
	// �f�B�t���[�Y
	float diffuse[3];
	float alpha;

	float specularity;

	// �X�y�L����
	float specular[3];

	// �A���r�G���g
	float ambient[3];

	// �g�[���}�b�v�e�N�X�`���ւ̃C���f�b�N�X
	// "toon??.bmp" �� ?? �̕����ɂȂ�
	//  0.bmp:0xFF, 1(01).bmp:0x00 �E�E�E 10.bmp:0x09
	BYTE idxToon;

	// 0:�G�b�W(�֊s)���L���A1:�G�b�W����
	BYTE enableEdge;

	// �ʒ��_��
	// �C���f�b�N�X�ɕϊ�����ꍇ�́A�}�e���A��[0]���珇�ɉ��Z
	DWORD faceVtxNum;

	// �e�N�X�`���t�@�C�����܂��̓X�t�B�A�t�@�C����
	char texFileName[20];
};

// �{�[���`�����N�w�b�_
struct SPmdBoneChunkHeader
{
	WORD  boneNum;
};

// �{�[��
struct SPmdBone
{
	// �{�[����
	char boneName[20];

	// �e�{�[���ԍ�(�Ȃ��ꍇ��0xFFFF)
	WORD parentIdx;

	// tail�ʒu�̃{�[���ԍ�(�`�F�[�����[�̏ꍇ��0xFFFF)
	// �e�F�q��1�F���Ȃ̂ŁA��Ɉʒu���ߗp
	WORD tailBoneIdx;

	// �{�[���̎��
	BYTE boneType;

	// IK�{�[���ԍ�(�e��IK�{�[���B�Ȃ��ꍇ��0)
	WORD idxIKParent;

	// �{�[���̃w�b�h�̈ʒu
	float boneHeadPos[3];
};

// IK�`�����N�w�b�_
struct SPmdIkChunkHeader
{
	WORD ikNum;
};

// IK�f�[�^
struct SPmdIk
{
	struct {
		// IK�{�[���ԍ�
		WORD idx;

		// IK�^�[�Q�b�g�{�[���ԍ�
		// IK�{�[�����ŏ��ɐڑ�����{�[��
		WORD targetBoneIdx;	

		// IK�`�F�[���̒���(�q�̐�)
		BYTE chainNum;

		WORD iterations;	// �ċA���Z��
		float weight;		// IK�̉e���x
	} param;

	// IK�e�����̃{�[���ԍ�
	std::vector<WORD> childBoneIdx;
};

#pragma pack(pop)

#endif	// #if !defined(__MODEL_LIB_PMD_FORMAT_H__)
