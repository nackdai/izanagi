using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/*
*  RUDP Header
*
*   0 1 2 3 4 5 6 7 8            15
*  +-+-+-+-+-+-+-+-+---------------+
*  |S|A|E|R|N|C| | |    Header     |
*  |Y|C|A|S|U|H|0|0|    Length     |
*  |N|K|K|T|L|K| | |               |
*  +-+-+-+-+-+-+-+-+---------------+
*  |  Sequence #   +   Ack Number  |
*  +---------------+---------------+
*  |            Checksum           |
*  +---------------+---------------+
*
*/
namespace RUDP
{
    abstract public class Segment
    {
        public static int RUDP_VERSION = 1;
        public static int RUDP_HEADER_LEN = 6;

        public enum Type : int
        {
            SYN = 0x80, // SYNchronous
            ACK = 0x40, // ACKnowledged
            EAK = 0x20, // Extend AcKnowledged
            RST = 0x10, // ReSeT
            NUL = 0x08, // NULl
            CHK = 0x04, // CHecK
            FIN = 0x02, // FINish
        }

        protected int Flags
        {
            get;
            private set;
        }

        /// <summary>
        /// Header Length.
        /// </summary>
        protected int HeaderLength
        {
            get;
            private set;
        }

        /// <summary>
        /// Sequence Number.
        /// </summary>
        public int SequenceNumber
        {
            get;
            private set;
        }

        /// <summary>
        /// Acknowleged Number.
        /// </summary>
        protected int AckNumber
        {
            get;
            private set;
        }

        public int RetryCount
        {
            get;
            set;
        }

        protected Segment()
        {
        }

        protected Segment(
            Type type,
            int sequenceNumber,
            int hederSize)
        {
            this.Flags = (int)type;
            this.SequenceNumber = sequenceNumber;
            this.HeaderLength = hederSize;

            this.AckNumber = -1;
        }

        /// <summary>
        /// Get segment type as string.
        /// </summary>
        /// <returns></returns>
        public abstract string GetSegmentType();

        /// <summary>
        /// 応答番号をセット.
        /// この番号と一致する番号を持つセグメントに対する応答となる.
        /// </summary>
        /// <param name="ackNumber"></param>
        public void SetAcknowledgedNumber(int ackNumber)
        {
            this.Flags |= (byte)Type.ACK;
            this.AckNumber = ackNumber;
        }

        /// <summary>
        /// フラグチェック.
        /// </summary>
        /// <param name="flag"></param>
        /// <param name="type"></param>
        /// <returns></returns>
        private static bool CheckFlag(int flag, Type type)
        {
            return ((flag & (byte)type) != 0);
        }

        /// <summary>
        /// フラグチェック.
        /// </summary>
        /// <param name="type"></param>
        /// <returns></returns>
        protected bool CheckFlag(Type type)
        {
            return CheckFlag(this.Flags, type);
        }

        /// <summary>
        /// 応答番号を取得.
        /// </summary>
        /// <returns></returns>
        public int GetAcknowledgedNumber()
        {
            if (CheckFlag(Type.ACK))
            {
                return this.AckNumber;
            }

            return -1;
        }

        /// <summary>
        /// データをパースして、対応するセグメントを作成する.
        /// </summary>
        /// <param name="bytes"></param>
        /// <param name="offset"></param>
        /// <param name="length"></param>
        /// <returns></returns>
        public static Segment Parse(byte[] bytes, int offset, int length)
        {
            Segment ret = null;

            var flags = bytes[0];

            if (CheckFlag(flags, Type.SYN))
            {
                ret = new SynchronousSegment();
            }
            else if (CheckFlag(flags, Type.NUL))
            {
                ret = new NullSegment();
            }
            else if (CheckFlag(flags, Type.EAK))
            {
                ret = new ExtendAckSegment();
            }
            else if (CheckFlag(flags, Type.RST))
            {
                ret = new ResetSegment();
            }
            else if (CheckFlag(flags, Type.FIN))
            {
                ret = new FinishSegment();
            }
            else if (CheckFlag(flags, Type.ACK))
            {
                //var headerSize = bytes[1];

                if (length == RUDP_HEADER_LEN)
                {
                    // ACK
                    ret = new AcknowledgementSegment();
                }
                else
                {
                    // DAT
                    ret = new DataSegment();
                }
            }


            if (ret != null)
            {
                ret.ReadBytes(bytes, offset, length);
            }
            else {
                throw new ApplicationException();
            }

            return ret;
        }

        /// <summary>
        /// データ書き込み.
        /// </summary>
        /// <param name="wr"></param>
        public virtual void WriteBytes(Stream wr)
        {
            wr.WriteByte((byte)this.Flags);
            wr.WriteByte((byte)this.HeaderLength);
            wr.WriteByte((byte)this.SequenceNumber);
            wr.WriteByte((byte)this.AckNumber);
        }

        /// <summary>
        /// データ読み込み.
        /// </summary>
        /// <param name="bytes"></param>
        /// <param name="offset"></param>
        /// <param name="length"></param>
        public virtual void ReadBytes(byte[] bytes, int offset, int length)
        {
            this.Flags = bytes[offset + 0];
            this.HeaderLength = bytes[offset + 1];
            this.SequenceNumber = bytes[offset + 2];
            this.AckNumber = bytes[offset + 3];
        }

        public override string ToString()
        {
            string ret = this.GetSegmentType()
                + " SEQ:" + this.SequenceNumber
                + " ACK:" + (this.GetAcknowledgedNumber() >= 0 ? "" + this.GetAcknowledgedNumber() : "N/A");
            return ret;
        }
    }
}
