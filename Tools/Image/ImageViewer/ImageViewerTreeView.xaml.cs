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
    /// ImageViewerTreeView.xaml の相互作用ロジック
    /// </summary>
    public partial class ImageViewerTreeView : UserControl
    {
        public ImageViewerTreeView()
        {
            InitializeComponent();

            // ツリービュー準備
            treeView1.ItemsSource = TreeViewRoot.Instance;
        }

        /// <summary>
        /// ツリービューで選択されているアイテムが変更されたときに呼ばれる
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OnSelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            // ツリービューで選択されているアイテムが変更されたときのコマンドを発火
            if (TreeViewSelectedItemChangedCommand.Command.CanExecute(e))
            {
                TreeViewSelectedItemChangedCommand.Command.Execute(e);
            }
        }
    }
}
