using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace ImageViewer
{
    /// <summary>
    /// 画像表示ペインの下にある画像の詳細情報表示のためのビューモデル
    /// </summary>
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

        private int r;
        private int g;
        private int b;
        private int a;

        /// <summary>
        /// マウスカーソルの位置
        /// </summary>
        public string Position
        {
            get
            {
                return string.Format("Position({0}, {1})", this.posX, this.posY);
            }
        }

        /// <summary>
        /// カーソル位置のピクセル値
        /// </summary>
        public string Pixel
        {
            get
            {
                return string.Format("RGBA({0}, {1}, {2}, {3}",
                    this.r, this.g, this.b, this.a);
            }
        }

        private int scale = 100;

        /// <summary>
        /// 画像のスケール
        /// </summary>
        public int Scale
        {
            get
            {
                return scale;
            }
            set
            {
                scale = value;
                NotifyPropertyChanged("ScaleText");
                NotifyPropertyChanged("ScaleX");
                NotifyPropertyChanged("ScaleY");
            }
        }

        /// <summary>
        /// 画像のスケールを表示する文字列
        /// </summary>
        public string ScaleText
        {
            get
            {
                return string.Format("{0}%", scale);
            }
        }

        public double ScaleX
        {
            get
            {
                return this.Scale * 0.01;
            }
        }

        public double ScaleY
        {
            get
            {
                return this.Scale * 0.01;
            }
        }

        /// <summary>
        /// マウスカーソルの位置をセットする
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        public void SetPosition(double x, double y)
        {
            if (this.TargetImage != null)
            {
                this.posX = (int)x;
                this.posY = (int)y;

                var width = this.TargetImage.Width;
                var height = this.TargetImage.Height;

                if (this.posX < width && this.posY < height)
                {
                    this.posX = (posX >= width ? width - 1 : posX);
                    this.posY = (posY >= height ? height - 1 : posY);

                    // Notify property is changed to binding target in XAML.
                    this.NotifyPropertyChanged("Position");
                }
            }

            this.SetPixel(x, y);
        }

        private void SetPixel(double x, double y)
        {
            if (this.TargetImage != null)
            {
                IntRGBA? rgba = this.TargetImage.GetRGBAFromPosition((int)x, (int)y);

                if (rgba.HasValue)
                {
                    r = rgba.Value.r;
                    g = rgba.Value.g;
                    b = rgba.Value.b;
                    a = rgba.Value.a;

                    this.NotifyPropertyChanged("Pixel");
                }
            }
        }
    }
}
