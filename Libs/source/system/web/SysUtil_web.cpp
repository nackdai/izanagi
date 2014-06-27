#include "system/SysUtil.h"

namespace izanagi
{
namespace sys
{
    // 実行プログラムの位置からカレントディレクトリを設定.
    IZ_BOOL CSysUtil::SetCurrentDirectoryFromExe()
    {
        return IZ_TRUE;
    }

    IZ_INT CSysUtil::GetExecuteFilePath(char* path, size_t pathBufSize)
    {
        return 1;
    }
}   // namespace sys
}   // namespace izanagi
