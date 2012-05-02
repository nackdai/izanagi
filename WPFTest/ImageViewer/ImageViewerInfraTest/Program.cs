using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using ImageViewerInfra;

namespace ImageViewerInfraTest
{
    class Program
    {
        [DllImport("kernel32.dll")]
        static extern IntPtr GetConsoleWindow();

        static IntPtr Test0(string path)
        {
            // イメージ読み込み
            IntPtr master = ImageLibDllProxy.Read(path);

            // テクスチャ数取得
            uint texNum = ImageLibDllProxy.GetTexNumInMaster(master);
            Console.WriteLine("TexNum = {0}", texNum);

            // テクスチャハンドル取得
            IntPtr tex = ImageLibDllProxy.GetTextureInMaster(master, 0);
            Console.WriteLine("Texture is {0}", tex != IntPtr.Zero ? "Exist" : "None");

            // イメージ数取得
            uint imgNum = ImageLibDllProxy.GetImageNumInTexture(tex);
            Console.WriteLine("ImageNum = {0}", imgNum);

            // イメージハンドル取得
            IntPtr img = ImageLibDllProxy.GetImageInTexture(tex, 0, 0);
            Console.WriteLine("Image is {0}", img != IntPtr.Zero ? "Exist" : "None");

            // イメージ幅・高さ取得
            uint width = ImageLibDllProxy.GetImageWidth(img);
            uint height = ImageLibDllProxy.GetImageHeight(img);
            Console.WriteLine("Width = {0} / Height = {1}", width, height);

            return master;
        }

        static IntPtr Test1(string path)
        {
            // イメージ読み込み
            IntPtr master = ImageLibDllProxy.Read(path);

            ImgMaster imgMaster = ImgMaster.CreateImgMaster("Test", path);

            // テクスチャ数取得
            int texNum = imgMaster.TextureNum;
            Console.WriteLine("TexNum = {0}", texNum);

            // テクスチャハンドル取得
            ImgTexture tex = imgMaster.GetTexture(0);
            Console.WriteLine("Texture is {0}", tex != null ? "Exist" : "None");

            // イメージ数取得
            int imgNum = tex.ImageNum;
            Console.WriteLine("ImageNum = {0}", imgNum);

            // イメージハンドル取得
            ImgImage img = tex.GetImage(0, 0);
            Console.WriteLine("Image is {0}", img != null ? "Exist" : "None");

            // イメージ幅・高さ取得
            int width = img.Width;
            int height = img.Height;
            Console.WriteLine("Width = {0} / Height = {1}", width, height);

            return master;
        }

        static void Main(string[] args)
        {
            // TODO
            //string path = args[0];
            string path = "a.bmp";

            IntPtr master = IntPtr.Zero;

            try
            {
                // コンソールウインドウのハンドルを取得
                IntPtr hWnd = GetConsoleWindow();

                // イメージライブラリ初期化
                ImageLibDllProxy.InitImageLib(hWnd);

                //master = Test0(path);
                master = Test1(path);
            }
            catch (Exception)
            {
            }
            finally
            {
                if (master != IntPtr.Zero)
                {
                    ImageLibDllProxy.Release(master);
                }
                ImageLibDllProxy.ReleaseImageLib();
            }
        }
    }
}
