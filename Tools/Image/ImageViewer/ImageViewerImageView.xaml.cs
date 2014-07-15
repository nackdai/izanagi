using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.ComponentModel;

namespace ImageViewer
{
    /// <summary>
    /// ImageViewerImageView.xaml の相互作用ロジック
    /// </summary>
    public partial class ImageViewerImageView : UserControl
    {
        double orgImgWidth;
        double orgImgHeight;

        public ImageViewerImageView()
        {
            InitializeComponent();

            this.DataContext = new ImageDetailViewModel();

            ((ImageDetailViewModel)this.DataContext).PropertyChanged += OnPropertyChanged;

            TreeViewSelectedItemChangedCommand.Command.OnSelectedItemChanged += OnSelectedItemChanged;
        }

        private void OnPropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            var vm = this.DataContext as ImageDetailViewModel;

            if (e.PropertyName.Equals("ScaleX"))
            {
                ImgCanvas.Width = orgImgWidth * vm.ScaleX;
            }
            else if (e.PropertyName.Equals("ScaleY"))
            {
                ImgCanvas.Height = orgImgHeight * vm.ScaleY;
            }
        }

        private void OnSelectedItemChanged(IImgObject selectedObj)
        {
            var img = selectedObj as ImgImage;

            if (img != null)
            {
                var imgCtrl = new Image();
                imgCtrl.BeginInit();
                {
                    imgCtrl.Source = img.GetImageSource();
                    imgCtrl.HorizontalAlignment = HorizontalAlignment.Left;
                    imgCtrl.VerticalAlignment = VerticalAlignment.Top;
                }
                imgCtrl.EndInit();

                ImgCanvas.Children.Clear();

                ImgCanvas.Children.Add(imgCtrl);

                ImgCanvas.Width = img.Width;
                this.orgImgWidth = img.Width;

                ImgCanvas.Height = img.Height;
                this.orgImgHeight = img.Height;

                (this.DataContext as ImageDetailViewModel).TargetImage = img;
            }
        }

        private void Scroller_MouseMove(object sender, MouseEventArgs e)
        {
            var vm = this.DataContext as ImageDetailViewModel;
            var position = e.GetPosition(this.Scroller);

            // マウスカーソルの位置を教える
            vm.SetPosition(position.X, position.Y);
        }
    }
}
