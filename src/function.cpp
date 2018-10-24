#include "function.hpp"

unsigned char generateCheckSum(char* data, int dataLength){
    unsigned int Sum = 0;
    int i = 0;
    while ((i < dataLength) && (data[i] != '\0')){
        Sum += data[i];
        i++;
    }
    Sum = -Sum;
    return (Sum & 0xFF);
}

void cleanBuffer(char* buffer, long length){
    int i;
    i = 0;
    while (i < length){
        buffer[i] = '\0';
        i++;
    }
}

int dataLengthInBuffer(char* buffer, int buffer_length){
    int i;
    bool f = false;
    i = 0;
    while ((i < buffer_length) && (f == false)){
        if (buffer[i] != '\0'){
            f = true;
        } else{
            i++;
        }
    }
    return i+1;
}
vector<frame> makeFrames(char*buffer, int buffer_size, int &lastSeqNum){\
    const int MAX_DATA_LENGTH = 1024;
    char nowChar = buffer[0]; // buffer character that is read now
    int i = 0; //index 
    int counter = 0;
    char dataInFrame[MAX_DATA_LENGTH]; //data that want to be inserted in frame
    int dataLength;
    vector<frame> v;

    while  ((nowChar != '\0') && (counter < buffer_size)){ // while still in buffer
        lastSeqNum++;
        dataLength = 1;
        cleanBuffer(dataInFrame,MAX_DATA_LENGTH);
        while ((nowChar != '\0') && (i < MAX_DATA_LENGTH) && (i < buffer_size)){
            dataInFrame[i] = buffer[i];
            i++;
        }
        dataLength = i;
        // cout << lastSeqNum << " " << dataInFrame;
        // nowChar == '\0' or i > MAX_DATA LENGTH or i > buffer_size
        frame f(0x1, lastSeqNum, dataLength, dataInFrame, generateCheckSum(dataInFrame, dataLength));
        v.push_back(f);
        counter = i;
    }
    return v;
}
void printVectorFrame(vector<frame> f){
    vector<frame>::iterator it;
    int i =0;
    for (it = f.begin(); it != f.end();it++){
        f.at(i).printData();
        i++;
    }
}
