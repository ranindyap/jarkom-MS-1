#include <iostream>
using namespace std;
#ifndef ACK_HPP
#define ACK_HPP

#ifndef ACK_MAX_LENGTH
#define ACK_MAX_LENGTH 6
#endif
class ack{
    private:
        char idxAck;
        int nextSeqNum;
        char checkSum;
    public:
        //construct
        ack(char i, int nS, char c);
        ack();
        //getter
        char getIdxAck();
        int getNextSeqNum();
        char getCheckSum();

        //setter
        void setIdxAck(char i);
        void setNextSeqNum(int nS);
        void setCheckSum(char c);

        char* toChars();
};

#endif