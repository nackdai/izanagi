using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/*
 *  EACK Segment
 *
 *   0 1 2 3 4 5 6 7 8            15
 *  +-+-+-+-+-+-+-+-+---------------+
 *  |0|1|1|0|0|0|0|0|     N + 6     |
 *  +-+-+-+-+-+-+-+-+---------------+
 *  | Sequence #    |   Ack Number  |
 *  +---------------+---------------+
 *  |1st out of seq |2nd out of seq |
 *  |  ack number   |   ack number  |
 *  +---------------+---------------+
 *  |  . . .        |Nth out of seq |
 *  |               |   ack number  |
 *  +---------------+---------------+
 *  |            Checksum           |
 *  +---------------+---------------+
 *
 */
namespace RUDP
{
    class ExtendAckSegment : Segment
    {
        public override string GetSegmentType()
        {
            return "EAK";
        }

        public byte[] AckNumbers
        {
            get;
            private set;
        }

        public ExtendAckSegment()
        {
        }

        public ExtendAckSegment(
            int sequenceNumber,
            int acknowledgedNumber,
            int[] ackNumbers)
            : base(Type.EAK, sequenceNumber, RUDP_HEADER_LEN + ackNumbers.Length)
        {
            this.AckNumbers = new byte[ackNumbers.Length];
            
            for (int i = 0; i < ackNumbers.Length; i++)
            {
                this.AckNumbers[i] = (byte)ackNumbers[i];
            }
        }

        public override void WriteBytes(Stream wr)
        {
            base.WriteBytes(wr);

            wr.Write(this.AckNumbers, 0, this.AckNumbers.Length);

            // For CheckSum
            wr.WriteByte(0);
            wr.WriteByte(0);
        }

        public override void ReadBytes(byte[] bytes, int offset, int length)
        {
            base.ReadBytes(bytes, offset, length);

            if (!CheckFlag(Type.EAK))
            {
                throw new ArgumentOutOfRangeException();
            }

            var dataLength = bytes.Length - RUDP_HEADER_LEN;
            this.AckNumbers = new byte[dataLength];

            System.Array.Copy(
                bytes,
                offset + RUDP_HEADER_LEN,
                this.AckNumbers,
                0,
                this.AckNumbers.Length);
        }
    }
}
