using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace ArchiveConverter
{
    [XmlRoot("archives")]
    public class ArchiveRoot
    {
        public ArchiveRoot()
        {
        }

        [XmlElement("groups")]
        public List<ArchiveGroup> Groups = new List<ArchiveGroup>();
    }

    public class ArchiveItem
    {
        [XmlAttribute("src")]
        public string Source
        {
            get;
            set;
        }

        [XmlAttribute("option")]
        public string Option
        {
            get;
            set;
        }

        [XmlAttribute("type")]
        public string Type
        {
            get;
            set;
        }
    }

    public class ArchiveGroup
    {
    }

    [XmlRoot("img")]
    public class ImageElement : ArchiveGroup
    {
    }

    [XmlRoot("mdl")]
    public class ModelEleement : ArchiveGroup
    {
    }

    [XmlRoot("anm")]
    public class AnimationElement : ArchiveGroup
    {
    }

    [XmlRoot("shd")]
    public class ShaderElelemnt : ArchiveGroup
    {
    }

    [XmlRoot("pes")]
    public class PostEffectElement : ArchiveGroup
    {
    }

    [XmlRoot("text")]
    public class StringElement : ArchiveGroup
    {
    }

    [XmlRoot("fnt")]
    public class FontElement : ArchiveGroup
    {
    }

    [XmlRoot("data")]
    public class DataElement : ArchiveGroup
    {
    }
}
