#include "std/ErrorUtil.h"

namespace izanagi {
namespace tool {
	void (*CException::s_PrintLogFunc)(IZ_PCSTR) = IZ_NULL;
}	// namespace tool
}	// namespace izanagi
