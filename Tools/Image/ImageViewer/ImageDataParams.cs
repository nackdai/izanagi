using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ImageViewer
{
    enum PixelFormat : int
    {
        RGBA8 = 0,
    }

    class ImageDataParams
    {
        /// <summary>
        /// ピクセル幅
        /// </summary>
        public int Width { get; set; }

        /// <summary>
        /// ピクセル高さ
        /// </summary>
        public int Height { get; set; }

        /// <summary>
        /// ピクセルフォーマット
        /// </summary>
        public PixelFormat Format { get; set; }

        /// <summary>
        /// ピクセルフォーマット文字列
        /// </summary>
        public string FormatName
        {
            get
            {
                return Format.ToString();
            }
            set
            {
                // 文字列からenumに変換
                Format = (PixelFormat)Enum.Parse(typeof(PixelFormat), value);
            }
        }

        /// <summary>
        /// ピクセルフォーマット名
        /// Enumから取得
        /// </summary>
        static private string[] PixelFormatNames = Enum.GetNames(typeof(PixelFormat));

        protected ImageDataParams()
        {
        }
    }
}
