#include "Option.h"
#include "izToolKit.h"

COption::COption()
{
	fileType = FileTypeUnknown;
	idx = 0;
}

COption::~COption()
{
}

// 解析
IZ_BOOL COption::Analysis(int argc, char* argv[])
{
	IZ_BOOL result = IZ_TRUE;

	for (IZ_INT i = 1; i < argc; i++) {
		std::string opt(argv[i]);

		if (i < argc - 1) {
			if (result = (opt == "-i")) {
				in = argv[++i];
			}
			else if (result = (opt == "-o")) {
				out = argv[++i];
			}
			else if (result = (opt == "-b")) {
				base = argv[++i];
			}
			else if (result = (opt == "-f")) {
				std::string subOpt(argv[++i]);

				if (result = (subOpt == "dae")) {
					fileType = FileTypeCollada;
				}
				else if (result = (subOpt == "x")) {
					fileType = FileTypeXFile;
				}
				else if (result = (subOpt == "fbx")) {
					fileType = FileTypeFBX;
				}
			}
			else if (result = (opt == "-idx")) {
				idx = ::atoi(argv[++i]);
			}
		}

		if (!result) {
			// TODO
			printf("無効なオプションです[%s]\n\n", opt.c_str());
			//ASSERT(FALSE);
			return IZ_FALSE;
		}
	}

	IZ_BOOL ret = AnalysisInternal();

	return ret;
}

static char BUF[MAX_PATH];

IZ_BOOL COption::AnalysisInternal()
{
	if (out.empty()) {
		// 出力ファイルが空なので、入力ファイルから作成する

		// 拡張子を除いた入力ファイル名を取得
		IZ_BOOL result = izanagi::tool::CFileUtility::RemoveExtension(
							BUF,
							sizeof(BUF),
							in.c_str());
		VRETURN(result);

		out.format("%s.anm", BUF);
	}

	if (fileType == FileTypeUnknown) {
		// 入力ファイルの拡張子から判定する

		// 拡張子を取得
		IZ_PCSTR ext = izanagi::tool::CFileUtility::GetExtension(
						BUF,
						sizeof(BUF),
						in.c_str());
		VRETURN(ext != IZ_NULL);

		std::string strExt(ext);

		if (strExt == "dae") {
			fileType = FileTypeCollada;
		}
		else if (strExt == "x") {
			fileType = FileTypeXFile;
		}
		else if (strExt == "fbx") {
		}
	}

	if (base.empty()) {
		// ベースモデルファイルが空なら入力ファイルと同じにする
		base = in;
	}

	return IZ_TRUE;
}

IZ_BOOL COption::IsValid()
{
	VRETURN(!in.empty());
	VRETURN(!out.empty());
	VRETURN(!base.empty());
	VRETURN(fileType != FileTypeUnknown);

	return IZ_TRUE;
}
