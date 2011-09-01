#include "Option.h"
#include "izToolKit.h"

COption::COption()
{
	exportType = ExportTypeMsh | ExportTypeSkl;
	fileType = FileTypeUnknown;
}

COption::~COption()
{
}

// ���
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
			else if (result = (opt == "-e")) {
				std::string subOpt(argv[++i]);

				if (result = (subOpt == "mdl")) {
					exportType = ExportTypeMdl;
				}
				else if (result = (subOpt == "msh")) {
					exportType = ExportTypeMsh;
				}
				else if (result = (subOpt == "skl")) {
					exportType = ExportTypeSkl;
				}
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
		}

		if (!result) {
			// TODO
			printf("�����ȃI�v�V�����ł�[%s]\n\n", opt.c_str());
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
	if (out.empty()
		|| (outMsh.empty() && (exportType & ExportTypeMsh))
		|| (outSkl.empty() && (exportType & ExportTypeSkl)))
	{
		// �o�̓t�@�C������Ȃ̂ŁA���̓t�@�C������쐬����

		// �g���q�����������̓t�@�C�������擾
		IZ_BOOL result = izanagi::izanagi_tk::CFileUtility::RemoveExtension(
							BUF,
							sizeof(BUF),
							in.c_str());
		VRETURN(result);

		// �o�̓^�C�v�ɉ����Ċg���q��t������
		if (exportType == ExportTypeMdl) {
			izanagi::izanagi_tk::CStringUtil::Format(
				out,
				"%s.mdl",
				BUF);
		}
		else {
			if (exportType & ExportTypeMsh) {
				izanagi::izanagi_tk::CStringUtil::Format(
					outMsh,
					"%s.msh",
					BUF);
			}
			if (exportType & ExportTypeSkl) {
				izanagi::izanagi_tk::CStringUtil::Format(
					outSkl,
					"%s.skl",
					BUF);
			}
		}
	}

	if (fileType == FileTypeUnknown) {
		// ���̓t�@�C���̊g���q���画�肷��

		// �g���q���擾
		IZ_PCSTR ext = izanagi::izanagi_tk::CFileUtility::GetExtension(
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

	return IZ_TRUE;
}

IZ_BOOL COption::IsValid()
{
	VRETURN(!in.empty());
	VRETURN(!out.empty() || !outMsh.empty() || !outSkl.empty());
	VRETURN(fileType != FileTypeUnknown);

	return IZ_TRUE;
}
