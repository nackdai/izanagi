using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace ArchiveConverter
{
    static class KeyDefsFileExporter
    {
        enum PathType : int
        {
            File,
            Directory,
        }

        class PathItem
        {
            public string Name;
            public PathType Type;

            public List<PathItem> TypeDirChildren = new List<PathItem>();
            public List<PathItem> TypeFileChildren = new List<PathItem>();

            public PathItem Parent;
        }

        static PathItem Root;
        static List<PathItem> PathItemList = new List<PathItem>();

        static PathItem Find(PathItem root, string name)
        {
            foreach (var child in root.TypeDirChildren)
            {
                if (child.Name == name)
                {
                    return child;
                }
            }
            foreach (var child in root.TypeFileChildren)
            {
                if (child.Name == name)
                {
                    return child;
                }
            }

            return null;
        }

        static public void Register(ArchiveRoot root, string rootName)
        {
            Root = new PathItem()
            {
                Name = rootName,
                Type = PathType.Directory,
            };

            foreach (var item in root.Items)
            {
                var paths = item.Name.Split('/', '\\');

                PathItem parent = Root;

                foreach (var pathStr in paths)
                {
                    if (pathStr.IndexOf('.') != 0)
                    {
                        var pathItem = PathItemList.Find(
                            (o) =>
                            {
                                return o.Name == pathStr;
                            }
                        );

                        if (pathItem == null)
                        {
                            pathItem = new PathItem()
                            {
                                Name = pathStr,
                                Type = (pathStr.IndexOf('.') > 0 ? PathType.File : PathType.Directory),
                                Parent = parent,
                            };

                            if (pathItem.Type == PathType.File
                                && !string.IsNullOrEmpty(item.Key))
                            {
                                pathItem.Name = item.Key;
                            }

                            if (parent != null)
                            {
                                if (pathItem.Type == PathType.Directory)
                                {
                                    parent.TypeDirChildren.Add(pathItem);
                                }
                                else
                                {
                                    parent.TypeFileChildren.Add(pathItem);
                                }
                            }

                            PathItemList.Add(pathItem);
                        }

                        parent = pathItem;
                    }
                }
            }
        }

        static int Indent = 0;

        static public void Export(string path)
        {
            var name = Path.GetFileName(path);
            name = name.Replace('.', '_');
            name = name.ToUpper();
            name = "_" + name + "_";

            using (var sw = new StreamWriter(path, false, Encoding.UTF8))
            {
                WriteLine(sw, string.Format("#ifndef {0}", name));
                WriteLine(sw, string.Format("#define {0}", name));
                Indent++;

                Export(sw, Root);

                Indent--;
                WriteLine(sw, "#endif");
            }
        }

        static void WriteLine(StreamWriter sw, string str)
        {
            for (int i = 0; i < Indent; i++)
            {
                sw.Write("    ");
            }

            sw.WriteLine(str);
        }

        static void Export(StreamWriter sw, PathItem parent)
        {
            var str = string.Format("namespace {0} ", parent.Name) + "{";
            WriteLine(sw, str);
            Indent++;

            foreach (var child in parent.TypeDirChildren)
            {
                Export(sw, child);
            }

            bool hasFileItem = (parent.TypeFileChildren.Find((o) => { return o.Type == PathType.File; }) != null);

            if (hasFileItem)
            {
                WriteLine(sw, "enum {");
                Indent++;
            }

            foreach (var child in parent.TypeFileChildren)
            {
                var name = Path.GetFileName(child.Name);
                name = name.Replace('.', '_');

                WriteLine(sw, string.Format("{0},", name));
            }

            if (hasFileItem)
            {
                Indent--;
                WriteLine(sw, "};");
            }

            Indent--;
            WriteLine(sw, "}");
        }
    }
}
