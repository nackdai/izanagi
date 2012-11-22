// CompressTest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "CompressTest.h"

#include "Compress/RangeCoderEncoder.h"
#include "Compress/RangeCoderDecoder.h"

#include "Compress/SlideDictionaryEncoder.h"
#include "Compress/SlideDictionaryDecoder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一のアプリケーション オブジェクトです。

CWinApp theApp;

using namespace std;

namespace {
	void RangeCoderTest()
	{
		static BYTE bufAlloc[1024 * 1024];

		izanagi::CMemoryAllocator cAllocator;
		cAllocator.Init(sizeof(bufAlloc), bufAlloc);

		static const char data[] = { 'A', 'B', 'A', 'C', 'A', 'D' };
		static BYTE enc_buf[1024];
		static BYTE dec_buf[1024];

		izanagi::CRangeCoderEncoder::SDesc sRCEncDesc;

		{	
			FILL_ZERO(enc_buf, sizeof(enc_buf));

			izanagi::CMemoryInputStream cInput;
			cInput.Init(data, sizeof(data));

			izanagi::izanagi_tk::CMemoryOutputStream cOutput;
			cOutput.Init(enc_buf, sizeof(enc_buf));

			izanagi::CRangeCoderEncoder cRCEncoder;
			cRCEncoder.Init(
				&cAllocator,
				16);

			cRCEncoder.Encode(
				&sRCEncDesc,
				&cInput,
				&cOutput);
		}

		{
			FILL_ZERO(dec_buf, sizeof(dec_buf));

			izanagi::CMemoryInputStream cInput;
			cInput.Init(enc_buf, sRCEncDesc.compressed_size);

			izanagi::izanagi_tk::CMemoryOutputStream cOutput;
			cOutput.Init(dec_buf, sizeof(dec_buf));

			izanagi::CRangeCoderDecoder cRCDecoder;
			izanagi::CRangeCoderDecoder::SDesc sRCDecDesc;
			cRCDecoder.Decode(
				&sRCDecDesc,
				&cInput,
				sRCEncDesc.org_size,
				&cOutput);
		}

		ASSERT(memcmp(data, dec_buf, sizeof(data)) == 0);
	}

	void SlideDicTest()
	{
		static BYTE bufAlloc[1024 * 1024];

		izanagi::CMemoryAllocator cAllocator;
		cAllocator.Init(sizeof(bufAlloc), bufAlloc);

		static const char data[] = { 'A', 'B', 'A', 'C', 'A', 'D', 'A', 'B', 'A' };
		static BYTE enc_buf[1024];
		static BYTE dec_buf[1024];

		izanagi::CSlideDictionaryEncoder::SDesc sDesc;

		{
			izanagi::CMemoryInputStream cInput;
			cInput.Init(data, sizeof(data));

			izanagi::izanagi_tk::CMemoryOutputStream cOutput;
			cOutput.Init(enc_buf, sizeof(enc_buf));

			izanagi::CSlideDictionaryEncoder cEnc;

			cEnc.Init(
				&cAllocator,
				1024 * 8,
				256);

			cEnc.Encode(
				&sDesc,
				&cInput,
				&cOutput);
		}

		{
			izanagi::CMemoryInputStream cInput;
			cInput.Init(enc_buf, sDesc.compressed_size);

			izanagi::izanagi_tk::CMemoryOutputStream cOutput;
			cOutput.Init(dec_buf, sizeof(dec_buf));

			izanagi::CSlideDictionaryDecoder cDec;

			cDec.Init(
				&cAllocator,
				1024 * 8);

			cDec.Decode(
				&cInput,
				&cOutput);
		}

		ASSERT(memcmp(data, dec_buf, sizeof(data)) == 0);
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
		RangeCoderTest();
		SlideDicTest();
	}

	return nRetCode;
}
