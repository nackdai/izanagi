using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;
using System.IO;

namespace ArchiveConverter
{
    [XmlRoot("config")]
    public class Config : List<ExeCmdItem>
    {
        public Config()
        {
        }

        public void SetBaseDir(string dir)
        {
        }

        public ExeCmdItem this[string type]
        {
            private set
            {
                // Nothing...
            }
            public get
            {
                var ret = this.Find((item) =>
                {
                    if (item.Type == type)
                    {
                        return true;
                    }
                    return false;
                });

                if (ret == null)
                {
                    throw new InvalidOperationException(string.Format("There is not config item({0}).", type));
                }

                return ret;
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

    [XmlRoot("item")]
    public class ExeCmdItem
    {
        [XmlAttribute("type")]
        public string Type
        {
            get;
            set;
        }

        [XmlAttribute("exe")]
        public string Exe
        {
            get;
            set;
        }

        [XmlAttribute("in")]
        public string InputOption
        {
            get;
            set;
        }

        [XmlAttribute("out")]
        public string OutputOption
        {
            get;
            set;
        }

        [XmlAttribute("opt")]
        public string Option
        {
            get;
            set;
        }

        public void DoCmd(
            string dir, 
            string input,
            string output,
            string option)
        {
            if (string.IsNullOrEmpty(input)
                || string.IsNullOrEmpty(output))
            {
                // TODO
                throw new InvalidOperationException("");
            }

            var cmd = this.Exe;
            if (!string.IsNullOrEmpty(dir))
            {
                cmd = dir + "/" + cmd;
            }

            cmd += " " + this.InputOption + input;
            cmd += " " + this.OutputOption + output;

            if (!string.IsNullOrEmpty(this.Option))
            {
                cmd += " " + this.Option;
            }
            if (!string.IsNullOrEmpty(option))
            {
                cmd += " " + option;
            }

            // TODO
            // Do command
        }
    }
}
