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
#include <thread>
#include <winsock2.h> 
#include <Ws2tcpip.h>
#include <windows.h>
#include <stdlib.h> 

#ifndef DEFAULT_RECEIVER_PORT  
#define DEFAULT_RECEIVER_PORT 8080
#endif
#ifndef TIME_OUT  
#define TIME_OUT 3000
#endif
#ifndef RECEIVER
#define RECEIVER "127.0.0.1"  //ip address of udp server
#endif
#ifndef BUF_MAX_LENGTH
#define BUF_MAX_LENGTH 1024  //Max length of buffer
#endif
#ifndef FRAME_LENGTH
#define FRAME_LENGTH 1034
#endif
#ifndef FINISH_MESSAGE
#define FINISH_MESSAGE "FINISH"
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
        frame();
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
        void printProperties();

        char* toChars();
};

#endif