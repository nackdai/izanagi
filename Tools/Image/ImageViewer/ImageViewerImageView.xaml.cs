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

namespace ImageViewer
{
    /// <summary>
    /// ImageViewerImageView.xaml の相互作用ロジック
    /// </summary>
    public partial class ImageViewerImageView : UserControl
    {
        public ImageViewerImageView()
        {
            InitializeComponent();

            this.DataContext = new ImageDetailViewModel();

            TreeViewSelectedItemChangedCommand.Command.OnSelectedItemChanged += OnSelectedItemChanged;
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

                this.Scroller.Content = imgCtrl;

                (this.DataContext as ImageDetailViewModel).TargetImage = img;
            }
        }

        private void Scroller_MouseMove(object sender, MouseEventArgs e)
        {
            var vm = this.DataContext as ImageDetailViewModel;
            var position = e.GetPosition(this.Scroller);
            vm.SetPosition(position.X, position.Y);
        }
    }
}
