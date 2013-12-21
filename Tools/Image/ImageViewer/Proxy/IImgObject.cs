using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ImageViewer
{
    public interface IImgObject
    {
        IImgObject Parent
        {
            get;
        }

        int ID
        {
            get;
        }
    }
}
