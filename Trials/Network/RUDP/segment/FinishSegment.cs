using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/*
 *  FIN Segment
 *
 *   0 1 2 3 4 5 6 7 8            15
 *  +-+-+-+-+-+-+-+-+---------------+
 *  | |A| | | | | | |               |
 *  |0|C|0|0|0|0|1|0|        6      |
 *  | |K| | | | | | |               |
 *  +-+-+-+-+-+-+-+-+---------------+
 *  | Sequence #    |   Ack Number  |
 *  +---------------+---------------+
 *  |         Header Checksum       |
 *  +---------------+---------------+
 *
 */
namespace RUDP
{
    class FinishSegment : Segment
    {
        public override string GetSegmentType()
        {
            return "FIN";
        }

        public FinishSegment()
        {
        }

        public FinishSegment(int sequenceNumber)
            : base(Type.FIN, sequenceNumber, RUDP_HEADER_LEN)
        {
        }

        public override void WriteBytes(Stream wr)
        {
            base.WriteBytes(wr);

            // For CheckSum
            wr.WriteByte(0);
            wr.WriteByte(0);
        }

        public override void ReadBytes(byte[] bytes, int offset, int length)
        {
            base.ReadBytes(bytes, offset, length);

            if (!CheckFlag(Type.FIN))
            {
                throw new ArgumentOutOfRangeException();
            }
        }
    }
}
