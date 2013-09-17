using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ArchiveConverter
{
    public class Person
    {
        public Person() { }

        public Person(string firstName, string lastName)
        {
            FirstName = firstName;
            LastName = lastName;
        }

        public string FirstName { get; set; }
        public string LastName { get; set; }
    }


    class Program
    {
        
        static void Main(string[] args)
        {
            var serializer = new System.Xml.Serialization.XmlSerializer(typeof(ArchiveRoot));
            var ms = new System.IO.MemoryStream();

            var root = new ArchiveRoot();
            //root.Groups.Add(new ImageElement());
            //root.Groups.Add(new ModelEleement());

            serializer.Serialize(ms, root);
            var xml = Encoding.UTF8.GetString(ms.ToArray());

            Console.WriteLine(xml);
        }
    }

    
}
