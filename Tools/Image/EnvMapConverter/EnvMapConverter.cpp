// ImageBuilder.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#ifndef _WIN32_WINNT		// Windows XP 以降のバージョンに固有の機能の使用を許可します。                   
#define _WIN32_WINNT 0x0501	// これを Windows の他のバージョン向けに適切な値に変更してください。
#endif						

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
	izanagi::izanagi_tk::CTextureLite* texInEnv = IZ_NULL;
	izanagi::izanagi_tk::CTextureLite* texOutEnv = IZ_NULL;

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

	// 環境マップ読み込み
	texInEnv = device->CreateTextureFromFile(option.in);
	VGOTO(texInEnv != NULL);

	// 出力先のサイズを計算
	IZ_UINT outWidth, outHeight;
	IZ_BOOL isValid = _ComputeSize(
						option.typeOutEnvMap,
						texInEnv->GetWidth(),
						texInEnv->GetHeight(),
						outWidth,
						outHeight);
	VGOTO(isValid);

	// 出力先作成
	// 強制的にRGBA32Fにする
	texOutEnv = device->CreateTexture(
					outWidth,
					outHeight,
					izanagi::E_GRAPH_PIXEL_FMT_RGBA32F);
	VGOTO(texOutEnv != NULL);

	// 変換
	Convert(
		texInEnv, option.typeInEnvMap,
		texOutEnv, option.typeOutEnvMap);

	// 出力先保存
	result = device->SaveTexture(
				option.out,
				texOutEnv,
				option.typeExport);
	VGOTO(result);

__EXIT__:
	SAFE_RELEASE(texInEnv);
	SAFE_RELEASE(texOutEnv);
	SAFE_RELEASE(device);

	return nRetCode;
}
