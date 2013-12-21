using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ImageViewer
{
    /// <summary>
    /// テクスチャデータ
    /// </summary>
    public class ImgTexture : DisposableObject, IImgObject
    {
        private ImgMaster parent = null;
        private IntPtr texBody = IntPtr.Zero;

        private List<List<ImgImage>> imageList = new List<List<ImgImage>>();

        /// <summary>
        /// 親のイメージオブジェクト
        /// 親はマスターデータ
        /// </summary>
        public IImgObject Parent
        {
            get
            {
                return parent;
            }
        }

        public int ID
        {
            get;
            private set;
        }

        /// <summary>
        /// テクスチャデータ内のイメージ数
        /// </summary>
        public int ImageNum
        {
            get
            {
                return imageList.Count;
            }
        }

        /// <summary>
        /// テクスチャデータ内のミップマップ数
        /// </summary>
        public int MipMapNum
        {
            get
            {
                if (imageList.Count > 0)
                {
                    return imageList[0].Count;
                }
                return 0;
            }
        }

        /// <summary>
        /// テクスチャデータの画像幅
        /// </summary>
        public int Width
        {
            get
            {
                if (ImageNum > 0 && MipMapNum > 0)
                {
                    return imageList[0][0].Width;
                }
                return 0;
            }
        }

        /// <summary>
        /// テクスチャデータの画像高さ
        /// </summary>
        public int Height
        {
            get
            {
                if (ImageNum > 0 && MipMapNum > 0)
                {
                    return imageList[0][0].Height;
                }
                return 0;
            }
        }

        private ImgTexture()
        {
        }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="master">親のマスタデータ</param>
        /// <param name="body">テクスチャデータ</param>
        internal ImgTexture(ImgMaster master, IntPtr body)
        {
            parent = master;
            texBody = body;

            if (texBody != IntPtr.Zero)
            {
                ID = ImgMaster.ImgObjID++;

                // Get number of images in texture.
                var num = ImageLibDllProxy.GetImageNumInTexture(texBody);

                // NOTE
                // どのイメージでもミップマップ数は同じ
                // テクスチャが複数のイメージを持つ場合はキューブマップのときのみで
                // キューブマップの場合はすべてが同じミップマップ数になる
                var mipmap = ImageLibDllProxy.GetMipMapNumInTexture(texBody);

                // ImageImageのキャッシュ用リストを作成
                for (uint i = 0; i < num; i++)
                {
                    var list = new List<ImgImage>();

                    for (uint m = 0; m < mipmap; m++)
                    {
                        var data = ImageLibDllProxy.GetImageInTexture(texBody, i, m);
                        ImgImage img = new ImgImage(this, data);
                        list.Add(img);
                    }

                    imageList.Add(list);
                }
            }
        }

        /// <summary>
        /// イメージデータを取得
        /// </summary>
        /// <param name="idx">取得したいイメージデータのインデックス</param>
        /// <param name="level">取得したいミップマップのレベル</param>
        /// <returns>イメージデータオブジェクト</returns>
        public ImgImage GetImage(int idx, int level)
        {
            idx = Math.Min(idx, ImageNum - 1);
            level = Math.Min(level, MipMapNum - 1);

            return imageList[idx][level];
        }

        public IEnumerable<ImgImage> GetImage()
        {
            foreach (var list in imageList)
            {
                foreach (var img in list)
                {
                    yield return img;
                }
            }
        }

        internal void RemoveImage(ImgImage img)
        {
            foreach (var list in imageList)
            {
                if (list.Remove(img))
                {
                    // TODO
                    return;
                }
            }
        }

        protected override void Disposing()
        {
            parent.RemoveTexture(this);
        }
    }
}
