using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Serialization;
using System.Xml.Schema;
using Utility.Xml;
using System.Reflection;
using System.IO;

namespace ArchiveConverter
{
    [XmlRoot("archives")]
    public class ArchiveRoot
        : IXmlSerializable
    {
        static ArchiveRoot()
        {
            // リフレクションでエレメント名とクラスの辞書を作成する
            // これによりクラスを追加するだけで動的にデシリアライズ処理が行われる

            var asm = Assembly.GetExecutingAssembly();

            foreach (var type in asm.GetTypes())
            {
                if (type.Namespace == typeof(ArchiveRoot).Namespace
                    && type.IsClass
                    && type.IsSubclassOf(typeof(ArchiveItem)))
                {
                    // Attributeを調べる
                    foreach (var attr in type.GetCustomAttributes(true))
                    {
                        if (attr is XmlRootAttribute)
                        {
                            var name = ((XmlRootAttribute)attr).ElementName;

                            if (!ArchiveItemType.ContainsKey(name))
                            {
                                ArchiveItemType.Add(name, type);
                            }
                        }
                    }
                }
            }
        }

        static Dictionary<string, Type> ArchiveItemType = new Dictionary<string, Type>();

        /// <summary>
        /// 読み込まれた要素で指定されたソースのパスの先頭に付加されるパス.
        /// </summary>
        static public string BasePath = "";

        static public ArchiveRoot Deserialize(string path)
        {
            using (var fs = new FileStream(path, FileMode.Open, FileAccess.Read))
            {
                var serializer = new XmlSerializer(typeof(ArchiveRoot));

                var ret = (ArchiveRoot)serializer.Deserialize(fs);
                return ret;
            }
        }

        public ArchiveRoot()
        {
        }

        [XmlIgnore]
        public List<ArchiveItem> Items = new List<ArchiveItem>();

        public XmlSchema GetSchema()
        {
            return null;
        }

        public void ReadXml(XmlReader reader)
        {
            reader.ReadStartElement();

            for (; ; )
            {
                if (reader.NodeType == XmlNodeType.Element)
                {
                    var name = reader.Name;

                    // 辞書に登録されているものをシリアライズする
                    if (ArchiveItemType.ContainsKey(name))
                    {
                        var s = new XmlSerializer(ArchiveItemType[name]);
                        var item = (ArchiveItem)s.Deserialize(reader);

                        if (string.IsNullOrEmpty(item.Type))
                        {
                            // Typeが空の場合は要素名を設定.
                            item.Type = name;
                        }

                        // ベースのパスが指定されていれば、それを付加する
                        if (!string.IsNullOrEmpty(ArchiveRoot.BasePath))
                        {
                            item.Source = ArchiveRoot.BasePath + "/" + item.Source;
                        }

                        // 出力ファイル名はファイル名のみが有効
                        if (!string.IsNullOrEmpty(item.Dest))
                        {
                            item.Dest = Path.GetFileName(item.Dest);
                        }

                        this.Items.Add(item);
                    }
                }
                else if (reader.NodeType == XmlNodeType.EndElement
                    && reader.Name == "archives")
                {
                    break;
                }
                else
                {
                    reader.MoveToElement();
                }
            }
        }

        public void WriteXml(XmlWriter writer)
        {
            // For test.

            // Desirialize xml without namespace.
            var ns = new XmlSerializerNamespaces();
            ns.Add("", "");

            foreach (var item in this.Items)
            {
                var s = new XmlSerializer(item.GetType());
                s.Serialize(writer, item, ns);
            }
        }
    }

    public class ArchiveItem
    {
        [XmlAttribute("src")]
        public string Source
        {
            get;
            set;
        }

        [XmlAttribute("dst")]
        public string Dest
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

        [XmlAttribute("type")]
        public string Type
        {
            get;
            set;
        }
    }

    [XmlRoot("img")]
    public class ImageItem : ArchiveItem
    {
    }

    [XmlRoot("mdl")]
    public class ModelItem : ArchiveItem
    {
    }

    [XmlRoot("anm")]
    public class AnimationItem : ArchiveItem
    {
    }

    [XmlRoot("shd")]
    public class ShaderElelemnt : ArchiveItem
    {
    }

    [XmlRoot("pes")]
    public class PostEffectItem : ArchiveItem
    {
    }

    [XmlRoot("text")]
    public class StringItem : ArchiveItem
    {
    }

    [XmlRoot("fnt")]
    public class FontItem : ArchiveItem
    {
    }

    [XmlRoot("data")]
    public class DataItem : ArchiveItem
    {
    }
}
