#if !defined(__IZANAGI_TOOL_KIT_UTIL_STRING_UTILITY_H__)
#define __IZANAGI_TOOL_KIT_UTIL_STRING_UTILITY_H__

#include "izDefs.h"

namespace izanagi {
namespace izanagi_tk {
	/**
	*/
	class CStringUtil {
	private:
		CStringUtil();
		~CStringUtil();

	public:
		static IZ_BOOL CmpStr(IZ_PCSTR str0, IZ_PCSTR str1)
		{
			size_t len0 = strlen(str0);
			size_t len1 = strlen(str1);

			if (len0 != len1) {
				return IZ_FALSE;
			}

			return (memcmp(str0, str1, len0) == 0);
		}

		static void Format(
			std::string& dst,
			IZ_PCSTR format, ...)
		{
			static std::vector<izChar> tmp(1000);
        
			va_list args;
			va_start(args, format);

			va_list src = args;
	        
			while (IZ_TRUE) {
				args = src;
	            
				if(IZ_VSPRINTF(&tmp[0], tmp.size(), format, args) == -1) {
					tmp.resize(tmp.size() * 2);
				}
				else{
					break;
				}
			}

			va_end(args);

			tmp.push_back('\0');
			dst = &tmp[0];
		}
	};

}	// namespace izanagi_tk
}	// namespace izanagi


#endif	// #if !defined(__IZANAGI_TOOL_KIT_UTIL_STRING_UTILITY_H__)
