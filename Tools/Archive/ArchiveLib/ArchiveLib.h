#if !defined(__IZ_TOOL_ARCHIVE_LIB__)
#define __IZ_TOOL_ARCHIVE_LIB__

void BeginExport(int fileNum, int maxFileSize);
void EndExport();

void ExportFile(char* name, int fileSize);

#endif  // #if !defined(__IZ_TOOL_ARCHIVE_LIB__)
