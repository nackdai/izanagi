using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RUDP
{
    internal class SafeList<Type>
    {
        Object Locker = new Object();
        List<Type> List = new List<Type>();

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
        }

        public void Add(Type item)
        {
            lock (this.Locker)
            {
                this.List.Add(item);
            }
        }

        public void Clear()
        {
            lock (this.Locker)
            {
                this.List.Clear();
            }
        }

        public bool Remove(Type item)
        {
            lock (this.Locker)
            {
                return this.List.Remove(item);
            }
        }

        public void RemoveAt(int index)
        {
            lock (this.Locker)
            {
                this.List.RemoveAt(index);
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
    }
}
