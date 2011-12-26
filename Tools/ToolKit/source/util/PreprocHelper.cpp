#include "util/PreprocHelper.h"

// Entrypoint for preprocessor.
extern "C"
int preproc_main(int argc, char* argv[]);

using namespace izanagi::tool;

IZ_INT CPreprocHelper::CallPreproc(int argc, char* argv[])
{
	int ret = preproc_main(argc, argv);
	return ret;
}
