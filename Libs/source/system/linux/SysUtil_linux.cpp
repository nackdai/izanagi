#include <unistd.h>
#include <string.h>
#include "system/SysUtil.h"
#include "std/StdUtil.h"

namespace izanagi
{
namespace sys
{
    // 実行プログラムの位置からカレントディレクトリを設定.
    IZ_BOOL CSysUtil::SetCurrentDirectoryFromExe()
    {
        // 実行プログラムのフルパスを取得
        const char* path = GetExecuteFilePath();
        VRETURN(path != IZ_NULL);

        char* tmp = const_cast<char*>(path);

        // ファイル名を取り除く
        char* p = ::strrchr(tmp, '/');
        VRETURN(p != IZ_NULL);

        IZ_UINT64 diff = CStdUtil::GetPtrDistance(tmp, p);

        tmp[diff] = 0;
        p = tmp;

        // カレントディレクトリを設定
        int result = ::chdir(p);
        VRETURN(result >= 0);

        return IZ_TRUE;
    }

    const char* CSysUtil::GetExecuteFilePath()
    {
        static izChar buf[260];

        // 実行プログラムのフルパスを取得
        int result = readlink("/proc/self/exe", buf, sizeof(buf));
        VRETURN_NULL(result > 0);

        return buf;
    }
}   // namespace sys
}   // namespace izanagi
