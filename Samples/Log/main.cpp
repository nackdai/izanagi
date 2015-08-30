#include "izSampleKit.h"

void log()
{
    IZ_LOGD_WITH_TAG("Test", "%s(%d)", "Hoge", 0);
    IZ_LOGE_WITH_TAG("Test", "%s(%d)", "Hoge", 1);
    IZ_LOGI_WITH_TAG("Test", "%s(%d)", "Hoge", 2);
    IZ_LOGV_WITH_TAG("Test", "%s(%d)", "Hoge", 3);
    IZ_LOGW_WITH_TAG("Test", "%s(%d)", "Hoge", 4);
}

IzMain(0, 0)
{
    IZ_PRINTF("=== Log Sample\n");
    log();
    IZ_PRINTF("\n");

    IZ_PRINTF("=== Disable Debug\n");
    izanagi::CLog::disableFlag(izanagi::LogType::Debug);
    log();
    IZ_PRINTF("\n");

    IZ_PRINTF("=== Without tag\n");
    izanagi::CLog::enableFlag(izanagi::LogType::Debug);
    {
        IZ_LOGD("%s(%d)", "Hoge", 0);
        IZ_LOGE("%s(%d)", "Hoge", 1);
        IZ_LOGI("%s(%d)", "Hoge", 2);
        IZ_LOGV("%s(%d)", "Hoge", 3);
        IZ_LOGW("%s(%d)", "Hoge", 4);
    }
    IZ_PRINTF("\n");
    
	return 0;
}
