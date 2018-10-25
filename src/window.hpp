#ifndef WINDOW_HPP
#define WINDOW_HPP
#include <string.h>
#include "frame.hpp"
#include <iostream>
using namespace std;

class FrameTimer{
    private:
        int frameSeqNum;
        int timer;
        int timeout;

    public:
        FrameTimer(int frameSeqNum, int timeout);
        //getter
        int getFrameSeqNum();
        int getFrameTimer();

        //setter
        void setFrameSeqNum(int frameSeqNum);

        //other
        void addTimer();
        void printFrameTimer();
};

class window {
    private:
        int size;
        int timeout;
        vector<FrameTimer> arrFrameTimer;

    public:
        //construct
        window(int size, vector<frame> listOfFrame, int startSeqNum, int timeout);

        //getter
        int getWindowSize();
        int getTimeout();
        FrameTimer getArrFrameTimer(int idx);

        //setter
        void setWindowSize(int size);
        void setTimeout(int timeout);
        void addArrFrameTimer(FrameTimer frametimer);

        //other 
        void printWindow();
};

#endif