#ifndef DATATYPE_HPP
#define DATATYPE_HPP

struct frame{
    char SOH;
    int seqNum;
    int dataLength;
    char* data;
    char checksum;
};

struct ack{
    char idxAck;
    int nextSeqNum;
    char checksum;
};

#endif