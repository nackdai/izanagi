using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace ArchiveConverter
{
    static unsafe class Exporter
    {
        /// <summary>
        /// 出力可能ファイル数
        /// </summary>
        static int exportableFileNum = 0;

        /// <summary>
        /// 最大ファイルサイズ
        /// </summary>
        static long maxFileSize = 0;

        static void Convert(
            Config config,
            Option option,
            ArchiveRoot root)
        {
            // ツールディレクトリをフルパスに変換
            var toolDir = option.ToolDir;
            if (!Path.IsPathRooted(option.ToolDir))
            {
                toolDir = Path.GetDirectoryName(option.Input);
                toolDir = Path.Combine(toolDir, option.ToolDir);
                toolDir = Path.GetFullPath(toolDir);
            }

            foreach (var item in root.Items)
            {
                if (!File.Exists(item.Source))
                {
                    throw new FileNotFoundException();
                }

                try
                {
                    var cmdItem = config[item.Type];

                    // 一時ファイル名作成
                    var dir = Path.GetTempFileName();
                    var file = Path.GetTempFileName();
                    item.Dest = Path.Combine(dir, file);

                    // ソースデータをフルパスに変換
                    var src = item.Source;
                    if (!Path.IsPathRooted(item.Source))
                    {
                        var baseDir = Path.GetDirectoryName(option.Input);
                        src = Path.Combine(baseDir, item.Source);
                        src = Path.GetFullPath(src);
                    }

                    if (cmdItem == null)
                    {
                        // そのままコピー
                        File.Copy(src, item.Dest);
                    }
                    else
                    {
                        cmdItem.DoCmd(
                            toolDir,
                            src,
                            item.Dest,
                            item.Option);
                    }

                    exportableFileNum++;

                    // 最大ファイルサイズ取得
                    var fi = new FileInfo(item.Dest);
                    maxFileSize = (maxFileSize > fi.Length ? maxFileSize : fi.Length);

                    item.Size = (int)fi.Length;
                }
                catch (Exception e)
                {
                    // TODO

                    // 失敗したので、出力先は空にする
                    item.Dest = "";
                }
            }
        }

        static void BeginExport(string output)
        {
            char* tmp = (char*)System.Runtime.InteropServices.Marshal.StringToHGlobalAnsi(output);

            izanagi.tool.ArchiveLibProxy.BeginExport(
                (sbyte*)tmp,
                exportableFileNum,
                (int)maxFileSize);
        }

        static void EndExport()
        {
            izanagi.tool.ArchiveLibProxy.EndExport();
        }

        static void Register(string name, string path)
        {
            char* nameTmp = (char*)System.Runtime.InteropServices.Marshal.StringToHGlobalAnsi(name);
            char* pathTmp = (char*)System.Runtime.InteropServices.Marshal.StringToHGlobalAnsi(path);

            izanagi.tool.ArchiveLibProxy.Register((sbyte*)nameTmp, (sbyte*)pathTmp);
        }

        static public void Export(
            Config config,
            Option option,
            ArchiveRoot root)
        {
            Convert(config, option, root);

            BeginExport(option.Output);

            foreach (var item in root.Items)
            {
                // 出力先が空の場合は失敗している

                if (!string.IsNullOrEmpty(item.Dest))
                {
                    Register(item.Name, item.Dest);
                }
            }

            EndExport();

            foreach (var item in root.Items)
            {
                if (File.Exists(item.Dest))
                {
                    File.Delete(item.Dest);
                }
            }
        }
    }
}
