using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using System.IO;
using System.Windows.Media.Imaging;

namespace ImageViewer
{
    /// <summary>
    /// 埋め込みリソースのプロキシ
    /// </summary>
    static class EmbeddedResourceProxy
    {
        /// <summary>
        /// リソースのストリームを取得
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        static public Stream GetResourceStream(string name)
        {
            Assembly asm = Assembly.GetExecutingAssembly();
            Stream ret = asm.GetManifestResourceStream(name);

            return ret;
        }

        /// <summary>
        /// 埋め込みリソースからBitmapImageを作成
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        static public BitmapImage CreateBitmapImageFromResource(string name)
        {
            BitmapImage ret = new BitmapImage();
            ret.BeginInit();
            {
                ret.CacheOption = BitmapCacheOption.OnLoad;
                ret.StreamSource = GetResourceStream(name);
            }
            ret.EndInit();

            return ret;
        }
    }
}
