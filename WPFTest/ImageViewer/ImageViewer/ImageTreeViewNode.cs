using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using ImageViewerInfra;

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

        public ImageTreeViewNode(IImgObject imgObj, string name, NodeType type)
            : base()
        {
            Model = imgObj;
            Name = name;
            State = NodeState.Normal;
            Type = type;

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
