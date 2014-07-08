using System;
using System.Collections.Generic;
using System.IO;

namespace ImageViewer
{
    /// <summary>
    /// イメージデータのルート
    /// 基本的にはイメージファイルと１対１になる.
    /// </summary>
    public class ImgMaster : DisposableObject, IImgObject
    {
        /// <summary>
        /// イメージオブジェクト全体でのID生成用
        /// </summary>
        static internal int ImgObjID = 0;

        // ネイティブで生成された本体
        private IntPtr masterBody = IntPtr.Zero;

        /// <summary>
        /// マスタデータに含まれるテクスチャのリスト
        /// </summary>
        private List<ImgTexture> texList = new List<ImgTexture>();

        /// <summary>
        /// ルートなので常にnullを返す.
        /// </summary>
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

        // ファイルパスが更新されたかどうか
        private bool pathUpdated = false;

        // ファイルパス
        public string _path = null;

        /// <summary>
        /// マスタデータの元になったファイルパス
        /// </summary>
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
        /// 保持しているファイルパスの拡張子
        /// </summary>
        public string Ext
        {
            get
            {
                var ext = System.IO.Path.GetExtension(this.Path);
                return ext;
            }
        }

        /// <summary>
        /// マスタデータに含まれるテクスチャ数
        /// </summary>
        public int TextureNum
        {
            get
            {
                return texList.Count;
            }
        }

        /// <summary>
        /// 指定されたパスからマスタデータ作成
        /// </summary>
        /// <param name="path">ファイルパス</param>
        /// <returns>作成されたマスタデータ</returns>
        static public ImgMaster CreateImgMaster(string path)
        {
            // Get name from path.
            string name = System.IO.Path.GetFileNameWithoutExtension(path);

            ImgMaster ret = CreateImgMaster(name, path);
            return ret;
        }

        /// <summary>
        /// 指定されたパスからマスタデータ作成
        /// </summary>
        /// <param name="name">名前</param>
        /// <param name="path">ファイルパス</param>
        /// <returns>作成されたマスタデータ</returns>
        static public ImgMaster CreateImgMaster(
            string name,
            string path)
        {
            // Read data from path.
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

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="name">名前</param>
        /// <param name="path">ファイルパス</param>
        /// <param name="body">ネイティブで生成された本体</param>
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

                // 本体内に存在するテクスチャデータ数を取得
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

        /// <summary>
        /// マスタデータ内のテクスチャを取得
        /// </summary>
        /// <returns>テクスチャ</returns>
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

        protected override void Disposing()
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
