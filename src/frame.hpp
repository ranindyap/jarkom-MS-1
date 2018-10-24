#ifndef FRAME_HPP
#define FRAME_HPP
#include <string.h>
#include <stdio.h>
#include <iostream>
class frame{
    private:
        char SOH;
        int seqNum;
        int dataLength;
        char* data;
        unsigned char checksum;
    public:
        //construct
        frame( char SOH,int seqNum,int dataLength,char* data, unsigned char checksum);
        //getter
        char getSOH();
        int getSeqNum();
        int getDataLength();
        char* getData();
        unsigned char getCheckSum();

        //setter
        void setSOH(char SOH);
        void setSeqNum(int SeqNum);
        void setDataLength(int DataLength);
        void setData(char* Data);
        void setCheckSum(unsigned char CheckSum);

        //other 
        void printData();
};

#endif