#if !defined(__URANUS_FONT_FNT_ENCODE_H__)
#define __URANUS_FONT_FNT_ENCODE_H__

#include "unDefs.h"
#include "graph/GraphDefs.h"
#include "FontDefs.h"

// FNT = FoNT

namespace uranus {
	// FNT�t�@�C���o�[�W����
	enum E_FNT_VERSION {
		E_FNT_VERSION_0010 = 0x30303130,				// �o�[�W����0010

		E_FNT_VERSION_CURRENT = E_FNT_VERSION_0010,		// ���݂̃o�[�W����
		E_FNT_VERSION_FORCE_INT32 = 0x7FFFFFFF,			// enum �̃T�C�Y�� UN_INT �Ɠ����ɂ��邽��.
	};

	enum {
		// "FNT "
		FNT_MAGIC_NUMBER = 0x464E5400,
	};

	enum {
		// �t�H���g�T�C�Y�ő�
		// ���ꂾ������΂����ł���
		FNT_MAX_FONT_SIZE = 64,

		// �������ő�
		// ���ꂾ������΂����ł���
		FNT_MAX_CHAR_NUM = 65535,
	};

	// NOTE
	// �t�H�[�}�b�g
	// +----------------+
	// | �t�@�C���w�b�_ |
	// +----------------+
	// |   �}�b�v���   |
	// +----------------+-+
	// |�t�H���g�C���[�W| |
	// |      ���      | |
	// +----------------+ +-- �f�[�^������
	// |    �t�H���g    | |
	// | �C���[�W�f�[�^ | |
	// +----------------+-+

	// �t�@�C���w�b�_
	struct S_FNT_HEADER {
		UN_UINT magic;
		UN_UINT version;

		UN_UINT sizeFile;			// �t�@�C����
		UN_UINT sizeHeader;			// �w�b�_��

		UN_UINT16 numFont;			// �t�H���g��
		UN_UINT8 numTex;			// �e�N�X�`����
		UN_UINT8 fontHeight;		// �t�H���g����

		UN_UINT sizeMaxImage;		// �ő�C���[�W�T�C�Y

		E_FONT_CHAR_ENCODE charEncode;	// �����R�[�h

		E_GRAPH_PIXEL_FMT texFmt;		// �e�N�X�`���t�H�[�}�b�g

		struct {
			UN_UINT isFixedPitch	: 1;	// �Œ�s�b�`���ǂ���
			UN_UINT reserved		: 31;
		};
	};

	// �t�H���g�C���[�W���
	struct S_FNT_IMAGE {
		UN_UINT sizeData;		// �f�[�^�T�C�Y

		UN_UINT16 numFont;		// �`�����܂ꂽ������
		UN_UINT16 pitch;		// �s�b�`

		UN_UINT16 width;
		UN_UINT16 height;

		const UN_UINT8* images;	// �f�[�^
	};

	// �t�H���g�}�b�v���
	struct S_FNT_MAP {
		UN_UINT16 idx;			// �����͉��Ԗڂ�
		UN_UINT16 padding;

		UN_UINT code;			// �����R�[�h

		// �C���[�W�ʒu
		UN_UINT16 srcX;
		UN_UINT16 srcY;

		UN_UINT16 fontWidth;	// �t�H���g��
		UN_UINT8 texID;			// �����e�N�X�`��
		UN_UINT8 reserved;
	};
}	// namespace uranus

// FNT�t�H�[�}�b�g���ǂ���
#define IS_FMT_ENCODE(f)			((f) == uranus::FNT_MAGIC_NUMBER)

// �ŐV�o�[�W�������ǂ���
#define IS_CURRENT_FNT_VERSION(v)	((v) == uranus::E_FNT_VERSION_CURRENT)

#endif	// #if !defined(__URANUS_FONT_FNT_ENCODE_H__)
