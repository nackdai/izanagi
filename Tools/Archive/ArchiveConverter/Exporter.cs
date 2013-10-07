using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace ArchiveConverter
{
    static class Exporter
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

                    if (cmdItem == null)
                    {
                        // そのままコピー
                        File.Copy(item.Source, item.Dest);
                    }
                    else
                    {
                        cmdItem.DoCmd(
                            "", // TODO
                            item.Source,
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

        static public void Export(
            Config config,
            Option option,
            ArchiveRoot root)
        {
            Convert(config, option, root);

            foreach (var item in root.Items)
            {
                // 出力先が空の場合は失敗している

                if (!string.IsNullOrEmpty(item.Dest))
                {

                }
            }
        }
    }
}
