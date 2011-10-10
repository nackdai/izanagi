#if !defined(__IZANAGI_TOOL_KIT_UTIL_STRING_UTILITY_H__)
#define __IZANAGI_TOOL_KIT_UTIL_STRING_UTILITY_H__

#include <string>
#include <vector>
#include "izDefs.h"

namespace izanagi {
namespace izanagi_tk {
	/**
	 */
	class CString : public std::string {
	public:
		CString() {}
		~CString() {}

		CString(const char* str) : std::string(str) {}

	public:
		static IZ_BOOL CmpStr(const char* str0, const char* str1);

		void format(const char* format, ...);

		void replace(char chOld, char chNew);

		const CString& make_lower();
		const CString& make_upper();

		void clear();

		int reverse_find(char ch);

		int compare_no_case(const char* str);

		operator const char*();
		operator const char*() const;

		operator const void*() const;

		const CString& operator=(const char* rhs);

		bool operator==(char rhs);
	};
}	// namespace izanagi_tk
}	// namespace izanagi


#endif	// #if !defined(__IZANAGI_TOOL_KIT_UTIL_STRING_UTILITY_H__)
