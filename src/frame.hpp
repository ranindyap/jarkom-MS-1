#ifndef FRAME_HPP
#define FRAME_HPP

#include <w32api.h>
#ifndef WINVER
#define WINVER                  WindowsVista
#endif
#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS          WindowsVista
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT            WindowsVista
#endif
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h> 
#include <stdio.h> 
#include <winsock2.h> 
#include <Ws2tcpip.h>
#include <windows.h>
#include <stdlib.h> 

#ifndef DEFAULT_SERVER_PORT
#define DEFAULT_SERVER_PORT "8080"
#endif

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