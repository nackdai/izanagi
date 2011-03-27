// Compressor.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "Compressor.h"

#include "Option.h"
#include "izToolKit.h"
#include "izCompress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一のアプリケーション オブジェクトです。

CWinApp theApp;

using namespace std;

namespace {
	inline void _PrintUsage()
	{
		printf(
			"-e"
			"-d"
			"-i <in>"
			"-o <out>"
			"-threshold <val>"
			"-blocksize <val>"
		);
	}

	IZ_BOOL _Encode(const SOption& sOption)
	{
		// Open source file.
		izanagi::CFileInputStream cIn;
		THROW_EXCEPTION(
			cIn.Open(sOption.fileIn),
			izanagi::izanagi_tk::CStdLog,
			"Can't Open [%s]\n",
			sOption.fileIn);

		// Open destination file.
		izanagi::CFileOutputStream cOut;
		THROW_EXCEPTION(
			cOut.Open(sOption.fileOut),
			izanagi::izanagi_tk::CStdLog,
			"Can't Open [%s]\n",
			sOption.fileOut);

		// Simple memory allocator.
		izanagi::izanagi_tk::CSimpleMemoryAllocator cAllocator;

		// Encode.
		IZ_BOOL ret = izanagi::CCompressor::GetInstance().Encode(
						&cIn,
						&cOut,
						&cAllocator,
						sOption.blocksize,
						sOption.threshold);

		cIn.Finalize();
		cOut.Finalize();

		THROW_EXCEPTION(
			ret,
			izanagi::izanagi_tk::CStdLog,
			"Failed Encode\n",
			sOption.fileOut);

		return ret;
	}
}	// namespace

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
		try {
			COption cOption;
			if (!cOption.Analysis(argc, argv)) {
				nRetCode = 1;
				goto __EXIT__;
			}

			if (cOption.isEncode) {
				if (!_Encode(cOption)) {
					nRetCode = 1;
					goto __EXIT__;
				}

				IZ_FLOAT fCmpRatio = izanagi::CCompressor::GetInstance().GetCompressRatio();
			}
			else {
				// TODO
			}
		}
		catch (izanagi::izanagi_tk::CException& e) {
			// TODO
			nRetCode = 1;
		}
		catch (CException* e) {
			// TODO
			e->Delete();
			nRetCode = 1;
		}
		catch (...) {
			nRetCode = 1;
		}
	}

__EXIT__:
	return nRetCode;
}
