using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Input;
using System.Windows;

namespace ImageViewer
{
    /// <summary>
    /// ツリービューで選択アイテムが変更されたときのコマンド
    /// </summary>
    class TreeViewSelectedItemChangedCommand : ICommand
    {
        static private TreeViewSelectedItemChangedCommand instance = new TreeViewSelectedItemChangedCommand();

        static public TreeViewSelectedItemChangedCommand Command
        {
            get
            {
                return instance;
            }
        }

        public event EventHandler CanExecuteChanged;
        public event Action<IImgObject> OnSelectedItemChanged;

        private TreeViewSelectedItemChangedCommand()
        {
            // Nothing is done...
        }

        public bool CanExecute(object parameter)  
        {  
            return true;
        }  
  
        public void Execute(object parameter)
        {
            var e = parameter as RoutedPropertyChangedEventArgs<object>;
            if (e != null)
            {
                // 前に選択されていたアイテム
                var oldNode = e.OldValue as ImageTreeViewNode;

                // 今回選択したアイテム
                var newNode = e.NewValue as ImageTreeViewNode;

                if (oldNode != null)
                {
                    // 通常状態
                    oldNode.State = NodeState.Normal;
                }
                if (newNode != null)
                {
                    // 選択状態
                    newNode.State = NodeState.Selected;
                }

                if (OnSelectedItemChanged != null)
                {
                    OnSelectedItemChanged(newNode.Model);
                }
            }
        }
    }
}
