// AnimationConverter.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "izModelLib.h"
#include "izToolKit.h"

int _tmain(int argc, _TCHAR* argv[])
{
#if 0
	IImporter* pImporter = IImporter::CreateImporter<CColladaImporter>();
	//pImporter->Open("cube.dae");
	//pImporter->Open("sphere.dae");
	//pImporter->Open("duck.dae");
	//pImporter->Open("dice.dae");
	pImporter->Open("Seymour.dae");
	pImporter->ReadBaseModel("Seymour.dae");
#else
	IImporter* pImporter = IImporter::CreateImporter<CXFileImporter>();

	//static IZ_PCSTR pszXFile = "Dwarf.x";
	static IZ_PCSTR pszXFile = "tiny.x";

	std::string strXFile(pszXFile);

#if 0
	IZ_BOOL bNeedRemoveFile = IZ_FALSE;

	if (CXFileImporter::IsTxtFormat(pszXFile)) {
		// Get temporary file name.
		std::string strTmp;
		izanagi::izanagi_tk::CFileUtility::CreateTmpFileName(strTmp);
		strTmp += ".tmp";

		// ウインドウハンドル取得
		HWND hWnd = ::GetConsoleWindow();

		// Create GraphicsDevice.
		izanagi::izanagi_tk::CGraphicsDeviceLite* pDevice = izanagi::izanagi_tk::CGraphicsDeviceLite::CreateGraphicsDeviceLight(hWnd);

		// Convert text format to binary format.
		Dix::convertXFileFormat(
			pDevice->GetRawInterface(),
			_T(strXFile.c_str()),
			_T(strTmp.c_str()),
			D3DXF_FILEFORMAT_BINARY);

		// Rename.
		strXFile = strTmp;
		bNeedRemoveFile = IZ_TRUE;

		SAFE_RELEASE(pDevice);
	}
#endif

	pImporter->Open(strXFile.c_str());
	pImporter->ReadBaseModel(strXFile.c_str());

#if 0
	if (bNeedRemoveFile) {
		// Remove file.
		izanagi::izanagi_tk::CFileUtility::RemoveFile(strXFile.c_str());
	}
#endif
#endif

	CAnmExporter::GetInstance().Export(
		"test.anm",
		0,
		pImporter);

	return 0;
}

