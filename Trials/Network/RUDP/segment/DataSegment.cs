using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/*
 *  DATa Segment
 *
 *   0 1 2 3 4 5 6 7 8            15
 *  +-+-+-+-+-+-+-+-+---------------+
 *  |0|1|0|0|0|0|0|0|       6       |
 *  +-+-+-+-+-+-+-+-+---------------+
 *  | Sequence #    |   Ack Number  |
 *  +---------------+---------------+
 *  |           Checksum            |
 *  +---------------+---------------+
 *  | ...                           |
 *  +-------------------------------+
 *
 */
namespace RUDP
{
    class DataSegment : Segment
    {
        public override string GetSegmentType()
        {
            return "DAT";
        }

        public byte[] Data
        {
            get;
            private set;
        }

        public DataSegment()
        {
        }

        public DataSegment(
            int sequenceNumber,
            int acknowledgedNumber,
            byte[] bytes,
            int offset,
            int length)
            : base(Type.ACK, sequenceNumber, RUDP_HEADER_LEN)
        {
            SetAcknowledgedNumber(acknowledgedNumber);

            this.Data = new byte[length];
            System.Array.Copy(bytes, offset, this.Data, 0, length);
        }

        public override void WriteBytes(Stream wr)
        {
            base.WriteBytes(wr);

            // For CheckSum
            wr.WriteByte(0);
            wr.WriteByte(0);

            wr.Write(this.Data, 0, this.Data.Length);
        }

        public override void ReadBytes(byte[] bytes, int offset, int length)
        {
            base.ReadBytes(bytes, offset, length);

            if (!CheckFlag(Type.ACK))
            {
                throw new ArgumentOutOfRangeException();
            }

            var dataLength = bytes.Length - RUDP_HEADER_LEN;
            this.Data = new byte[dataLength];

            System.Array.Copy(
                bytes,
                offset + RUDP_HEADER_LEN,
                this.Data,
                0,
                this.Data.Length);
        }
    }
}
