#if !defined(__IZANAGI_SYSTEM_SYS_UTIL_H__)
#define __IZANAGI_SYSTEM_SYS_UTIL_H__

#include "izDefs.h"

namespace izanagi
{
namespace sys
{
    /**
    */
    class CSysUtil {
    private:
        CSysUtil();
        ~CSysUtil();

        NO_COPIABLE(CSysUtil);

    public:
        /** 実行プログラムの位置からカレントディレクトリを設定.
         */
        static IZ_BOOL SetCurrentDirectoryFromExe();
    };
}   // namespace sys
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SYSTEM_SYS_UTIL_H__)
