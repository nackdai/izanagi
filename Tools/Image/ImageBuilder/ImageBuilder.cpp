// ImageBuilder.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ImageBuilder.h"

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>

#include "ToolKitGraphDefs.h"
#include "ImageBuilderImpl.h"
#include "AppError.h"
#include "Option.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一のアプリケーション オブジェクトです。

CWinApp theApp;

using namespace std;

namespace {
	inline void _DispUsage()
	{
		printf(
			"ImageBuilder Ver.0.01\n"
			"\n"
			"[USAGE]\n"
			"ImageBuilder.exe -i [xml] -o [out]\n"
			"\n"
			" -i [xml] : 入力XMLファイル\n"
			" -o [out] : 出力ファイル名\n"
			"            指定がない場合は入力XMLファイルから自動的に設定\n"
		);
	}

	void _PrintString(IZ_PCSTR pszStr)
	{
		printf("%s\n", pszStr);
	}
}	// namespace

#ifdef VGOTO
#undef VGOTO
#endif

#define VGOTO(b)	if (!(b)) { ASSERT(FALSE); nRetCode = 1; goto __EXIT__; }

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// MFC を初期化して、エラーの場合は結果を印刷します。
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 必要に応じてエラー コードを変更してください。
		_tprintf(_T("致命的なエラー: MFC の初期化ができませんでした。\n"));
		nRetCode = 1;
	}
	else
	{
		// エラーストリング表示用関数セット
		izanagi::izanagi_tk::CException::SetPrintLogFunc(_PrintString);

		COption cOption;

		// ウインドウハンドル取得
		HWND hWnd = ::GetConsoleWindow();
		VGOTO(hWnd != NULL);

		// グラフィックスデバイス作成
		izanagi::izanagi_tk::CGraphicsDeviceLite* pDevice = IZ_NULL;
		pDevice = izanagi::izanagi_tk::CGraphicsDeviceLite::CreateGraphicsDeviceLight(hWnd);
		VGOTO(pDevice != NULL);

		// オプション解析
		BOOL result = cOption.Analysis(argc, argv);
		VGOTO(result);
		VGOTO(cOption.IsValid());

		try {
			xercesc::XMLPlatformUtils::Initialize();

			xercesc::SAX2XMLReader* parser = xercesc::XMLReaderFactory::createXMLReader();
			
			parser->setContentHandler(&CImageBuilder::GetInstance());

			parser->parse(cOption.in);

			result = CImageBuilder::GetInstance().BuildIMG(cOption.out);
			VGOTO(result);
		}
		catch (izanagi::izanagi_tk::CException* e) {
			e->PrintLog();
			VGOTO(FALSE);
		}
		catch (...) {
			printf("Failed\n");
			VGOTO(FALSE);
		}

__EXIT__:
		SAFE_RELEASE(pDevice);
	}

	return nRetCode;
}
