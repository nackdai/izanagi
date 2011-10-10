#if !defined(__IZANAGI_TOOL_KIT_SYSTEM_ENV_VAR_HELPER_H__)
#define __IZANAGI_TOOL_KIT_SYSTEM_ENV_VAR_HELPER_H__

#include <windows.h>
#include <vector>
#include "izDefs.h"

namespace izanagi {
namespace izanagi_tk {
	class CEnvVarHelper {
	private:
		CEnvVarHelper();
		~CEnvVarHelper();

	public:
		// 環境変数が有効かどうか
		static IZ_BOOL IsValidEnvVar(IZ_PCSTR lpszName)
		{
			// TODO
			// このくらいの長さがあれば十分？
			std::vector<char> tmp(1024);

			char* pBuf = &tmp[0];
			DWORD size = ::GetEnvironmentVariable(lpszName, pBuf, sizeof(lpszName));
			return (size > 0);
		}

		// 環境変数の展開
		static IZ_BOOL ExpandEnvStrings(
			izChar pDst[],
			size_t nDstLen,
			IZ_PCSTR lpszStr)
		{
			IZ_BOOL ret = IZ_FALSE;

			if (strlen(lpszStr) > 0) {
				FILL_ZERO(pDst, nDstLen);
				DWORD size = ::ExpandEnvironmentStrings(lpszStr, pDst, static_cast<DWORD>(nDstLen));
				ret = (size > 0);
			}

			IZ_ASSERT(ret);
			return ret;
		}
	};
}	// namespace izanagi_tk
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_TOOL_KIT_SYSTEM_ENV_VAR_HELPER_H__)
