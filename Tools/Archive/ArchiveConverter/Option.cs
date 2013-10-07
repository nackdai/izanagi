using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace ArchiveConverter
{
    class Option
    {
        /// <summary>
        /// コンフィグファイル.
        /// </summary>
        public string Config
        {
            get;
            private set;
        }

        /// <summary>
        /// 入力ファイル.
        /// </summary>
        public string Input
        {
            get;
            private set;
        }

        /// <summary>
        /// 出力ファイル.
        /// </summary>
        public string Output
        {
            get;
            private set;
        }

        /// <summary>
        /// ベースディレクトリ.
        /// </summary>
        public string BaseDir
        {
            get;
            private set;
        }

        /// <summary>
        /// ツールディレクトリ.
        /// ファイルコンバータなどのツールのあるディレクトリ.
        /// </summary>
        public string ToolDir
        {
            get;
            private set;
        }

        public Option(string[] args)
        {
            for (int i = 0; i < args.Length; i++)
            {
                var arg = args[i];

                if (arg == "-c")
                {
                    // config file.
                    this.Config = args[i + 1];
                }
                else if (arg == "-i")
                {
                    // input file.
                    this.Input = args[i + 1];
                }
                else if (arg == "-o")
                {
                    // output file.
                    this.Output = args[i + 1];
                }
                else if (arg == "-d")
                {
                    // object dir.
                    this.BaseDir = args[i + 1];
                }
                else if (arg == "-td")
                {
                    // tool dir.
                    this.ToolDir = args[i + 1];
                }
            }
        }

        public void CheckValidation()
        {
            if (string.IsNullOrEmpty(this.Input))
            {
                throw new InvalidOperationException("No input file.");
            }

            if (string.IsNullOrEmpty(this.Output))
            {
                var dir = Path.GetDirectoryName(this.Input);
                var file = Path.GetFileNameWithoutExtension(this.Input);

                this.Output = dir + "/" + file + ".arc";
            }
        }
    }
}
