using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace ImageViewer
{
    class ImgMasterPropertyViewModel : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        protected void NotifyPropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }
        }

        private ImgMaster imgMaster;

        public ImgMaster ImgMaster
        {
            private get
            {
                return this.imgMaster;
            }
            set
            {
                if (this.imgMaster != value)
                {
                    this.imgMaster = value;

                    NotifyPropertyChanged("ImageType");
                }
            }
        }

        public string ImageType
        {
            get
            {
                if (this.ImgMaster != null)
                {
                    return this.ImgMaster.Ext;
                }
                else
                {
                    return "";
                }
            }
        }
    }
}
