using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ImagePacker
{
    class Program
    {
        static void DisplayUsage()
        {
            Console.WriteLine("Usage : ImagePacker <options>");
            Console.WriteLine("");
            Console.WriteLine("Options");
            Console.WriteLine(" -i <input>");
            Console.WriteLine(" -d <directory>");
            Console.WriteLine(" -o <output>");
            Console.WriteLine(" -f <format>");
        }

        static void Main(string[] args)
        {
        }
    }

    class Option
    {
        public string Directory;
        public string Input;
        public string Output;
        public string Format;

        public Option(string[] args)
        {
            for (var i = 0; i < args.Length; i++)
            {
                var arg = args[i];

                if (arg == "-i")
                {
                    this.Input = args[++i];
                }
                else if (arg == "-d")
                {
                    this.Directory = args[++i];
                }
                else if (arg == "-o")
                {
                    this.Output = args[++i];
                }
                else if (arg == "-f")
                {
                    this.Format = args[++i];
                    this.Format = this.Format.ToLower();
                }
            }
        }

        public bool IsValid()
        {
            // TODO
            return true;
        }
    }
}
