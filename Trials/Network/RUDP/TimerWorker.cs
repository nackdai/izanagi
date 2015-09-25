using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace RUDP
{
    class TimerWorker
    {
        /// <summary>
        /// 時間測定用.
        /// </summary>
        Stopwatch StopWatch = new Stopwatch();

        Thread Runner;

        public delegate void Proc();

        Proc ProcFunc = null;

        bool WillJoin = false;

        public TimerWorker(Proc proc)
        {
            this.ProcFunc = proc;

            this.Runner = new Thread(Run);
            this.Runner.Start();
        }

        private void Run()
        {
            while (true)
            {
                if (WillJoin)
                {
                    break;
                }

                // TODO
                // どれだけ待つか計算する.
                long wait = 16;

                this.StopWatch.Start();

                while (true)
                {
                    if (this.StopWatch.ElapsedMilliseconds >= wait)
                    {
                        break;
                    }
                }

                this.StopWatch.Stop();

                if (WillJoin)
                {
                    break;
                }

                this.ProcFunc();
            }
        }

        public void Join()
        {
            WillJoin = true;

            this.Runner.Join();
        }
    }
}
