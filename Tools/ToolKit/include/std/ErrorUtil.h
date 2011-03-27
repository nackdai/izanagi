#if !defined(__IZANAGI_TOOL_KIT_STD_ERROR_UTIL_H__)
#define __IZANAGI_TOOL_KIT_STD_ERROR_UTIL_H__

#include <list>
#include "izDefs.h"

namespace izanagi {
namespace izanagi_tk {
	class CException;

	/**
	* ログ
	*/
	class ILog {
		friend class CException;

	protected:
		ILog() {}
		virtual ~ILog() {}

		NO_COPIABLE(ILog);

	public:
		virtual IZ_PCSTR GetString() const = 0;
	};

	/**
	*/
	template <IZ_UINT STR_NUM>
	class TStdLog : public ILog {
	public:
		TStdLog(IZ_PCSTR format, ...)
		{
			va_list	argp;
			va_start(argp, format);
			vsprintf_s(m_Buf, STR_NUM, format, argp);
			va_end(argp);
		}

	protected:
		TStdLog() {}
		~TStdLog() {}

	public:
		IZ_PCSTR GetString() const { return m_Buf; }

	protected:
		IZ_CHAR m_Buf[STR_NUM];
	};

	/**
	*/
	typedef TStdLog<1024>	CStdLog;

	/**
	* 例外
	*/
	class CException {
	private:
		static void (*s_PrintLogFunc)(IZ_PCSTR);

	public:
		static void SetPrintLogFunc(void (*func)(IZ_PCSTR))
		{
			s_PrintLogFunc = func;
		}

	public:
		CException(ILog* pLog)
		{
			m_Log = pLog;
		}
		~CException()
		{
			SAFE_DELETE(m_Log);
		}

	private:
		CException() {}

		//NO_COPIABLE(CException);

	public:
		void PrintLog()
		{
			if ((s_PrintLogFunc != IZ_NULL) && (m_Log != IZ_NULL)) {
				IZ_PCSTR str = m_Log->GetString();
				if (str != IZ_NULL) {
					(*s_PrintLogFunc)(str);
				}
			}
		}

	private:
		ILog* m_Log;
	};
}	// namespace izanagi_tk
}	// namespace izanagi

#define THROW_EXCEPTION(b, log_type, format, ...)\
	do {\
		if (!(b)) {\
			throw izanagi::izanagi_tk::CException(new log_type(format, __FILE__, __LINE__, __VA_ARGS__));\
		}\
	} while(IZ_FALSE)

#endif	// #if !defined(__IZANAGI_TOOL_KIT_STD_ERROR_UTIL_H__)
