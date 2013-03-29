using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;
using System.IO;
using System.Collections.ObjectModel;

namespace ImageViewer
{
    class TextureItem
    {
        [XmlAttribute("fmt")]
        public string FormatString
        {
            get;
            set;
        }
    }

    [XmlElement("plane")]
    class PlaneElement : TextureItem
    {
        [XmlAttribute("path")]
        public string Path
        {
            get;
            set;
        }
    }

    [XmlElement("cube")]
    class CubeElement
    {
        public ObservableCollection<CubeItemElement> Items
        {
            get;
            set;
        }
    }

    enum CubeFace
    {
        xp,
        xn,
        yp,
        yn,
        zp,
        zn,
    }

    [XmlElement("element")]
    class CubeItemElement
    {
        [XmlAttribute("path")]
        public string Path
        {
            get;
            set;
        }

        [XmlAttribute("face")]
        public CubeFace Face
        {
            get;
            set;
        }
    }

    [XmlRoot("textures")]
    class Project
    {
        private Project()
        {
        }

        static void ListingFiles(string dirpath, List<string> files)
        {
            foreach (var file in Directory.GetFiles(dirpath))
            {
                files.Add(file);
            }
            foreach (var dir in Directory.GetDirectories(dirpath))
            {
                ListingFiles(dir, files);
            }
        }

        public ObservableCollection<TextureItem> 
    }
}
