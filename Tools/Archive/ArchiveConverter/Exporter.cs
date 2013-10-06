using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace ArchiveConverter
{
    static class Exporter
    {
        static public void Export(
            Config config, 
            Option option, 
            ArchiveRoot root)
        {
            foreach (var item in root.Items)
            {
                var cmdItem = config[item.Type];

                try
                {
                    var dir = Path.GetTempFileName();
                    var file = Path.GetTempFileName();
                    item.Dest = Path.Combine(dir, file);

                    cmdItem.DoCmd(
                        "", // TODO
                        item.Source,
                        item.Dest,
                        item.Option);
                }
                catch (Exception e)
                {
                    // TODO
                }
            }


        }
    }
}
