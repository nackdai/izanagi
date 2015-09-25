using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RUDP
{
    class SafeQueue<Type>
    {
        Object Locker = new Object();
        Queue<Type> Queue = new Queue<Type>();

        public int Count
        {
            get
            {
                lock (this.Locker)
                {
                    return this.Queue.Count;
                }
            }
        }

        public SafeQueue()
        {
        }

        public void Enqueue(Type item)
        {
            lock (this.Locker)
            {
                this.Queue.Enqueue(item);
            }
        }

        public Type Dequeue()
        {
            lock (this.Locker)
            {
                return this.Queue.Dequeue();
            }
        }
    }
}
