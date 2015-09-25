using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/*
 *  SYNchronous Segment
 *
 *   0             7 8             15
 *  +-+-+-+-+-+-+-+-+---------------+
 *  | |A| | | | | | |               |
 *  |1|C|0|0|0|0|0|0|       22      |
 *  | |K| | | | | | |               |
 *  +-+-+-+-+-+-+-+-+---------------+
 *  |  Sequence #   +   Ack Number  |
 *  +---------------+---------------+
 *  | Vers  | Spare | Max # of Out  |
 *  |       |       | standing Segs |
 *  +---------------+---------------+
 *  | Option Flags  |     Spare     |
 *  +---------------+---------------+
 *  |     Maximum Segment Size      |
 *  +---------------+---------------+
 *  | Retransmission Timeout Value  |
 *  +---------------+---------------+
 *  | Cumulative Ack Timeout Value  |
 *  +---------------+---------------+
 *  |   Null Segment Timeout Value  |
 *  +---------------+---------------+
 *  |  Max Retrans  | Max Cum Ack   |
 *  +---------------+---------------+
 *  | Max Out of Seq| Max Auto Reset|
 *  +---------------+---------------+
 *  |           Checksum            |
 *  +---------------+---------------+
 *
 */

namespace RUDP
{
    class SynchronousSegment : Segment
    {
        public override string GetSegmentType()
        {
 	        return "SYN";
        }

        private static int SYN_HEADER_LEN = RUDP_HEADER_LEN + 16;

        public int Version { get; private set; }

        public int MaxNumberOfOutStandingSegs { get; private set; }

        public int OptionFlags { get; private set; }

        public int MaxSegmentSize { get; private set; }

        public int RetransmissionTimeout { get; private set; }

        public int CumulativeAckTimeout { get; private set; }

        public int NullSegmentTimeout { get; private set; }

        public int MaxRetransmission { get; private set; }

        public int MaxCumulativeAck { get; private set; }

        public int MaxOutOfSequence { get; private set; }

        public int MaxAutoReset { get; private set; }

        public SynchronousSegment()
        {
        }

        public SynchronousSegment(
            int sequenceNumber,
            int maxNumberOfOutStandingSegs,
            int maxSegmentSize,
            int retransmissionTimeout,
            int cumulativeAckTimeout,
            int nullSegmentTimeout,
            int maxRetransmission,
            int maxCumulativeAck,
            int maxOutOfSequence,
            int maxAutoReset)
            : base(Type.SYN, sequenceNumber, SYN_HEADER_LEN)
        {
            this.Version = RUDP_VERSION;

            this.MaxNumberOfOutStandingSegs = maxNumberOfOutStandingSegs;
            this.OptionFlags = 0x01;    // no options.
            this.MaxSegmentSize = maxSegmentSize;
            this.RetransmissionTimeout = retransmissionTimeout;
            this.CumulativeAckTimeout = cumulativeAckTimeout;
            this.NullSegmentTimeout = nullSegmentTimeout;
            this.MaxRetransmission = maxRetransmission;
            this.MaxCumulativeAck = maxCumulativeAck;
            this.MaxOutOfSequence = maxOutOfSequence;
            this.MaxAutoReset = maxAutoReset;
        }

        public SynchronousSegment(int sequenceNumber,  Parameter parameter)
            : this(sequenceNumber, 
            parameter.MaxNumberOfOutStandingSegs,
            parameter.MaxSegmentSize,
            parameter.RetransmissionTimeout,
            parameter.CumulativeAckTimeout,
            parameter.NullSegmentTimeout,
            parameter.MaxRetransmission,
            parameter.MaxCumulativeAck,
            parameter.MaxOutOfSequence,
            parameter.MaxAutoReset)
        {
        }

        public override void WriteBytes(Stream wr)
        {
            base.WriteBytes(wr);

            wr.WriteByte((byte)((this.Version << 4) & 0xff));
            wr.WriteByte((byte)this.MaxNumberOfOutStandingSegs);
            wr.WriteByte((byte)this.OptionFlags);
            
            wr.WriteByte(0);    // spare.

            wr.WriteByte((byte)((this.MaxSegmentSize >> 8) & 0xff));
            wr.WriteByte((byte)((this.MaxSegmentSize >> 0) & 0xff));

            wr.WriteByte((byte)((this.RetransmissionTimeout >> 8) & 0xff));
            wr.WriteByte((byte)((this.RetransmissionTimeout >> 0) & 0xff));

            wr.WriteByte((byte)((this.CumulativeAckTimeout >> 8) & 0xff));
            wr.WriteByte((byte)((this.CumulativeAckTimeout >> 0) & 0xff));

            wr.WriteByte((byte)((this.NullSegmentTimeout >> 8) & 0xff));
            wr.WriteByte((byte)((this.NullSegmentTimeout >> 0) & 0xff));

            wr.WriteByte((byte)this.MaxRetransmission);
            wr.WriteByte((byte)this.MaxCumulativeAck);
            wr.WriteByte((byte)this.MaxOutOfSequence);
            wr.WriteByte((byte)this.MaxAutoReset);

            // For CheckSum
            wr.WriteByte(0);
            wr.WriteByte(0);
        }

        public override void ReadBytes(byte[] bytes, int offset, int length)
        {
            base.ReadBytes(bytes, offset, length);

            if (!CheckFlag(Type.SYN))
            {
                throw new ArgumentOutOfRangeException();
            }

            if (HeaderLength != SYN_HEADER_LEN)
            {
                throw new ArgumentOutOfRangeException();
            }

            this.Version = (bytes[offset + 4] >> 4);
            if (this.Version != RUDP_VERSION)
            {
                throw new ArgumentOutOfRangeException();
            }

            this.MaxNumberOfOutStandingSegs = bytes[offset + 5];
            this.OptionFlags = bytes[offset + 6];
            
            var spare = bytes[offset + 7];

            this.MaxSegmentSize = ((bytes[offset + 8] << 8) | bytes[offset + 9]);
            this.RetransmissionTimeout = ((bytes[offset + 10] << 8) | bytes[offset + 11]);
            this.CumulativeAckTimeout = ((bytes[offset + 12] << 8) | bytes[offset + 13]);
            this.NullSegmentTimeout = ((bytes[offset + 14] << 8) | bytes[offset + 15]);

            this.MaxRetransmission = bytes[offset + 16];
            this.MaxCumulativeAck = bytes[offset + 17];
            this.MaxOutOfSequence = bytes[offset + 18];
            this.MaxAutoReset = bytes[offset + 19];
        }
    }
}
