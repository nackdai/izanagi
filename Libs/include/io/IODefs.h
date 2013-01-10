#if !defined(__IZANAGI_IO_IO_DEFS_H__)
#define __IZANAGI_IO_IO_DEFS_H__

namespace izanagi
{
    enum E_IO_STREAM_SEEK_POS
    {
        E_IO_STREAM_SEEK_POS_START = 0,
        E_IO_STREAM_SEEK_POS_CUR,
        E_IO_STREAM_SEEK_POS_END,

        E_IO_STREAM_SEEK_POS_NUM,
        E_IO_STREAM_SEEK_POS_FORCE_INT32 = 0x7fffffff,
    };

    enum E_IO_FILE_MODE
    {
        E_IO_FILE_MODE_TEXT = 0,
        E_IO_FILE_MODE_BIN,

        E_IO_FILE_MODE_NUM,
        E_IO_FILE_MODE_FORCE_INT32 = 0x7fffffff,
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_IO_IO_DEFS_H__)
