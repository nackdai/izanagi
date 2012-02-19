#if !defined(__IZANAGI_SCENEGRAPH_MTRL_FORMAT_H__)
#define __IZANAGI_SCENEGRAPH_MTRL_FORMAT_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izShader.h"

// MTRL = MaTeRiaL

namespace izanagi {
	enum E_MTRL_VERSION {
		E_MTRL_VERSION_0010 = 0x30303130,				// バージョン0010

		E_MTRL_VERSION_CURRENT = E_MTRL_VERSION_0010,	// 現在のバージョン
		E_MTRL_VERSION_FORCE_INT32 = 0x7FFFFFFF,
	};

	enum {
		MTRL_MATERIAL_NAME_LEN = 63,
		MTRL_TEXTURE_NAME_LEN  = 63,
		MTRL_SHADER_NAME_LEN   = 63,
		MTRL_PARAM_NAME_LEN    = 63,
	};

	enum E_MTRL_PARAM_TYPE {
		E_MTRL_PARAM_TYPE_FLOAT = 0,
		E_MTRL_PARAM_TYPE_VECTOR,
		E_MTRL_PARAM_TYPE_MATRIX,
		E_MTRL_PARAM_TYPE_UINT,
		E_MTRL_PARAM_TYPE_BOOL,

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

	/**
	*/
	struct S_MTRL_HEADER {
		IZ_UINT magic;
		E_MTRL_VERSION version;

		IZ_UINT sizeFile;
		IZ_UINT sizeHeader;

#if 0
		IZ_UINT numMtrl;
#endif
	};

	/**
	*/
	struct S_MTRL_MATERIAL {
		CStdString<izChar, MTRL_MATERIAL_NAME_LEN> name;
		IZ_UINT keyMaterial;

		IZ_UINT16 numTex;
		IZ_UINT16 numShader;

		IZ_UINT16 numParam;
		IZ_UINT16 paramBytes;
	};

	/**
	*/
	union S_MTRL_TEXTURE_TYPE {
		struct {
			IZ_UINT isNormal		: 1;
			IZ_UINT isSpecular		: 1;
			IZ_UINT isEnvironment	: 1;
			IZ_UINT isVolume		: 1;
		};
		IZ_UINT flags;
	};

	/**
	*/
	struct S_MTRL_TEXTURE {
		CStdString<izChar, MTRL_TEXTURE_NAME_LEN> name;
		IZ_UINT key;

		S_MTRL_TEXTURE_TYPE type;

	private:
		CBaseTexture* tex;

		friend class CMaterial;
	};

	/**
	*/
	struct S_MTRL_SHADER {
		CStdString<izChar, MTRL_SHADER_NAME_LEN> name;
		IZ_UINT key;

	private:
		IShader* shader;
		IZ_UINT tech_idx;

		friend class CMaterial;
	};

	/**
	*/
	struct S_MTRL_PARAM {
		CStdString<izChar, MTRL_PARAM_NAME_LEN> name;
		IZ_UINT key;

		E_MTRL_PARAM_TYPE type;

		IZ_UINT16 elements;	// 要素数
		IZ_UINT16 bytes;	// データサイズ

	private:
		void* param;
		IZ_SHADER_HANDLE handle;

		friend class CMaterial;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_MTRL_FORMAT_H__)
