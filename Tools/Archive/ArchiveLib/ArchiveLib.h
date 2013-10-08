#if !defined(__IZ_TOOL_ARCHIVE_LIB__)
#define __IZ_TOOL_ARCHIVE_LIB__

void BeginExport(
    char* path,
    int fileNum, int maxFileSize);

void EndExport();

void Register(char* name, char* path);

#endif  // #if !defined(__IZ_TOOL_ARCHIVE_LIB__)
