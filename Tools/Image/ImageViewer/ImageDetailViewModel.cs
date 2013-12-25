using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace ImageViewer
{
    class ImageDetailViewModel : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        protected void NotifyPropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }
        }

        public ImgImage TargetImage
        {
            get;
            set;
        }

        private int posX;
        private int posY;

        public string Position
        {
            get
            {
                return string.Format("Position({0}, {1})", posX, posY);
            }
        }

        public void SetPosition(double x, double y)
        {
            if (this.TargetImage != null)
            {
                posX = (int)x;
                posY = (int)y;

                var width = this.TargetImage.Width;
                var height = this.TargetImage.Height;

                if (posX < width && posY < height)
                {
                    posX = (posX >= width ? width - 1 : posX);
                    posY = (posY >= height ? height - 1 : posY);

                    // Notify property is changed to binding target in XAML.
                    NotifyPropertyChanged("Position");
                }
            }
        }
    }
}
