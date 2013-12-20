using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ImageViewer
{
    public interface IImgObject
    {
        IImgObject Parent
        {
            get;
        }

        int ID
        {
            get;
        }
    }

    public abstract class DisposableObject : IDisposable
    {
        ~DisposableObject()
        {
            this.Dispose(false);
        }

        public void Dispose()
        {
            this.Dispose(true);
            GC.SuppressFinalize(this);
        }

        private bool disposed = false;

        private void Dispose(bool disposing)
        {
            if (this.disposed)
            {
                return;
            }
            this.disposed = true;

            if (disposing)
            {
                Disposing();
            }
        }

        protected abstract void Disposing();
    }
}
