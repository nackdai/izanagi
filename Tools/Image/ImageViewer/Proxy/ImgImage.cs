using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows;

namespace ImageViewer
{
    public class ImgImage : IImgObject
    {
        private ImgTexture parent = null;
        private IntPtr imageBody = IntPtr.Zero;
        private IntPtr pixelData = IntPtr.Zero;
        private int width = -1;
        private int height = -1;

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

        internal ImgImage(ImgTexture tex, IntPtr body)
        {
            parent = tex;
            imageBody = body;
            ID = ImgMaster.ImgObjID++;
        }

        internal void Dispose(bool byInternal)
        {
            if (!byInternal)
            {
                parent.RemoveImage(this);
            }
        }

        public void Dispose()
        {
            Dispose(false);
        }

        public ImageSource GetImageSource()
        {
            // TODO
            // 異なるフォーマットのピクセルデータを取得する場合は
            // 前のデータを解放する

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
