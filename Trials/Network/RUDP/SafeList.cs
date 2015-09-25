using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RUDP
{
    internal class SafeList<Type>
    {
        List<Type> List = new List<Type>();

        public Object Locker
        {
            get;
            private set;
        }

        public int Count
        {
            get
            {
                lock (this.Locker)
                {
                    return this.List.Count;
                }
            }
        }

        public SafeList()
        {
            this.Locker = new Object();
        }

        public void Add(Type item)
        {
            if (this.IsInForeachWithRemoving)
            {
                throw new InvalidOperationException();
            }

            lock (this.Locker)
            {
                this.List.Add(item);
            }
        }

        public void Clear()
        {
            if (this.IsInForeachWithRemoving)
            {
                throw new InvalidOperationException();
            }

            lock (this.Locker)
            {
                this.List.Clear();
            }
        }

        public bool Remove(Type item)
        {
            if (this.IsInForeachWithRemoving)
            {
                throw new InvalidOperationException();
            }

            lock (this.Locker)
            {
                return this.List.Remove(item);
            }
        }

        public bool IsEmpty()
        {
            lock (this.Locker)
            {
                return (this.List.Count == 0);
            }
        }

        public Type this[int index]
        {
            get
            {
                lock (this.Locker)
                {
                    return this.List[index];
                }
            }
        }

        bool IsInForeachWithRemoving = false;

        public delegate bool FuncForeachWithRemoving(Type item);

        public void ForeachWithRemoving(FuncForeachWithRemoving cond)
        {
            this.IsInForeachWithRemoving = true;

            List<Type> willRemoveList = new List<Type>();

            lock (this.Locker)
            {
                for (int i = 0; i < this.List.Count; i++)
                {
                    if (cond(this.List[i]))
                    {
                        willRemoveList.Add(this.List[i]);
                    }
                }

                for (int i = 0; i < willRemoveList.Count; i++)
                {
                    this.List.Remove(willRemoveList[i]);
                }
            }

            this.IsInForeachWithRemoving = false;
        }

        public delegate void FuncForeach(Type item);

        public void Foreach(FuncForeach func)
        {
            lock (this.Locker)
            {
                for (int i = 0; i < this.List.Count; i++)
                {
                    func(this.List[i]);
                }
            }
        }
    }
}
