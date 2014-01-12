#include <unistd.h>
#include <string.h>
#include "system/SysUtil.h"

namespace izanagi
{
namespace sys
{
    // 実行プログラムの位置からカレントディレクトリを設定.
    IZ_BOOL CSysUtil::SetCurrentDirectoryFromExe()
    {
        static izChar buf[260];

        // 実行プログラムのフルパスを取得
        int result = readlink("/proc/self/exe", buf, sizeof(buf));
        VRETURN(result > 0);

        // ファイル名を取り除く
        char* p = ::strrchr(buf, '\\');
        VRETURN(p != NULL);

        // カレントディレクトリを設定
        result = ::chdir(p);
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
