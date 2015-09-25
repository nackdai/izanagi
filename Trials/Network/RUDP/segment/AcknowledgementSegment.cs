using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/*
 *  ACK Segment
 *
 *   0 1 2 3 4 5 6 7 8            15
 *  +-+-+-+-+-+-+-+-+---------------+
 *  |0|1|0|0|0|0|0|0|       6       |
 *  +-+-+-+-+-+-+-+-+---------------+
 *  | Sequence #    |   Ack Number  |
 *  +---------------+---------------+
 *  |           Checksum            |
 *  +---------------+---------------+
 *
 */
namespace RUDP
{
    class AcknowledgementSegment : Segment
    {
        public override string GetSegmentType()
        {
            return "ACK";
        }

        public AcknowledgementSegment()
        {
        }

        public AcknowledgementSegment(
            int sequenceNumber,
            int acknowledgedNumber)
            : base (Type.ACK, sequenceNumber, RUDP_HEADER_LEN)
        {
            SetAcknowledgedNumber(acknowledgedNumber);
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

            if (!CheckFlag(Type.ACK))
            {
                throw new ArgumentOutOfRangeException();
            }
        }
    }
}
