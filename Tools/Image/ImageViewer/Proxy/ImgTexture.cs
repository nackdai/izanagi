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
        // 親のイメージルートデータ
        private ImgMaster parent = null;

        // ネイティブで生成された本体
        private IntPtr texBody = IntPtr.Zero;

        // テクスチャデータが持っているイメージデータ（ミップマップ、キューブイメージ）
        private List<List<ImgImage>> imageList = new List<List<ImgImage>>();

        /// <summary>
        /// 親のイメージオブジェクト
        /// 親はマスターデータ
        /// </summary>
        public IImgObject Parent
        {
            get
            {
                return this.parent;
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
                return this.imageList.Count;
            }
        }

        /// <summary>
        /// テクスチャデータ内のミップマップ数
        /// </summary>
        public int MipMapNum
        {
            get
            {
                if (this.imageList.Count > 0)
                {
                    return this.imageList[0].Count;
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
                if (this.ImageNum > 0 && this.MipMapNum > 0)
                {
                    return this.imageList[0][0].Width;
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
                if (this.ImageNum > 0 && this.MipMapNum > 0)
                {
                    return this.imageList[0][0].Height;
                }
                return 0;
            }
        }

        /// <summary>
        /// テクスチャタイプ
        /// </summary>
        public enum TextureType
        {
            Plane,
            Cube,
            Volume,
        }

        /// <summary>
        /// テクスチャタイプ
        /// </summary>
        public TextureType Type
        {
            get
            {
                uint type = ImageLibDllProxy.GetTextureType(this.texBody);
                TextureType ret = (TextureType)type;
                return ret;
            }
        }

        private ImgTexture()
        {
        }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="master">親のマスタデータ</param>
        /// <param name="body">ネイティブで生成された本体</param>
        internal ImgTexture(ImgMaster master, IntPtr body)
        {
            this.parent = master;
            this.texBody = body;

            if (this.texBody != IntPtr.Zero)
            {
                this.ID = ImgMaster.ImgObjID++;

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

                    this.imageList.Add(list);
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
            foreach (var list in this.imageList)
            {
                foreach (var img in list)
                {
                    yield return img;
                }
            }
        }

        internal void RemoveImage(ImgImage img)
        {
            foreach (var list in this.imageList)
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
            this.parent.RemoveTexture(this);
        }
    }
}
