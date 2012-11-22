#if !defined(__IZANAGI_TOOL_KIT_UTIL_PREPROC_HELPER_H__)
#define __IZANAGI_TOOL_KIT_UTIL_PREPROC_HELPER_H__

#include <vector>
#include "izDefs.h"

namespace izanagi {
namespace tool {

	class CPreprocHelper {
	private:
		CPreprocHelper();
		~CPreprocHelper();

		static IZ_INT CallPreproc(int argc, char* argv[]);

	public:
		/**
		* プリプロセス処理を行う
		*/
		template <typename _T>
		static IZ_INT Preproc(
			IZ_PCSTR pszSrc,
			IZ_PCSTR pszDst,
			const std::vector<_T>& tInclude,
			const std::vector<_T>& tDefine)
		{
			// プリプロセッサに渡す引数を作成
			std::vector<char*> tvArgs;

			// 最初はダミー
			tvArgs.push_back("");

			// インクルードパス
			{
				std::vector<_T>::const_iterator it = tInclude.begin();
				while (it != tInclude.end()) {
					IZ_PCSTR str = *(it++);
					tvArgs.push_back("-I");
					tvArgs.push_back(const_cast<char*>(str));
				}
			}

			// プリプロセス定義
			{
				std::vector<_T>::const_iterator it = tDefine.begin();
				while (it != tDefine.end()) {
					IZ_PCSTR str = *(it++);
					tvArgs.push_back("-D");
					tvArgs.push_back(const_cast<char*>(str));
				}
			}

			// コメントは保持する
			//tvArgs.push_back("-C");

			// プリプロセッサ出力をファイルに書き込むようにする
			//tvArgs.push_back("-P");

			// 入力ファイル
			tvArgs.push_back(const_cast<char*>(pszSrc));

			// 出力ファイル
			tvArgs.push_back(const_cast<char*>(pszDst));

			IZ_INT ret = CallPreproc((int)tvArgs.size(), &tvArgs[0]);

			if (ret > 0) {
				// TODO
				IZ_ASSERT(IZ_FALSE);
			}

			return ret;
		}
	};
}	// tool
}	// izanagi

#endif	// #if !defined(__IZANAGI_TOOL_KIT_UTIL_PREPROC_HELPER_H__)
