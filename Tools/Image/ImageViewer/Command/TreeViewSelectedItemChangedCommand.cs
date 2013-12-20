using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Input;
using System.Windows;

namespace ImageViewer
{
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
                var oldNode = e.OldValue as ImageTreeViewNode;
                var newNode = e.NewValue as ImageTreeViewNode;

                if (oldNode != null)
                {
                    oldNode.State = NodeState.Normal;
                }
                if (newNode != null)
                {
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
