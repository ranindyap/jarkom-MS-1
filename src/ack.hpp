#ifndef ACK_HPP
#define ACK_HPP

class ack{
    private:
        char idxAck;
        int nextSeqNum;
        char checksum;
    public:
        //construct
        ack(char i, int nS, char c);

        //getter
        char getIdxAck();
        int getNextSeqNum();
        char getCheckSum();

        //setter
        void setIdxAck(char i);
        void setNextSeqNum(int nS);
        void setCheckSum(char c);
};

#endif