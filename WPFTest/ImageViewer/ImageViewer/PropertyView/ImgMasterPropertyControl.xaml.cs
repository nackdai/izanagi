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
    /// ImgMasterPropertyControl.xaml の相互作用ロジック
    /// </summary>
    public partial class ImgMasterPropertyControl : UserControl
    {
        public ImgMasterPropertyControl()
        {
            InitializeComponent();

            this.DataContext = new ImgMasterPropertyViewModel();
        }
    }
}
