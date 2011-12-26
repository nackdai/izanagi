#include "util/StringUtility.h"

using namespace izanagi;
using namespace tool;

IZ_BOOL CString::CmpStr(const char* str0, const char* str1)
{
	size_t len0 = strlen(str0);
	size_t len1 = strlen(str1);

	if (len0 != len1) {
		return IZ_FALSE;
	}

	return (memcmp(str0, str1, len0) == 0);
}

void CString::format(const char* format, ...)
{
	std::vector<izChar> tmp(1000);
        
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
	*(std::string*)this = &tmp[0];
}

void CString::replace(char chOld, char chNew)
{
	char replaceChar[2] = { chNew, 0 };

	for (;;) {
		size_type idx = find_first_of(chOld);
		if (idx == std::string::npos) {
			break;
		}

		std::string::replace(idx, 1, replaceChar);
	}
}

const CString& CString::make_lower()
{
	std::vector<char> tmp;

	const char* str = c_str();

	for (IZ_UINT i = 0; i < length(); i++) {
		IZ_UINT ch = tolower(str[i]);
		tmp.push_back(ch);
	}
	tmp.push_back('\0');

	*this = &tmp[0];

	return *this;
}

const CString& CString::make_upper()
{
	std::vector<char> tmp;

	const char* str = c_str();

	for (IZ_UINT i = 0; i < length(); i++) {
		IZ_UINT ch = toupper(str[i]);
		tmp.push_back(ch);
	}
	tmp.push_back('\0');

	*this = &tmp[0];

	return *this;
}

int CString::reverse_find(char ch)
{
	for (;;) {
		size_type idx = rfind(ch, length());
		if (idx == std::string::npos) {
			return -1;
		}

		return idx;
	}
}

int CString::compare_no_case(const char* str)
{
	CString tmp0(*this);
	CString tmp1(str);

	tmp0.make_lower();
	tmp1.make_lower();

	int ret = strncmp(tmp0, tmp1, max(tmp0.length(), tmp1.length()));
	return ret;
}

void CString::clear()
{
	char tmp[1] = {0};

	*this = &tmp[0];
}

CString::operator const char*()
{
	return c_str();
}

CString::operator const char*() const
{
	return c_str();
}

CString::operator const void*() const
{
	return c_str();
}

const CString& CString::operator=(const char* rhs)
{
	*(std::string*)this = rhs;
	return *this;
}

bool CString::operator==(char rhs)
{
	char tmp[2] = { rhs, 0 };

	return (*this == &tmp[0]);
}
