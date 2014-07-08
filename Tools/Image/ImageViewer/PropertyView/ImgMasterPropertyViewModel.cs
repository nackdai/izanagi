using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace ImageViewer
{
    /// <summary>
    /// イメージマスタのプロパティビューのビューモデル
    /// </summary>
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

        /// <summary>
        /// プロパティ表示対象のイメージマスタ
        /// </summary>
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

                    // 画像タイプを更新する
                    NotifyPropertyChanged("ImageType");
                }
            }
        }

        /// <summary>
        /// イメージマスタの画像タイプ
        /// </summary>
        public string ImageType
        {
            get
            {
                if (this.ImgMaster != null)
                {
                    // 拡張子を返す
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
