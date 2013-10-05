using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace ArchiveConverter
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                var option = new Option(args);
                option.CheckValidation();

                var config = Config.Deserialize(option.Config);

                ArchiveRoot.BasePath = Path.GetDirectoryName(option.Input);
                var root = ArchiveRoot.Deserialize(option.Input);
            }
            catch (Exception e)
            {
            }
        }
    }

    
}
