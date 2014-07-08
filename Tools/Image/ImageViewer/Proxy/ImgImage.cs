using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows;

namespace ImageViewer
{
    public struct IntRGBA
    {
        int r;
        int g;
        int b;
        int a;
    }

    /// <summary>
    /// イメージデータ
    /// </summary>
    public class ImgImage : DisposableObject, IImgObject
    {
        private ImgTexture parent = null;
        private IntPtr imageBody = IntPtr.Zero;
        private IntPtr pixelData = IntPtr.Zero;
        private int width = -1;
        private int height = -1;

        /// <summary>
        /// 親のイメージオブジェクト
        /// 親はテクスチャデータ
        /// </summary>
        public IImgObject Parent
        {
            get
            {
                return this.parent;
            }
        }

        public int ID
        {
            get;
            private set;
        }

        /// <summary>
        /// イメージの幅
        /// </summary>
        public int Width
        {
            get
            {
                if (this.width < 0)
                {
                    this.width = (int)ImageLibDllProxy.GetImageWidth(imageBody);
                }
                return this.width;
            }
        }

        /// <summary>
        /// イメージの高さ
        /// </summary>
        public int Height
        {
            get
            {
                if (this.height < 0)
                {
                    this.height = (int)ImageLibDllProxy.GetImageHeight(imageBody);
                }
                return this.height;
            }
        }

        private ImgImage()
        {
        }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="tex">親のテクスチャデータ</param>
        /// <param name="body">イメージデータ</param>
        internal ImgImage(ImgTexture tex, IntPtr body)
        {
            this.parent = tex;
            this.imageBody = body;
            this.ID = ImgMaster.ImgObjID++;
        }

        protected override void Disposing()
        {
            // 親の管理から自分を抜く
            this.parent.RemoveImage(this);
        }

        /// <summary>
        /// 表示用のイメージソースを取得
        /// </summary>
        /// <returns>イメージソース</returns>
        public ImageSource GetImageSource()
        {
            // TODO
            // 異なるフォーマットのピクセルデータを取得する場合は
            // 前のデータを解放する

            // Get data as BGRA8.
            this.pixelData = ImageLibDllProxy.GetPixelDataAsBGRA8(imageBody);

            if (this.pixelData != IntPtr.Zero)
            {
                int stride = Width * PixelFormats.Bgra32.BitsPerPixel / 8;
                int size = stride * Width;

                var ret = BitmapSource.Create(
                    this.Width, this.Height,
                    96, 96,
                    PixelFormats.Bgra32,
                    null,
                    this.pixelData,
                    size,
                    stride);

                return ret;
            }

            return null;
        }

        /// <summary>
        /// 指定された座標のRGBA値を返す
        /// </summary>
        /// <param name="x">X座標</param>
        /// <param name="y">Y座標</param>
        /// <returns>指定された座標のRGBA値を返す。ピクセルデータが存在しない場合はnullを返す</returns>
        public IntRGBA? GetRGBAFromPosition(int x, int y)
        {
            if (this.pixelData != IntPtr.Zero)
            {
            }

            return null;
        }
    }
}
