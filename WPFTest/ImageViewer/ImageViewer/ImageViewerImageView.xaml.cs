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
using ImageViewerInfra;

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

            TreeViewSelectedItemChangedCommand.Command.OnSelectedItemChanged += OnSelectedItemChanged;
        }

        private void OnSelectedItemChanged(IImgObject selectedObj)
        {
            Rectangle blueRectangle = new Rectangle();
            blueRectangle.Height = 200;
            blueRectangle.Width = 400;

            Canvas.SetLeft(blueRectangle, 0);
            Canvas.SetTop(blueRectangle, 0);

            // Create a blue and a black Brush
            SolidColorBrush blueBrush = new SolidColorBrush();
            blueBrush.Color = Colors.Red;
            SolidColorBrush blackBrush = new SolidColorBrush();
            blackBrush.Color = Colors.Black;

            // Set Rectangle's width and color
            blueRectangle.StrokeThickness = 4;
            blueRectangle.Stroke = blackBrush;
            // Fill rectangle with blue color
            blueRectangle.Fill = blueBrush;

//            if (ImageCanvas.Width < blueRectangle.Width)
            {
                ImageCanvas.Width = blueRectangle.Width;
            }

            ImageCanvas.Children.Add(blueRectangle);
        }

        private void OnSizeChanged(object sender, SizeChangedEventArgs e)
        {
            Scroller.Width = this.Width;
            Scroller.Height = this.Height;
        }
    }
}
