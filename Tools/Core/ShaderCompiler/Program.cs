using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.IO;
using System.Reflection;

namespace ShaderCompiler
{
    class Program
    {
        static void Main(string[] args)
        {
            Compiler.Do(args);
        }
    }

    static public unsafe class Compiler
    {
        static void DisplayUsage()
        {
            Console.WriteLine("Usage : ShaderCompiler <options>");
            Console.WriteLine();
            Console.WriteLine("options ****");
            Console.WriteLine("-i <input>");
            Console.WriteLine("-o <output>");
            Console.WriteLine("-e <etnry>");
            Console.WriteLine("-p <profile>");
            Console.WriteLine("-I <includes>");
            Console.WriteLine("-D <defines>");
            Console.WriteLine("-opt <options> : options for native compiler");
        }

        static ProcessStartInfo PrepareProcessPreproc(Option option)
        {
            var asm = Assembly.GetEntryAssembly();
            string path = asm.Location;

            var preproc = Path.GetDirectoryName(path);
            preproc = Path.Combine(preproc, "Preproc.exe");

            if (!File.Exists(preproc))
            {
                // TODO
                throw new Exception();
            }

            string args = "";

            if (!string.IsNullOrEmpty(option.Includes))
            {
                args += "-I" + " " + option.Includes;
            }

            if (!string.IsNullOrEmpty(option.Defines))
            {
                args += "-D" + " " + option.Defines;
            }

            //args += " " + "-EP";

            args += " " + option.Input;
            args += " " + option.PreprocessedFile;

            ProcessStartInfo info = new ProcessStartInfo();

            info.FileName = preproc;

            info.Arguments = args;

            info.CreateNoWindow = true;
            info.RedirectStandardOutput = true;
            info.RedirectStandardError = false;
            info.UseShellExecute = false;

            return info;
        }

        static void RunPreprocess(Option option)
        {
            var process = new Process();

            process.StartInfo = PrepareProcessPreproc(option);

            process.Start();
            process.WaitForExit();
        }

        static ProcessStartInfo PrepareProcessFxc(Option option)
        {
            var dxDir = System.Environment.GetEnvironmentVariable("DXSDK_DIR");
            if (string.IsNullOrEmpty(dxDir))
            {
                return null;
            }

            ProcessStartInfo info = new ProcessStartInfo();

            info.FileName = Path.Combine(dxDir, @"Utilities\bin\x64", "fxc.exe");

            info.Arguments = "/nologo";
            info.Arguments += " " + "/Fo" + " " + option.ByteCodedFile;
            info.Arguments += " " + "/E" + " " + option.Entry;
            info.Arguments += " " + "/T" + " " + option.Profile;
            info.Arguments += " " + option.PreprocessedFile;

            info.CreateNoWindow = true;
            info.RedirectStandardOutput = true;
            info.RedirectStandardError = true;
            info.UseShellExecute = false;

            return info;
        }

        static void RunFxc(Option option)
        {
            var process = new Process();

            process.StartInfo = PrepareProcessFxc(option);

            process.Start();
            process.WaitForExit();

            var output = process.StandardError.ReadToEnd();

            Console.WriteLine(output);
        }

        static public void Do(string[] args)
        {
            var option = new Option(args);

            if (!option.IsValid)
            {
                DisplayUsage();
                return;
            }

            RunPreprocess(option);
            RunFxc(option);

            char* bytecode = (char*)System.Runtime.InteropServices.Marshal.StringToHGlobalAnsi(option.ByteCodedFile);
            char* output = (char*)System.Runtime.InteropServices.Marshal.StringToHGlobalAnsi(option.Output);

            bool result = izanagi.tool.MojoShaderProxy.Parse((sbyte*)bytecode, (sbyte*)output);

            if (!result)
            {
                // TODO
                return;
            }
        }
    }

    class Option
    {
        public string Input;
        public string Output;
        public string Entry;
        public string Profile;

        public string PreprocessedFile;
        public string ByteCodedFile;

        public string Includes;
        public string Defines;
        public string Options;

        public Option(string[] args)
        {
            bool isOptions = false;
            bool isIncludes = false;
            bool isDefines = false;

            for (int i = 0; i < args.Length; i++)
            {
                var arg = args[i];

                if (arg == "-opt")
                {
                    isOptions = true;
                }
                else if (arg == "-I")
                {
                    isIncludes = true;
                }
                else if (arg == "-D")
                {
                    isDefines = true;
                }
                else if (arg == "-i")
                {
                    this.Input = args[++i];
                }
                else if (arg == "-o")
                {
                    this.Output = args[++i];
                }
                else if (arg == "-e")
                {
                    this.Entry = args[++i];
                }
                else if (arg == "-p")
                {
                    this.Profile = args[++i];
                }
                else if (isOptions)
                {
                    if (string.IsNullOrEmpty(this.Options))
                    {
                        this.Options = arg;
                    }
                    else
                    {
                        this.Options += " " + arg;
                    }
                }
                else if (isIncludes)
                {
                    if (string.IsNullOrEmpty(this.Includes))
                    {
                        this.Includes = arg;
                    }
                    else
                    {
                        this.Includes += " " + arg;
                    }
                }
                else if (isDefines)
                {
                    if (string.IsNullOrEmpty(this.Defines))
                    {
                        this.Defines = arg;
                    }
                    else
                    {
                        this.Defines += " " + arg;
                    }
                }
            }

            if (!string.IsNullOrEmpty(this.Input))
            {
                this.ByteCodedFile = Path.GetFileNameWithoutExtension(this.Input);
                this.ByteCodedFile += ".bc";

                this.PreprocessedFile = this.Input + "_";
            }
        }

        public bool IsValid
        {
            get
            {
                bool isInvalid = string.IsNullOrEmpty(this.Entry)
                    || string.IsNullOrEmpty(this.Profile)
                    || string.IsNullOrEmpty(this.Output)
                    || string.IsNullOrEmpty(this.Input)
                    || string.IsNullOrEmpty(this.ByteCodedFile)
                    || string.IsNullOrEmpty(this.PreprocessedFile);

                return !isInvalid;
            }
        }
    }
}
