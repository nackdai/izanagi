using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace ImageViewer
{
    public enum NodeState : int
    {
        Normal,
        Selected,
    }

    public enum NodeType : int
    {
        Master,
        Texture,
        Image,
        None,
    }

    /// <summary>
    /// TreeViewのノード
    /// 
    /// TreeViewに対するViewModelとなる
    /// </summary>
    class ImageTreeViewNode : TreeViewNode
    {
        private ImageSource[] imgSource = new ImageSource[2];
        private NodeState state = NodeState.Normal;

        public string Name
        {
            get;
            private set;
        }

        public ImageSource ImgSrc
        {
            get
            {
                return imgSource[(int)State];
            }
        }

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

        public NodeType Type
        {
            get;
            private set;
        }

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

        private void ReadImage()
        {
            imgSource[(int)NodeState.Normal] = EmbeddedResourceProxy.CreateBitmapImageFromResource(
                "ImageViewer.Resource.ImgMaster_Normal.png");

            imgSource[(int)NodeState.Selected] = EmbeddedResourceProxy.CreateBitmapImageFromResource(
                "ImageViewer.Resource.ImgMaster_Selected.png");
        }
    }
}
