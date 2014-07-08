using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace ImageViewer
{
    /// <summary>
    /// ツリービューのノードの状態
    /// </summary>
    public enum NodeState : int
    {
        Normal,
        Selected,
    }

    /// <summary>
    /// ツリービューのノードのタイプ
    /// </summary>
    public enum NodeType : int
    {
        Master,
        Texture,
        Image,
        None,
    }

    /// <summary>
    /// TreeViewのノード
    /// TreeViewに対するViewModelとなる
    /// </summary>
    class ImageTreeViewNode : TreeViewNode
    {
        private ImageSource[] imgSource = new ImageSource[2];
        private NodeState state = NodeState.Normal;

        /// <summary>
        /// ツリービューに表示するノードの名前
        /// </summary>
        public string Name
        {
            get;
            private set;
        }

        /// <summary>
        /// ツリービューに表示する画像のソース
        /// </summary>
        public ImageSource ImgSrc
        {
            get
            {
                return imgSource[(int)State];
            }
        }

        /// <summary>
        /// ノードの状態
        /// </summary>
        public NodeState State
        {
            get
            {
                return state;
            }
            set
            {
                if (state != value)
                {
                    state = value;

                    // ノードに表示する画像を変更する必要があるので
                    // ImgSrcの変更を通知する
                    NotifyPropertyChanged("ImgSrc");
                }
            }
        }

        /// <summary>
        /// ノードのタイプ
        /// </summary>
        public NodeType Type
        {
            get;
            private set;
        }

        /// <summary>
        /// ノードが持つモデル
        /// </summary>
        public IImgObject Model
        {
            get;
            private set;
        }

        protected ImageTreeViewNode()
        {
        }

        public ImageTreeViewNode(IImgObject imgObj, string name)
            : base()
        {
            Model = imgObj;
            Name = name;
            State = NodeState.Normal;

            // Modelの種類からTypeを決める
            Type = NodeType.None;
            if (Model is ImgMaster)
            {
                Type = NodeType.Master;
            }
            else if (Model is ImgTexture)
            {
                Type = NodeType.Texture;
            }
            else if (Model is ImgImage)
            {
                Type = NodeType.Image;
            }

            ReadImage();
        }

        /// <summary>
        /// ツリービューに表示するアイコン画像を読み込む
        /// </summary>
        private void ReadImage()
        {
            // 通常状態用
            imgSource[(int)NodeState.Normal] = EmbeddedResourceProxy.CreateBitmapImageFromResource(
                "ImageViewer.Resource.ImgMaster_Normal.png");

            // 選択状態用
            imgSource[(int)NodeState.Selected] = EmbeddedResourceProxy.CreateBitmapImageFromResource(
                "ImageViewer.Resource.ImgMaster_Selected.png");
        }
    }
}
