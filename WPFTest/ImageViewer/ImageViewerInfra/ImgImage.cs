using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ImageViewerInfra
{
    public class ImgImage : IImgObject
    {
        private ImgTexture parent = null;
        private IntPtr imageBody = IntPtr.Zero;

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
                return (int)ImageLibDllProxy.GetImageWidth(imageBody);
            }
        }

        public int Height
        {
            get
            {
                return (int)ImageLibDllProxy.GetImageHeight(imageBody);
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
    }
}
