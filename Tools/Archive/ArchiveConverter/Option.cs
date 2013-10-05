using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace ArchiveConverter
{
    class Option
    {
        public string Config
        {
            get;
            private set;
        }

        public string Input
        {
            get;
            private set;
        }

        public string Output
        {
            get;
            private set;
        }

        public string ObjDir
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
                    this.ObjDir = args[i + 1];
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
