using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace ImageViewerInfra
{
    /// <summary>
    /// ImageLibDll.dllのproxy
    /// </summary>
    static public unsafe class ImageLibDllProxy
    {
        [DllImport("ImageLibDll.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern bool izInitImageLib(IntPtr hWnd);

        [DllImport("ImageLibDll.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void izReleaseImageLib();

        [DllImport("ImageLibDll.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        static extern IntPtr izReadImageMaster(char* path);

        [DllImport("ImageLibDll.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void izReleaseImageMaster(IntPtr p);

        [DllImport("ImageLibDll.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern uint izGetTexNumInMaster(IntPtr p);

        [DllImport("ImageLibDll.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern IntPtr izGetTextureInMaster(IntPtr p, uint texIdx);

        [DllImport("ImageLibDll.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern uint izGetImageNumInTexture(IntPtr p);

        [DllImport("ImageLibDll.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern uint izGetMipMapNumInTexture(IntPtr p);

        [DllImport("ImageLibDll.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern IntPtr izGetImageInTexture(IntPtr p, uint imgIdx, uint level);

        [DllImport("ImageLibDll.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern uint izGetImageWidth(IntPtr p);

        [DllImport("ImageLibDll.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern uint izGetImageHeight(IntPtr p);

        [DllImport("ImageLibDll.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern IntPtr izGetPixelDataAsRGBA8(IntPtr p);

        [DllImport("ImageLibDll.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void izReleasePixelData(IntPtr p);


        /// <summary>
        /// ImageLibを利用するための初期化
        /// </summary>
        /// <param name="hWnd">ウインドウハンドル</param>
        static public bool InitImageLib(IntPtr hWnd)
        {
            bool ret = izInitImageLib(hWnd);
            if (!ret)
            {
                // TODO
                throw new Exception();
            }
            return ret;
        }

        /// <summary>
        /// ImageLibの利用終了処理
        /// </summary>
        static public void ReleaseImageLib()
        {
            izReleaseImageLib();
        }

        /// <summary>
        /// ファイル読み込み
        /// </summary>
        /// <param name="path">読み込むファイルパス</param>
        /// <returns>イメージファイルハンドル</returns>
        static public IntPtr Read(string path)
        {
            char* p = (char*)System.Runtime.InteropServices.Marshal.StringToHGlobalAnsi(path);
            IntPtr ret = izReadImageMaster(p);
            if (ret == IntPtr.Zero)
            {
                throw new Exception();
            }
            return ret;
        }

        /// <summary>
        /// イメージファイルの解放
        /// </summary>
        /// <param name="p">イメージファイルハンドル</param>
        static public void Release(IntPtr p)
        {
            izReleaseImageMaster(p);
        }

        /// <summary>
        /// マスタデータ内のテクスチャ数取得
        /// </summary>
        /// <param name="p">イメージファイルハンドル</param>
        /// <returns>テクスチャ数</returns>
        static public uint GetTexNumInMaster(IntPtr p)
        {
            uint ret = izGetTexNumInMaster(p);
            if (ret == 0)
            {
                throw new Exception();
            }
            return ret;
        }

        /// <summary>
        /// テクスチャハンドル取得.
        /// </summary>
        /// <param name="p">イメージファイルハンドル</param>
        /// <param name="texIdx">マスタデータ内のテクスチャインデックス</param>
        /// <returns>テクスチャハンドル</returns>
        static public IntPtr GetTextureInMaster(IntPtr p, uint texIdx)
        {
            IntPtr ret = izGetTextureInMaster(p, texIdx);
            if (ret == IntPtr.Zero)
            {
                throw new Exception();
            }
            return ret;
        }

        /// <summary>
        /// テクスチャデータ内のイメージ数取得
        /// </summary>
        /// <param name="p">テクスチャハンドル</param>
        /// <returns>テクスチャデータ内のイメージ数</returns>
        static public uint GetImageNumInTexture(IntPtr p)
        {
            uint ret = izGetImageNumInTexture(p);
            if (ret == 0)
            {
                throw new Exception();
            }
            return ret;
        }

        /// <summary>
        /// テクスチャデータ内のミップマップ数取得
        /// </summary>
        /// <param name="p">テクスチャハンドル</param>
        /// <returns>テクスチャデータ内のミップマップ数</returns>
        static public uint GetMipMapNumInTexture(IntPtr p)
        {
            uint ret = izGetMipMapNumInTexture(p);
            if (ret == 0)
            {
                throw new Exception();
            }
            return ret;
        }

        /// <summary>
        /// テクスチャデータ内のイメージハンドルを取得
        /// </summary>
        /// <param name="p">テクスチャハンドル</param>
        /// <param name="imgIdx">テクスチャデータ内のイメージインデックス</param>
        /// <param name="level">イメージデータのミップマップレベル</param>
        /// <returns>イメージハンドル</returns>
        static public IntPtr GetImageInTexture(IntPtr p, uint imgIdx, uint level)
        {
            IntPtr ret = izGetImageInTexture(p, imgIdx, level);
            if (ret == IntPtr.Zero)
            {
                throw new Exception();
            }
            return ret;
        }

        /// <summary>
        /// イメージ幅を取得
        /// </summary>
        /// <param name="p">イメージハンドル</param>
        /// <returns>イメージ幅</returns>
        static public uint GetImageWidth(IntPtr p)
        {
            uint ret = izGetImageWidth(p);
            if (ret == 0)
            {
                throw new Exception();
            }
            return ret;
        }

        /// <summary>
        /// イメージ高さを取得
        /// </summary>
        /// <param name="p">イメージハンドル</param>
        /// <returns>イメージ高さ</returns>
        static public uint GetImageHeight(IntPtr p)
        {
            uint ret = izGetImageHeight(p);
            if (ret == 0)
            {
                throw new Exception();
            }
            return ret;
        }

        /// <summary>
        /// RGBA8としてピクセルデータを取得.
        /// </summary>
        /// <param name="p">イメージハンドル</param>
        /// <returns>ピクセルデータ</returns>
        static public IntPtr GetPixelDataAsRGBA8(IntPtr p)
        {
            IntPtr ret = izGetPixelDataAsRGBA8(p);
            if (ret == IntPtr.Zero)
            {
                throw new Exception();
            }
            return ret;
        }

        /// <summary>
        /// ピクセルデータを解放.
        /// </summary>
        /// <param name="p">イメージハンドル</param>
        static public void ReleasePixelData(IntPtr p)
        {
            izReleasePixelData(p);
        }
    }
}
