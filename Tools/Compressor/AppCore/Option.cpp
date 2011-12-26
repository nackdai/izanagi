#include <stdafx.h>
#include "Option.h"
#include "izCompress.h"

COption::COption()
{
	blocksize = izanagi::IZ_CMP_MAX_BLOCK_SIZE;
	threshold = 0.95f;

	isEncode = IZ_TRUE;
}

BOOL COption::Analysis(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++) {
		BOOL result = FALSE;
		CString cmd(argv[i]);

		if (i < argc - 1) {
			if (result = (cmd == _T("-i"))) {
				// -i
				fileIn.Format(_T("%s"), argv[++i]);
			}
			else if (result = (cmd == _T("-o"))) {
				// -o
				fileOut.Format(_T("%s"), argv[++i]);
			}
			else if (result = (cmd == _T("-threshold"))) {
				// -threshold
				threshold = static_cast<IZ_FLOAT>(atof(argv[++i]));
			}
			else if (result = (cmd == _T("-blocksize"))) {
				// -blocksize
				blocksize = atoi(argv[++i]);
			}
		}

		if (!result) {
			// 引数無しオプション
			if (result = (cmd == _T("-e"))) {
				isEncode = IZ_TRUE;
			}
			else if (result = (cmd == _T("-d"))) {
				isEncode = IZ_FALSE;
			}
		}

		if (!result) {
			// TODO
			printf("無効なオプションです[%s]\n\n", cmd);
			//ASSERT(FALSE);
			return FALSE;
		}
	}

	// TODO

	return TRUE;
}
