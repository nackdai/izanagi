#include <windows.h>
#include <shlwapi.h>
#include "system/SysUtil.h"

#pragma comment(lib, "shlwapi.lib")

using namespace izanagi;

// 実行プログラムの位置からカレントディレクトリを設定.
IZ_BOOL CSysUtil::SetCurrentDirectoryFromExe()
{
	static izChar buf[_MAX_PATH];

	// 実行プログラムのフルパスを取得
	{
		DWORD result = ::GetModuleFileName(
						IZ_NULL,
						buf,
						sizeof(buf));
		VRETURN(result > 0);
	}

	// ファイル名を取り除く
	BOOL result = ::PathRemoveFileSpec(buf);
	VRETURN(result);

	// カレントディレクトリを設定
	result = ::SetCurrentDirectory(buf);
	VRETURN(result);

	return result;
}
