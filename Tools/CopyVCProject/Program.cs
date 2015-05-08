using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;

namespace CopyVCProject
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length != 2)
            {
                // TODO
                Console.WriteLine("Argument error.");
                return;
            }

            var rootName = Path.GetFileNameWithoutExtension(args[1]);

            var xmlDocument = new XmlDocument();

            try
            {
                xmlDocument.Load(args[0]);
            }
            catch (Exception ex)
            {
                Console.Write(ex.Message);
                return;
            }

            var child = xmlDocument.FirstChild;

            for (; ; )
            {
                child = child.NextSibling;
                if (child.Name == "Project")
                {
                    break;
                }
            }

            bool doneGUID = false;
            bool doneRootnamespace = false;

            for (child = child.FirstChild; ; child = child.NextSibling)
            {
                if (child.Name == "PropertyGroup")
                {
                    foreach (XmlAttribute attr in child.Attributes)
                    {
                        if (attr.Name == "Label" && attr.Value == "Globals")
                        {
                            var childChild = child.FirstChild;

                            for (childChild = child.FirstChild; ; childChild = childChild.NextSibling)
                            {
                                if (childChild.Name == "ProjectGuid")
                                {
                                    foreach (XmlNode node in childChild.ChildNodes)
                                    {
                                        // Create guid.
                                        var guid = Guid.NewGuid();
                                        var strGUID = guid.ToString();

                                        node.Value = "{" + strGUID + "}";

                                        doneGUID = true;
                                        break;
                                    }
                                }
                                else if (childChild.Name == "RootNamespace")
                                {
                                    foreach (XmlNode node in childChild.ChildNodes)
                                    {
                                        node.Value = rootName;

                                        doneRootnamespace = true;
                                        break;
                                    }
                                }

                                if (doneGUID && doneRootnamespace)
                                {
                                    break;
                                }
                            }
                        }

                        if (doneGUID && doneRootnamespace)
                        {
                            break;
                        }
                    }
                }

                if (doneGUID && doneRootnamespace)
                {
                    break;
                }
            }

            // Save XML.
            try
            {
                using (var fs = new FileStream(args[1], FileMode.Create, FileAccess.Write))
                {
                    xmlDocument.Save(fs);
                }
            }
            catch (Exception ex)
            {
                Console.Write(ex.Message);
                return;
            }
        }
    }
}
