using System;
using System.Windows;
using System.Windows.Interop;
using System.Collections.ObjectModel;

namespace ImageViewer
{
    /// <summary>
    /// MainWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class MainWindow : Window
    {
        /// <summary>
        /// ImageLib初期済みフラグ
        /// </summary>
        private bool isInitlizedImageLib = false;

        public MainWindow()
        {
            InitializeComponent();

            TreeViewSelectedItemChangedCommand.Command.OnSelectedItemChanged += OnSelectedItemChanged;
        }

        /// <summary>
        /// メニュー「開く」
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        public void OnOpen(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();

            dlg.DefaultExt = "*.png";
            dlg.Filter = "PNG|*.png|JPEG|*.jpg|BMP|*.bmp|HDR|*.hdr|ianaggi IMG|*.img|XML|*.xml";

            if (dlg.ShowDialog() == true)
            {
                // ファイルを開く
                ImgMaster imgMaster = ImgMaster.CreateImgMaster(dlg.FileName);
                if (imgMaster != null)
                {
                    // 
                    ImgObjectManager.Instance.Register(imgMaster);
                    TreeViewRoot.Instance.Register(imgMaster);
                }
            }
        }

        private new void Close()
        {
            if (isInitlizedImageLib)
            {
                // ImageLibを終了
                ImageLibDllProxy.ReleaseImageLib();

                isInitlizedImageLib = false;
            }

            base.Close();
        }

        /// <summary>
        /// メニュー「閉じる」
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        public void OnClose(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            // ウインドウハンドル取得
            HwndSource source = HwndSource.FromVisual(this) as HwndSource;
            IntPtr hWnd = source.Handle;

            // ImageLibを初期化
            isInitlizedImageLib = ImageLibDllProxy.InitImageLib(hWnd);
        }

        private void Winsow_Closed(object sender, EventArgs e)
        {
            Close();
        }

        /// <summary>
        /// ツリービューで選択アイテムが変更されたときに呼ばれる
        /// </summary>
        /// <param name="selectedObj">選択アイテム</param>
        private void OnSelectedItemChanged(IImgObject selectedObj)
        {
            // プロパティペイン内の要素を一度すべて見えないようにする
            for (int i = 0; i< PropertyPane.Children.Count; i++)
            {
                PropertyPane.Children[i].Visibility = Visibility.Collapsed;
            }

            // 選択されたアイテムがイメージマスタ（イメージデータのルート）のとき
            if (selectedObj is ImgMaster)
            {
                // イメージマスタのプロパティビューにイメージマスタ教える
                ImgMasterPropertyView.SetImageMaster(selectedObj as ImgMaster);

                // イメージマスタのプロパティビューを表示する
                ImgMasterPropertyView.Visibility = Visibility.Visible;
            }
            else
            {
                // ダミー用の塗りつぶし矩形を表示する
                DummyRect.Visibility = Visibility.Visible;
            }
        }
    }
}
