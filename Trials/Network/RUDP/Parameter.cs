using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RUDP
{
    public class Parameter
    {
        static public int MAX_SEND_QUEUE_SIZE = 32;
        static public int MAX_RECV_QUEUE_SIZE = 32;

        static public int MAX_SEGMENT_SIZE = 128;
        static public int MAX_OUTSTANDING_SEGS = 3;
        static public int MAX_RETRANS = 3;
        static public int MAX_CUMULATIVE_ACKS = 3;
        static public int MAX_OUT_OF_SEQUENCE = 3;
        static public int MAX_AUTO_RESET = 3;
        static public int NULL_SEGMENT_TIMEOUT = 2000;
        static public int RETRANSMISSION_TIMEOUT = 600;
        static public int CUMULATIVE_ACK_TIMEOUT = 300;

        public int MaxSendQueueSize { get; private set; }

        public int MaxRecieveQueueSize { get; private set; }

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

        public Parameter()
            : this(
            MAX_SEND_QUEUE_SIZE,
            MAX_RECV_QUEUE_SIZE,
            MAX_SEGMENT_SIZE,
            MAX_OUTSTANDING_SEGS,
            0/*MAX_RETRANS*/,
            MAX_CUMULATIVE_ACKS,
            MAX_OUT_OF_SEQUENCE,
            MAX_AUTO_RESET,
            NULL_SEGMENT_TIMEOUT,
            RETRANSMISSION_TIMEOUT,
            CUMULATIVE_ACK_TIMEOUT)
        {
        }

        public Parameter(
            int maxSendQueueSize,
            int maxRecvQueueSize,
            int maxSegmentSize,
            int maxOutstandingSegs,
            int maxRetrans,
            int maxCumulativeAcks,
            int maxOutOfSequence,
            int maxAutoReset,
            int nullSegmentTimeout,
            int retransmissionTimeout,
            int cumulativeAckTimeout)
        {
            this.MaxSendQueueSize = maxSendQueueSize;
            this.MaxRecieveQueueSize = maxRecvQueueSize;
            this.MaxSegmentSize = maxSegmentSize;
            this.MaxNumberOfOutStandingSegs = maxOutstandingSegs;
            this.MaxRetransmission = maxRetrans;
            this.MaxCumulativeAck = maxCumulativeAcks;
            this.MaxOutOfSequence = maxOutOfSequence;
            this.MaxAutoReset = maxAutoReset;
            this.NullSegmentTimeout = nullSegmentTimeout;
            this.RetransmissionTimeout = retransmissionTimeout;
            this.CumulativeAckTimeout = cumulativeAckTimeout;
        }
    }
}
