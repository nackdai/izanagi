// ImageBuilder.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#ifndef _WIN32_WINNT        // Windows XP 以降のバージョンに固有の機能の使用を許可します。                   
#define _WIN32_WINNT 0x0501 // これを Windows の他のバージョン向けに適切な値に変更してください。
#endif                      

#include "ImageLibDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef VGOTO
#undef VGOTO
#endif

#define VGOTO(b)    if (!(b)) { IZ_ASSERT(FALSE); nRetCode = 1; goto __EXIT__; }

int main(int argc, char* argv[])
{
    int nRetCode = 0;

    HWND hWnd = ::GetConsoleWindow();
    VGOTO(hWnd != IZ_NULL);

    // イメージライブラリ初期化
    IZ_BOOL result = izInitImageLib(hWnd);
    void* master = izReadImageMaster(argv[1]);
    void* tex = izGetTextureInMaster(master, 0);
    void* img = izGetImageInTexture(tex, 0, 0);
    void* data = izGetPixelDataAsRGBA8(img);

__EXIT__:
    return nRetCode;
}
