using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ArchiveConverter
{
    static class Exporter
    {
        static public void Export(Config config, Option option, ArchiveRoot root)
        {
            foreach (var item in root.Items)
            {
                var cmdItem = config[item.Type];

                try
                {
                    cmdItem.DoCmd(
                        "", // TODO
                        item.Source,
                        "", // TODO
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
