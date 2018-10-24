#include "frame.hpp"
#include "ack.hpp"
#include <vector>

void cleanBuffer(char* buffer, long length){
    int i;
    i = 0;
    while (i < length){
        buffer[i] = '\0';
        i++;
    }
}

int dataLengthInBuffer(char* buffer){
    int i;
    bool f = false;
    i = 0;
    while ((i < length) && (f == false)){
        if (buffer[i] != '\0'){
            f = true;
        } else{
            i++;
        }
    }
    return i+1;
}
vector<frame> makeFrames(char*buffer, int buffer_size){
    char nowChar = buffer[0]; // buffer character that is read now
    int i = 0; //index 
    int counter = 0;
    char* dataInFrame; //data that want to be inserted in frame
    int dataLength;
    const int MAX_DATA_LENGTH = 1024;
    int seqNum = 0;
    frame f;

    vector<frame> v;
    while  ((nowChar != '\0') && (counter <= buffer_size)){ // while still in buffer
        seqNum++;
        dataLength = 1;
        cleanBuffer(dataInFrame,MAX_DATA_LENGTH);
        while ((nowChar != '\0') && (i < MAX_DATA_LENGTH) && (i < buffer_size)){
            dataInFrame[i] = buffer[i];
            dataLength++;
            i++;
        }
        // nowChar == '\0' or i > MAX_DATA LENGTH or i > buffer_size
        f = new frame(0x1, seqNum, dataLength, dataInFrame, generateChecksSum(dataInFrame));
        v.push_back(f);
        counter = i;
    }
    return v;
}

unsigned char generateChecksSum(char* data, int dataLength){
    unsigned char checkS = 0;
    int i = 0;
    while (i < dataLength){
        checkS += data[i];
    }
    return checkS;
}