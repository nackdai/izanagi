#if !defined(__IZANAGI_TOOL_KIT_SYSTEM_FILE_UTILITY_H__)
#define __IZANAGI_TOOL_KIT_SYSTEM_FILE_UTILITY_H__

#include <windows.h>
#include <shlwapi.h>
#include <stdio.h>
#include "izDefs.h"

namespace izanagi {
namespace izanagi_tk {
	class CFileUtility {
	private:
		CFileUtility();
		~CFileUtility();

	public:
		/**
		* Returns whether specified file is exist.
		*/
		static IZ_BOOL IsExist(IZ_PCSTR pszName)
		{
#if 0
			HANDLE hFind;
			WIN32_FIND_DATA fd;

			// ファイル検索
			hFind = FindFirstFile(pszName, &fd);

			return (hFind != INVALID_HANDLE_VALUE);
#else
			return ::PathFileExists(pszName);
#endif
		}

		/**
		* Returns whether specified file is read-only.
		*/
		static IZ_BOOL IsReadOnly(IZ_PCSTR pszName)
		{
			HANDLE hFind;
			WIN32_FIND_DATA fd;

			hFind = ::FindFirstFile(pszName, &fd);

			if(hFind != INVALID_HANDLE_VALUE) {
				if (fd.dwFileAttributes & FILE_ATTRIBUTE_READONLY) {
					return IZ_TRUE;
				}
			}

			return IZ_FALSE;
		}

		/**
		* Combine path.
		*/
		static IZ_BOOL CombinePath(
			izChar* pDst,
			size_t nSize,
			IZ_PCSTR path_0,
			IZ_PCSTR path_1)
		{
			IZ_ASSERT((pDst != path_0) && (pDst != path_1));

			FILL_ZERO(pDst, nSize);

			IZ_BOOL ret = (::PathCombine(pDst, path_0, path_1) != IZ_NULL);
			IZ_ASSERT(ret);

			return ret;
		}

		/**
		* Create directory.
		*/
		static IZ_BOOL CreateDir(IZ_PCSTR pszName)
		{
			IZ_BOOL ret = IZ_TRUE;
			
			if (!IsExist(pszName)) {
				ret = (::CreateDirectory(pszName, IZ_NULL) != 0);
				IZ_ASSERT(ret);
			}

			return ret;
		}

		/**
		* Returns file name from path.
		*/
		static IZ_PCSTR GetFileNameFromPath(IZ_PCSTR pszPath)
		{
			// Get file name from path.
			IZ_PCSTR ret = ::PathFindFileName(pszPath);
			return ret;
		}

		/**
		* Get path without file name.
		*/
		static IZ_BOOL GetPathWithoutFileName(
			izChar* pDst,
			size_t nSize,
			IZ_PCSTR pszPath)
		{
			IZ_ASSERT(pDst != pszPath);

			IZ_BOOL ret = (::sprintf_s(pDst, nSize, "%s\0", pszPath) >= 0);
			IZ_ASSERT(ret);

			if (ret) {
				::PathRemoveFileSpec(pDst);
			}

			return ret;
		}

		/**
		* Get file name from path without extension.
		*/
		static IZ_BOOL GetFileNameFromPathWithoutExt(
			izChar* pDst,
			size_t nSize,
			IZ_PCSTR pszPath)
		{
			IZ_ASSERT(pDst != pszPath);

			IZ_PCSTR tmp = GetFileNameFromPath(pszPath);

			VRETURN(IsEnableCopyToBuf(pDst, nSize, tmp));
			VRETURN(CopyToBuf(tmp, pDst, nSize));

			::PathRemoveExtension(pDst);

			return IZ_TRUE;
		}

		/**
		* Get path without extension.
		*/
		static IZ_BOOL RemoveExtension(
			izChar* pDst,
			size_t nSize,
			IZ_PCSTR pszPath)
		{
			IZ_ASSERT(pDst != pszPath);

			VRETURN(IsEnableCopyToBuf(pDst, nSize, pszPath));
			VRETURN(CopyToBuf(pszPath, pDst, nSize));

			// Get path without extension.
			::PathRemoveExtension(pDst);

			return IZ_TRUE;
		}

		/**
		* Get extension.
		*/
		static IZ_PCSTR GetExtension(
			izChar* pDst,
			size_t nSize,
			IZ_PCSTR pszPath)
		{
			IZ_ASSERT(pDst != pszPath);

			VRETURN_NULL(IsEnableCopyToBuf(pDst, nSize, pszPath));
			VRETURN_NULL(CopyToBuf(pszPath, pDst, nSize));

			// Get extension.
			izChar* ret = ::PathFindExtension(pDst);
			if ((ret != NULL)
				&& (ret[0] == '.'))
			{
				ret = ret++;
			}

			return ret;
		}

		/**
		* Get execute module name.
		*/
		static IZ_BOOL GetExeModuleName(
			izChar* pDst,
			size_t nSize)
		{
			FILL_ZERO(pDst, nSize);

			IZ_BOOL ret = (::GetModuleFileName(
							IZ_NULL,
							pDst,
							static_cast<DWORD>(nSize)) != 0);
			IZ_ASSERT(ret);
			return ret;
		}

		/**
		* Create temporary file name by time.
		*/
		static void CreateTmpFileName(std::string& refName)
		{
			SYSTEMTIME stTime;
			::GetLocalTime(&stTime);

			static izChar tmp[8];

			refName += IntegerToString(stTime.wYear, tmp, sizeof(tmp));
			refName += IntegerToString(stTime.wMonth, tmp, sizeof(tmp));
			refName += IntegerToString(stTime.wDay, tmp, sizeof(tmp));
			refName += IntegerToString(stTime.wHour, tmp, sizeof(tmp));
			refName += IntegerToString(stTime.wMinute, tmp, sizeof(tmp));
			refName += IntegerToString(stTime.wSecond, tmp, sizeof(tmp));
			refName += IntegerToString(stTime.wMilliseconds, tmp, sizeof(tmp));
		}

		/**
		* Remove file.
		*/
		static IZ_BOOL RemoveFile(IZ_PCSTR pszStr)
		{
			return ::DeleteFile(pszStr);
		}

		/**
		* Return whether path is relative path.
		*/
		static IZ_BOOL IsRelativePath(IZ_PCSTR pszPath)
		{
			return ::PathIsRelative(pszPath);
		}

	private:
		static IZ_BOOL IsEnableCopyToBuf(
			IZ_PCSTR pszStr0,
			size_t nSize,
			IZ_PCSTR pszStr1)
		{
			// Compare buffer size and size of input string.
			size_t nStrSize = strlen(pszStr1);
			IZ_BOOL b0 = (nSize >= nStrSize);

			IZ_BOOL b1 = (pszStr0 != pszStr1);

			return (b0 && b1);
		}

		static IZ_BOOL CopyToBuf(IZ_PCSTR pszSrc, izChar* pDst, size_t nSize)
		{
			IZ_BOOL ret = IZ_FALSE;

			if (IsEnableCopyToBuf(pDst, nSize, pszSrc)) {
				ret = (::sprintf_s(pDst, nSize, "%s\0", pszSrc) >= 0);
			}

			return ret;
		}

		static IZ_PCSTR IntegerToString(IZ_INT i, izChar* buf, size_t nBufSize)
		{
			::sprintf_s(buf, nBufSize, "%d", i);
			return buf;
		}
	};
}	// izanagi_tk
}	// izanagi

#endif	// #if !defined(__IZANAGI_TOOL_KIT_SYSTEM_FILE_UTILITY_H__)
