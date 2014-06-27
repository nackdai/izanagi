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
        static izChar path[260];

        // 実行プログラムのフルパスを取得
        IZ_INT pathSize = GetExecuteFilePath(path, sizeof(path));
        VRETURN(pathSize > 0);

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

    IZ_INT CSysUtil::GetExecuteFilePath(char* path, size_t pathBufSize)
    {
        // 実行プログラムのフルパスを取得
        int result = readlink("/proc/self/exe", path, pathBufSize);
        VRETURN_VAL(result > 0, 0);

        return result;
    }
}   // namespace sys
}   // namespace izanagi
