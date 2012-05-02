using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ImageViewerInfra;

namespace ImageViewer
{
    static class ImageViewerProxy
    {
        static public event Action<IImgObject> OnSelectedItemChanged;

        static public void NorifySelectedItemChanged(IImgObject selectedObj)
        {
            if (OnSelectedItemChanged != null)
            {
                OnSelectedItemChanged(selectedObj);
            }
        }
    }
}
