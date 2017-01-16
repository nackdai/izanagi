// ImageBuilder.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#ifndef _WIN32_WINNT        // Windows XP 以降のバージョンに固有の機能の使用を許可します。                   
#define _WIN32_WINNT 0x0501 // これを Windows の他のバージョン向けに適切な値に変更してください。
#endif                      

#include "PointCloudeConverterCore.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int main(int argc, char* argv[])
{
    if (argc < 3) {
        return 1;
    }

    std::string inFile(argv[1]);
    std::string outFile(argv[2]);

    auto result = convert(inFile, outFile);

    return result ? 0 : 1;
}
