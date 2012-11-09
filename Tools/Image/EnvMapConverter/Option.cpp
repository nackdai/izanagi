#include <shlwapi.h>
#include "Option.h"

namespace {
	// オプションで共有するバッファ
	CHAR s_BUF[1024]; 

	inline EnvMapType _GetEnvMapType(const char* arg)
	{
		EnvMapType ret = EnvMapTypeNum;
		izanagi::tool::CString opt(arg);

		if (opt == "m") {
			// mirror
			ret = EnvMapTypeMirror;
		}
		else if (opt == "l") {
			// latitude-longitude
			ret = EnvMapTypeLatLong;
		}
		else if (opt == "a") {
			// angular
			ret = EnvMapTypeAngular;
		}
		else if (opt == "c") {
			// cube
			ret = EnvMapTypeCube;
		}
        else if (opt == "cr") {
            // cross
            ret = EnvMapTypeCross;
        }
		
		return ret;
	}
}

COption::COption()
{
	typeExport = izanagi::tool::TextureExportTypePNG;
	typeInEnvMap = EnvMapTypeNum;
	typeOutEnvMap = EnvMapTypeNum;
}

// 解析
IZ_BOOL COption::Analysis(int argc, TCHAR* argv[])
{
	for (int i = 1; i < argc; i++) {
		BOOL result = FALSE;
		izanagi::tool::CString cmd(argv[i]);

		if (i < argc - 1) {
			if (result = (cmd == "-i")) {
				// -i
				in.format("%s", argv[++i]);
				in.replace('/', '\\');
			}
			else if (result = (cmd == "-o")) {
				// -o
				out.format("%s", argv[++i]);
			}
			else if (result = (cmd == "-ie")) {
				// -ie
				typeInEnvMap = _GetEnvMapType(argv[++i]);
			}
			else if (result = (cmd == "-oe")) {
				// -oe
				typeOutEnvMap = _GetEnvMapType(argv[++i]);
			}
			else if (result = (cmd == "-f")) {
				// -f
				izanagi::tool::CString opt(argv[++i]);

				// 小文字変換
				opt.make_lower();

				if (opt == "png") {
					typeExport = izanagi::tool::TextureExportTypePNG;
				}
				else if (opt == "hdr") {
					typeExport = izanagi::tool::TextureExportTypeHDR;
				}
			}
		}

		if (!result) {
			// 引数無しオプション
		}

		if (!result) {
			// TODO
			printf("無効なオプションです[%s]\n\n", cmd);
			//IZ_ASSERT(FALSE);
			return FALSE;
		}
	}

	// 後処理
	if (!AfterAnalysis()) {
		IZ_ASSERT(FALSE);
		return FALSE;
	}

	return TRUE;
}

IZ_BOOL COption::IsValid() const
{
	IZ_BOOL b0 = (!out.empty() && !in.empty());
	IZ_BOOL b1 = (typeInEnvMap != EnvMapTypeNum);
	IZ_BOOL b2 = (typeOutEnvMap != EnvMapTypeNum);
	IZ_BOOL b3 = (typeInEnvMap != typeOutEnvMap);

	return (b0 && b1 && b2 && b3);
}

// 解析の後処理
IZ_BOOL COption::AfterAnalysis()
{
	memset(s_BUF, 0, sizeof(s_BUF));

	IZ_BOOL ret = IZ_TRUE;

	if (out.empty() && !in.empty()) {
		// 出力ファイルが設定されていない
		// 入力ファイルを基に出力ファイル名を決める
		memcpy(s_BUF, in, min(sizeof(s_BUF), strlen(in)));

		// ファイル名取得
		LPSTR file_name = PathFindFileName(s_BUF);

		// 拡張子削除
		PathRemoveExtension(file_name);

		// 出力ファーマットに応じて
		switch (typeExport) {
		case izanagi::tool::TextureExportTypePNG:
			{
				out.format("%s.png", file_name);
				ret = IZ_TRUE;
			}
			break;
		case izanagi::tool::TextureExportTypeHDR:
			{
				out.format("%s.hdr", file_name);
				ret = IZ_TRUE;
			}
			break;
		default:
			VRETURN(IZ_FALSE);
			break;
		}
		
		memset(s_BUF, 0, sizeof(s_BUF));
	}

	return ret;
}
