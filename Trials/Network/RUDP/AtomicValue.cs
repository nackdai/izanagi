using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RUDP
{
    class AtomicValue<Type>
        where Type : IComparable
    {
        protected Object Locker = new Object();
        protected Type Value;

        protected AtomicValue()
        {
        }

        public AtomicValue(Type v)
        {
            Store(v);
        }

        public Type Store(Type v)
        {
            lock(this.Locker)
            {
                this.Value = v;
                return this.Value;
            }
        }

        public Type Load()
        {
            lock (this.Locker)
            {
                return this.Value;
            }
        }

        public static explicit operator Type (AtomicValue<Type> v)
        {
            return v.Value;
        }

        public static bool operator ==(AtomicValue<Type> a, Type b)
        {
            return a.Value.Equals(b);
        }

        public static bool operator !=(AtomicValue<Type> a, Type b)
        {
            return !(a.Value.Equals(b));
        }
    }

    class AtomicInt : AtomicValue<int>
    {
        public AtomicInt()
            : this(0)
        {
        }

        public AtomicInt(int v)
        {
            Store(v);
        }

        public int Increment()
        {
            lock (this.Locker)
            {
                this.Value++;
                return this.Value;
            }
        }

        public int Decrement()
        {
            lock(this.Locker)
            {
                this.Value--;
                return this.Value;
            }
        }
    }
}
