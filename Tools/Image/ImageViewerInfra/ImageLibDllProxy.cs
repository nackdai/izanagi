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
        /// <summary>
        /// ImageLibを利用するための初期化
        /// </summary>
        /// <param name="hWnd">ウインドウハンドル</param>
        static public bool InitImageLib(IntPtr hWnd)
        {
            bool ret = izanagi.tool.ImageLibProxy.InitImageLib(hWnd);
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
            izanagi.tool.ImageLibProxy.ReleaseImageLib();
        }

        /// <summary>
        /// ファイル読み込み
        /// </summary>
        /// <param name="path">読み込むファイルパス</param>
        /// <returns>イメージファイルハンドル</returns>
        static public IntPtr Read(string path)
        {
            char* p = (char*)System.Runtime.InteropServices.Marshal.StringToHGlobalAnsi(path);
            IntPtr ret = izanagi.tool.ImageLibProxy.ReadImageMaster((sbyte*)p);
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
            izanagi.tool.ImageLibProxy.ReleaseImageMaster(p);
        }

        /// <summary>
        /// マスタデータ内のテクスチャ数取得
        /// </summary>
        /// <param name="p">イメージファイルハンドル</param>
        /// <returns>テクスチャ数</returns>
        static public uint GetTexNumInMaster(IntPtr p)
        {
            uint ret = izanagi.tool.ImageLibProxy.GetTexNumInMaster(p);
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
            IntPtr ret = izanagi.tool.ImageLibProxy.GetTextureInMaster(p, texIdx);
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
            uint ret = izanagi.tool.ImageLibProxy.GetImageNumInTexture(p);
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
            uint ret = izanagi.tool.ImageLibProxy.GetMipMapNumInTexture(p);
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
            IntPtr ret = izanagi.tool.ImageLibProxy.GetImageInTexture(p, imgIdx, level);
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
            uint ret = izanagi.tool.ImageLibProxy.GetImageWidth(p);
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
            uint ret = izanagi.tool.ImageLibProxy.GetImageHeight(p);
            if (ret == 0)
            {
                throw new Exception();
            }
            return ret;
        }

        /// <summary>
        /// BGRA8としてピクセルデータを取得.
        /// </summary>
        /// <param name="p">イメージハンドル</param>
        /// <returns>ピクセルデータ</returns>
        static public IntPtr GetPixelDataAsBGRA8(IntPtr p)
        {
            IntPtr ret = izanagi.tool.ImageLibProxy.GetPixelDataAsBGRA8(p);
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
            izanagi.tool.ImageLibProxy.ReleasePixelData(p);
        }
    }
}
