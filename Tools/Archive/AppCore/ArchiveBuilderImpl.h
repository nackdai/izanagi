#if !defined(__ARCHIVE_BUILDER_IMPL_H__)
#define __ARCHIVE_BUILDER_IMPL_H__

#include "FileDefs.h"

struct SOption;

class CArchiveBuilderImpl {
private:
	CArchiveBuilderImpl();
	~CArchiveBuilderImpl();

public:
	// 出力
	static BOOL Export(const SOption& sOption);

	// 中間データ出力
	static BOOL ExportObj(const SOption& sOption);
};

#endif	// #if !defined(__ARCHIVE_BUILDER_IMPL_H__)
