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

        private bool IsExportHeader;

        public string HeaderFile
        {
            get;
            private set;
        }

        public Option(string[] args)
        {
            for (int i = 0; i < args.Length; i++)
            {
                var arg = args[i];

                bool result = false;

                if (i < args.Length - 1)
                {
                    if (result = (arg == "-c"))
                    {
                        // config file.
                        this.Config = args[i + 1];
                    }
                    else if (result = (arg == "-i"))
                    {
                        // input file.
                        this.Input = args[i + 1];
                    }
                    else if (result = (arg == "-o"))
                    {
                        // output file.
                        this.Output = args[i + 1];
                    }
                    else if (result = (arg == "-d"))
                    {
                        // object dir.
                        this.BaseDir = args[i + 1];
                    }
                    else if (result = (arg == "-td"))
                    {
                        // tool dir.
                        this.ToolDir = args[i + 1];
                    }
                }

                if (!result)
                {
                    if (result = (arg == "-h"))
                    {
                        IsExportHeader = true;
                    }
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

                this.Output = file + ".arc";

                if (!string.IsNullOrEmpty(dir))
                {
                    this.Output = dir + "/" + this.Output;
                }
            }

            if (this.IsExportHeader)
            {
                var dir = Path.GetDirectoryName(this.Output);
                var file = Path.GetFileNameWithoutExtension(this.Output);

                this.HeaderFile = file + ".h";

                if (!string.IsNullOrEmpty(dir))
                {
                    this.HeaderFile = dir + "/" + this.HeaderFile;
                }
            }
        }
    }
}
