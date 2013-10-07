using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace ArchiveConverter
{
    class Program
    {
        static void DisplayUsage()
        {
            Console.WriteLine("Usage : ArchiveConverter <options>");
            Console.WriteLine("");
            Console.WriteLine("options ****");
            Console.WriteLine(" -i <input>");
            Console.WriteLine(" -o <output>");
            Console.WriteLine(" -c <config>");
            Console.WriteLine(" -d <directory>");
            Console.WriteLine("    base directory for files. relative path from input file.");
            Console.WriteLine(" -td <directory>");
            Console.WriteLine("    tool directory. relative path from ArchiveConverter.");
        }

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
