using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ImageViewerInfra
{
    public class ImgMaster : IImgObject
    {
        static internal int ImgObjID = 0;

        private IntPtr masterBody = IntPtr.Zero;

        private List<ImgTexture> texList = new List<ImgTexture>();

        public IImgObject Parent
        {
            get
            {
                return null;
            }
        }

        public int ID
        {
            get;
            private set;
        }

        public string Name
        {
            get;
            private set;
        }

        private bool pathUpdated = false;
        public string _path = null;

        public string Path
        {
            get
            {
                return _path;
            }
            private set
            {
                if (_path != value)
                {
                    _path = value;
                    pathUpdated = true;
                }
            }
        }

        /// <summary>
        /// テクスチャ数
        /// </summary>
        public int TextureNum
        {
            get
            {
                return texList.Count;
            }
        }

        static public ImgMaster CreateImgMaster(string path)
        {
            string name = System.IO.Path.GetFileNameWithoutExtension(path);

            ImgMaster ret = CreateImgMaster(name, path);
            return ret;
        }

        static public ImgMaster CreateImgMaster(
            string name,
            string path)
        {
            IntPtr ptr = ImageLibDllProxy.Read(path);
            if (ptr != IntPtr.Zero)
            {
                ImgMaster ret = new ImgMaster(name, path, ptr);
                return ret;
            }

            return null;
        }

        private ImgMaster()
        {
        }

        private ImgMaster(
            string name,
            string path,
            IntPtr body)
        {
            Name = name;
            Path = path;
            masterBody = body;

            // マスタ内のテクスチャをテクスチャリストに登録
            if (masterBody != IntPtr.Zero)
            {
                ID = ImgObjID++;

                pathUpdated = true;

                var num = ImageLibDllProxy.GetTexNumInMaster(masterBody);

                for (uint i = 0; i < num; i++)
                {
                    var texBody = ImageLibDllProxy.GetTextureInMaster(masterBody, i);
                    if (texBody != IntPtr.Zero)
                    {
                        var tex = new ImgTexture(this, texBody);
                        texList.Add(tex);
                    }
                }
            }
        }

        /// <summary>
        /// マスタデータ内のテクスチャを取得
        /// </summary>
        /// <param name="idx">取得したいテクスチャのインデックス</param>
        /// <returns>テクスチャ</returns>
        public ImgTexture GetTexture(int idx)
        {
            return texList[idx];
        }

        public IEnumerable<ImgTexture> GetTexture()
        {
            foreach (var tex in texList)
            {
                yield return tex;
            }
        }

        internal void RemoveTexture(ImgTexture tex)
        {
            if (texList.Remove(tex))
            {
                // TODO
            }
        }

        public void Dispose()
        {
            foreach (var tex in texList)
            {
                tex.Dispose();
            }

            if (masterBody != IntPtr.Zero)
            {
                ImageLibDllProxy.Release(masterBody);
            }
        }
    }
}
