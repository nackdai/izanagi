// ImageBuilder.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#ifndef _WIN32_WINNT		// Windows XP 以降のバージョンに固有の機能の使用を許可します。                   
#define _WIN32_WINNT 0x0501	// これを Windows の他のバージョン向けに適切な値に変更してください。
#endif						

#include <vector>
#include "izToolKit.h"
#include "izMath.h"
#include "Option.h"
#include "EnvMapConverterImpl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

namespace {
	inline void _DispUsage()
	{
		printf(
			"EnvMapConverter Ver.0.01\n"
			"\n"
			"[USAGE]\n"
			"EnvMapConverter.exe -i [in] -o [out]\n"
			"\n"
			" -i [tex]   : 基になる環境マップのファイル名\n"
			" -o [out]   : 出力ファイル名\n"
			"              指定が無い場合は入力ファイルをもとに自動で設定される\n"
			" -ie [type] : 入力環境マップのタイプ\n"
			"              m : mirror map\n"
			"              l : latitude-longitude map\n"
			"              a : angular map\n"
			"              c : cube map\n"
			" -oe [type] : 出力環境マップのタイプ\n"
			"              m : mirror map\n"
			"              l : latitude-longitude map\n"
			"              a : angular map\n"
			"              c : cube map\n"
			" -f [type]  : 出力ファイルのタイプ\n"
			"               png : PNG\n"
			"               hdr : HDR\n"
			"              指定が無い場合はPNGとして出力される\n"
		);
	}

	// 出力テクスチャサイズを計算
	IZ_BOOL _ComputeSize(
		EnvMapType type,
		IZ_UINT inWidth, IZ_UINT inHeight,
		IZ_UINT& outWidth, IZ_UINT& outHeight)
	{
		IZ_BOOL isValid = IZ_TRUE;

		switch (type) {
		case EnvMapTypeMirror:
		case EnvMapTypeAngular:
		case EnvMapTypeCube:
			{
				// 幅・高さは同じでないといけない
				IZ_UINT size = min(inWidth, inHeight);
				outWidth = size;
				outHeight = size;
			}
			break;
		case EnvMapTypeLatLong:
			{
				// 基になるサイズは幅と高さが一致しないといけない
				isValid = (inWidth == inHeight);

				// 幅が高さの２倍でないといけない
				outWidth = inWidth * 2;
				outHeight = inWidth;
			}
			break;
		}

		return isValid;
	}

	// キューブマップ用プレフィックス
	static const char* PreName[] = {
		"XP", "XN",
		"YP", "YX",
		"ZP", "ZN"
	};
	C_ASSERT(COUNTOF(PreName) == izanagi::E_GRAPH_CUBE_TEX_FACE_NUM);

	// テクスチャ名一覧を取得
	void _GetTexNameList(
		const izanagi::izanagi_tk::CString texName,
		EnvMapType type,
		std::vector<izanagi::izanagi_tk::CString>& texList)
	{
		if (type == EnvMapTypeCube) {
			texList.resize(izanagi::E_GRAPH_CUBE_TEX_FACE_NUM);

			// 頭に面ごとのプレフィックスを付ける

			// ファイル名のみ取得
			IZ_PCSTR name = izanagi::izanagi_tk::CFileUtility::GetFileNameFromPath(texName.c_str());

			// ディレクトリパス部分のみ取得
			char dir[1024];
			izanagi::izanagi_tk::CFileUtility::GetPathWithoutFileName(dir, sizeof(dir), texName.c_str());

			for (IZ_UINT i = 0; i < izanagi::E_GRAPH_CUBE_TEX_FACE_NUM; i++) {
				if (::strlen(dir) > 0) {
					texList[i].format("%s\\%s_%s", dir, PreName[i], name);
				}
				else {
					texList[i].format("%s_%s", PreName[i], name);
				}
			}
		}
		else {
			texList.push_back(texName);
		}
	}

	// テクスチャを解放
	void _ReleaseTexture(
		std::vector<izanagi::izanagi_tk::CTextureLite*>& texList)
	{
		for (size_t i = 0; i < texList.size(); i++) {
			izanagi::izanagi_tk::CTextureLite* tex = texList[i];
			SAFE_RELEASE(tex);
		}
		texList.clear();
	}
}	// namespace

#ifdef VGOTO
#undef VGOTO
#endif

#define VGOTO(b)	if (!(b)) { IZ_ASSERT(FALSE); nRetCode = 1; goto __EXIT__; }

int main(int argc, char* argv[])
{
	int nRetCode = 0;

	COption option;

	izanagi::izanagi_tk::CGraphicsDeviceLite* device = IZ_NULL;

	std::vector<izanagi::izanagi_tk::CTextureLite*> texInEnv;
	std::vector<izanagi::izanagi_tk::CTextureLite*> texOutEnv;

	std::vector<izanagi::izanagi_tk::CString> inTexList;
	std::vector<izanagi::izanagi_tk::CString> outTexList;

	// ウインドウハンドル取得
	HWND hWnd = ::GetConsoleWindow();
	VGOTO(hWnd != NULL);

	// グラフィックスデバイス作成
	device = izanagi::izanagi_tk::CGraphicsDeviceLite::CreateGraphicsDeviceLight(hWnd);
	VGOTO(device != NULL);

	// オプション解析
	IZ_BOOL result = option.Analysis(argc, argv);
	VGOTO(result);
	VGOTO(option.IsValid());

	_GetTexNameList(
		option.in,
		option.typeInEnvMap,
		inTexList);

	// 環境マップ読み込み
	for (size_t i = 0; i < inTexList.size(); i++) {
		izanagi::izanagi_tk::CTextureLite* tex = device->CreateTextureFromFile(inTexList[i]);
		VGOTO(tex != NULL);

		texInEnv.push_back(tex);
	}

	// 出力先のサイズを計算
	IZ_UINT outWidth, outHeight;
	IZ_BOOL isValid = _ComputeSize(
						option.typeOutEnvMap,
						texInEnv[0]->GetWidth(),
						texInEnv[0]->GetHeight(),
						outWidth,
						outHeight);
	VGOTO(isValid);

	_GetTexNameList(
		option.out,
		option.typeOutEnvMap,
		outTexList);

	// 出力先作成
	// 強制的にRGBA32Fにする
	for (size_t i = 0; i < outTexList.size(); i++) {
		izanagi::izanagi_tk::CTextureLite* tex = device->CreateTexture(
													outWidth,
													outHeight,
													izanagi::E_GRAPH_PIXEL_FMT_RGBA32F);
		VGOTO(tex != NULL);

		texOutEnv.push_back(tex);
	}

	// 変換
	Convert(
		texInEnv, option.typeInEnvMap,
		texOutEnv, option.typeOutEnvMap);

	// 出力先保存
	for (size_t i = 0; i < outTexList.size(); i++) {
		result = device->SaveTexture(
					outTexList[i],
					texOutEnv[i],
					option.typeExport);
		VGOTO(result);
	}

__EXIT__:
	_ReleaseTexture(texInEnv);
	_ReleaseTexture(texOutEnv);
	SAFE_RELEASE(device);

	return nRetCode;
}
