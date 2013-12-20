using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows;

namespace ImageViewer
{
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
                return parent;
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
                if (width < 0)
                {
                    width = (int)ImageLibDllProxy.GetImageWidth(imageBody);
                }
                return width;
            }
        }

        /// <summary>
        /// イメージの高さ
        /// </summary>
        public int Height
        {
            get
            {
                if (height < 0)
                {
                    height = (int)ImageLibDllProxy.GetImageHeight(imageBody);
                }
                return height;
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
            parent = tex;
            imageBody = body;
            ID = ImgMaster.ImgObjID++;
        }

        protected override void Disposing()
        {
            parent.RemoveImage(this);
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
            pixelData = ImageLibDllProxy.GetPixelDataAsBGRA8(imageBody);

            if (pixelData != IntPtr.Zero)
            {
                int stride = Width * PixelFormats.Bgra32.BitsPerPixel / 8;
                int size = stride * Width;

                var ret = BitmapSource.Create(
                    Width, Height,
                    96, 96,
                    PixelFormats.Bgra32,
                    null,
                    pixelData,
                    size,
                    stride);

                return ret;
            }

            return null;
        }
    }
}
