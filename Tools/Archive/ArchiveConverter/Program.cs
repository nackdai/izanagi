using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ArchiveConverter
{
    class Program
    {
        static void Main(string[] args)
        {
            var serializer = new System.Xml.Serialization.XmlSerializer(typeof(ArchiveRoot));

            //var fs = new System.IO.FileStream("hoge.xml", System.IO.FileMode.Open, System.IO.FileAccess.Read);
            var fs = new System.IO.FileStream("hoge.xml", System.IO.FileMode.Open, System.IO.FileAccess.Write);

            var root = new ArchiveRoot();

            var img = new ImageItem()
            {
                Source = "image.png",
                Option = "-D GLES2 -t gles2",
            };

            root.Items.Add(img);
            root.Items.Add(new ModelItem());

            //var root = (ArchiveRoot)serializer.Deserialize(fs);
            serializer.Serialize(fs, root);

            fs.Close();

            fs = new System.IO.FileStream("hoge.xml", System.IO.FileMode.Open, System.IO.FileAccess.Read);
            serializer = new System.Xml.Serialization.XmlSerializer(typeof(ArchiveRoot));
            var r = (ArchiveRoot)serializer.Deserialize(fs);
        }
    }

    
}
