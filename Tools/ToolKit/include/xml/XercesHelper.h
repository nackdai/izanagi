#if !defined(__XERCES_HELPER_H__)
#define __XERCES_HELPER_H__

#include <string>
#include <xercesc/util/XMLString.hpp>

class UnicodeTranslator {
public:
	explicit UnicodeTranslator(const char* const str) : unicode_str(NULL)
	{
		if (xercesc::XMLPlatformUtils::fgMemoryManager) {
			unicode_str = xercesc::XMLString::transcode(str);
		}
	}
	explicit UnicodeTranslator(const std::string& str) : unicode_str(NULL)
	{
		if (xercesc::XMLPlatformUtils::fgMemoryManager) {
			unicode_str = xercesc::XMLString::transcode(str.c_str());
		}
	}

	~UnicodeTranslator()
	{
		xercesc::XMLString::release(&unicode_str);
	}

	const XMLCh* const GetString() const
	{
		C_ASSERT(sizeof (XMLCh) == sizeof (wchar_t));
		return unicode_str ? unicode_str : reinterpret_cast<const XMLCh*>(L"");
	}

private:
	XMLCh* unicode_str;
};

class NativecodeTranslator {
public:
	explicit NativecodeTranslator(const XMLCh* const str) : native_str(NULL)
	{
		if (xercesc::XMLPlatformUtils::fgMemoryManager) {
			native_str = xercesc::XMLString::transcode(str);
		}
	}

	~NativecodeTranslator()
	{
		xercesc::XMLString::release(&native_str);
	}

	const char* GetString() const
	{
		return native_str ? native_str : "";
	}

private:
	char* native_str;
};

#define XU(str)	UnicodeTranslator(str).GetString()
#define XN(str)	NativecodeTranslator(str).GetString()

#endif // #if !defined(__XERCES_HELPER_H__)
