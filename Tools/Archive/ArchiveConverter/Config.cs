using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;
using System.IO;
using System.Diagnostics;

namespace ArchiveConverter
{
    [XmlRoot("config")]
    public class Config : List<Command>
    {
        public Config()
        {
        }

        public void SetBaseDir(string dir)
        {
        }

        public Command this[string type]
        {
            set
            {
                // Nothing...
            }
            get
            {
                var ret = this.Find((item) =>
                {
                    if (item.Type == type)
                    {
                        return true;
                    }
                    return false;
                });

#if false
                if (ret == null)
                {
                    throw new InvalidOperationException(string.Format("There is not config item({0}).", type));
                }
#endif
                return ret;
            }
        }

        static public void Serialize(Config config, string path)
        {
            using (var fs = new FileStream(path, FileMode.OpenOrCreate, FileAccess.Write))
            {
                var serilizer = new XmlSerializer(typeof(Config));

                serilizer.Serialize(fs, config);
            }
        }

        static public Config Deserialize(string path)
        {
            if (string.IsNullOrEmpty(path))
            {
                return null;
            }

            using (var fs = new FileStream(path, FileMode.Open, FileAccess.Read))
            {
                var serializer = new XmlSerializer(typeof(Config));

                var config = (Config)serializer.Deserialize(fs);
                return config;
            }
        }

        
    }

    public class Command
    {
        [XmlAttribute("type")]
        public string Type
        {
            get;
            set;
        }

        /// <summary>
        /// 実行ファイルパス
        /// </summary>
        [XmlAttribute("exe")]
        public string Exe
        {
            get;
            set;
        }

        /// <summary>
        /// 入力ファイル設定のためのオプション
        /// </summary>
        [XmlAttribute("in")]
        public string InputOption
        {
            get;
            set;
        }

        /// <summary>
        /// 出力ファイル設定のためのオプション
        /// </summary>
        [XmlAttribute("out")]
        public string OutputOption
        {
            get;
            set;
        }

        /// <summary>
        /// 実行ファイルに設定するオプション
        /// </summary>
        [XmlAttribute("opt")]
        public string Option
        {
            get;
            set;
        }

        void ReceiveStandardOutput(object sender, DataReceivedEventArgs args)
        {
            Console.WriteLine(args.Data);
        }

        void ReceiveStandardError(object sender, DataReceivedEventArgs args)
        {
            Console.WriteLine(args.Data);
        }

        /// <summary>
        /// コマンド実行
        /// </summary>
        /// <param name="dir">実行ファイルのあるディレクトリ</param>
        /// <param name="input">実行ファイルに渡す入力ファイル</param>
        /// <param name="output">実行ファイルに出力されるファイル</param>
        /// <param name="option">実行ファイルに設定するオプション</param>
        public void DoCmd(
            string dir, 
            string input,
            string output,
            string option)
        {
            // 入出力ファイルが設定されていないといけない
            if (string.IsNullOrEmpty(input)
                || string.IsNullOrEmpty(output))
            {
                // TODO
                throw new InvalidOperationException("");
            }

            var exe = this.Exe;
            if (!string.IsNullOrEmpty(dir))
            {
                exe = dir + "/" + exe;
            }

            if (!File.Exists(exe))
            {
                // 実行ファイルが存在しない
                throw new FileNotFoundException("{0} is not found.", exe);
            }

            exe = Path.GetFullPath(exe);

            var proc = new Process();
            proc.StartInfo.CreateNoWindow = false;
            proc.StartInfo.RedirectStandardError = false;
            proc.StartInfo.RedirectStandardOutput = false;
            proc.StartInfo.UseShellExecute = false;

            //proc.OutputDataReceived += new DataReceivedEventHandler(ReceiveStandardOutput);
            //proc.ErrorDataReceived += new DataReceivedEventHandler(ReceiveStandardError);

            proc.StartInfo.Arguments = this.InputOption + " " + input;
            proc.StartInfo.Arguments += " " + this.OutputOption + " " + output;

            if (!string.IsNullOrEmpty(this.Option))
            {
                proc.StartInfo.Arguments += " " + this.Option;
            }
            if (!string.IsNullOrEmpty(option))
            {
                proc.StartInfo.Arguments += " " + option;
            }

            proc.StartInfo.FileName = exe;

            proc.Start();

            //proc.BeginOutputReadLine();
            //proc.BeginErrorReadLine();

            //var stderr = proc.StandardError.ReadToEnd();
            //var stdout = proc.StandardOutput.ReadToEnd();

            proc.WaitForExit();
        }
    }
}
