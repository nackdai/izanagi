using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace RUDP
{
    class Counter
    {
        AtomicInt SequenceNumber = new AtomicInt(-1);

        /// <summary>
        /// 送信されたが返事のないセグメントの数.
        /// </summary>
        AtomicInt OutStandingSegmentCount = new AtomicInt(0);

        /// <summary>
        /// 現在のシーケンス番号.
        /// </summary>
        AtomicInt LastInSequenceNumber = new AtomicInt(-1);

        public int SetSequenceNumber(int n)
        {
            return this.SequenceNumber.Store(n);
        }

        public int NextSequenceNumber()
        {
            return this.SequenceNumber.Increment();
        }

        public int GetOutStandingSegmentCount()
        {
            return this.OutStandingSegmentCount.Load();
        }

        public int IncrementOutStandingSegmentCount()
        {
            return this.OutStandingSegmentCount.Increment();
        }

        /// <summary>
        /// 現在のシーケンス番号を設定.
        /// </summary>
        /// <param name="n"></param>
        /// <returns></returns>
        public int SetLastInSequenceNumber(int n)
        {
            return this.LastInSequenceNumber.Store(n);
        }

        /// <summary>
        /// 現在のシーケンス番号を取得.
        /// </summary>
        /// <returns></returns>
        public int GetLastInSequenceNumber()
        {
            return this.LastInSequenceNumber.Load();
        }
    }
}
