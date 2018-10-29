#include <vector>
#include <iostream>
#include "window.hpp"
#include "frame.hpp"
using namespace std;

FrameTimer::FrameTimer(int frameSeqNum, int timeout) {
    this->frameSeqNum = frameSeqNum;
    this->timer = 0;
    this->timeout = timeout;
}

//getter
int FrameTimer::getFrameSeqNum() {
    return this->frameSeqNum;
}

int FrameTimer::getFrameTimer() {
    return this->timer;
}

//setter
void FrameTimer::setFrameSeqNum(int frameSeqNum) {
    this->frameSeqNum = frameSeqNum;
}

//other
void FrameTimer::addTimer() {
    this->timer++;
    if (this->timer >= this->timeout) {
        this->timer = 0;
    }
}

void FrameTimer::printFrameTimer() {
    cout << "Frame SeqNum = " << this->frameSeqNum;
    cout << "Frame Timer = " << this->timer;
    cout << "Frame Timeout = " << this->timeout;
}

window::window(int size, vector<frame> listOfFrame, int startSeqNum, int timeout) {
    int lastIdxFrame = listOfFrame.size() - 1;
    int frameIdx = startSeqNum;
    int i = 0;
    while (i < size && i <= lastIdxFrame) {
        int seqNum = listOfFrame[frameIdx].getSeqNum();

        FrameTimer frametimer = FrameTimer(seqNum, timeout);
        this->addArrFrameTimer(frametimer);
        frameIdx++;
        i++;
    }

    this->size = size;
    this->timeout = timeout;
}

//getter
int window::getWindowSize() {
    return this->size;
}

FrameTimer window::getArrFrameTimer(int idx) {
    return this->arrFrameTimer[idx];
}

int window::getTimeout() {
    return this->timeout;
}

//setter
void window::setWindowSize(int size) {
    this->size = size;
}

void window::setTimeout(int timeout) {
    this->timeout = timeout;
}

void window::addArrFrameTimer(FrameTimer frametimer) {
    this->arrFrameTimer.push_back(frametimer);
}
 
void window::printWindow() {
    cout << this->size << endl;
    for (int i=0; i < this->size; i++) {
        this->arrFrameTimer[i].printFrameTimer();
        cout << endl;
    }
}