#include "std/ErrorUtil.h"

namespace izanagi {
namespace izanagi_tk {
	void (*CException::s_PrintLogFunc)(IZ_PCSTR) = IZ_NULL;
}	// namespace izanagi_tk
}	// namespace izanagi
