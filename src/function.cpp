#include "function.hpp"

bool findAck(int seqNum, vector<ack> ackV){
    vector<ack>::iterator it;
    int i = 0;
    bool found = false;
    for (it = ackV.begin(); it != ackV.end();it++){
        if (ackV.at(i).getNextSeqNum()-1 == seqNum){
            found = true;
        }
        i++;
    }
    return found;
}

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
unsigned Char4ToInt(char* number){
    return (number[3] << 24) | (number[2] << 16) | (number[1] << 8) | (number[0]);
}
void cleanBuffer(char* buffer, long length){
    int i;
    i = 0;
    while (i < length){
        buffer[i] = '\0';
        i++;
    }
}

int lengthDataInBuffer(char* buffer){
    int i = 0;
    while (buffer[i] != '\0'){
        i++;
    }
    return i;
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

vector<frame> makeFrames(char*buffer, int buffer_size, int &lastSeqNum, char* filename){
    const int MAX_DATA_LENGTH = 1024;
    char nowChar = buffer[0]; // buffer character that is read now
    int i = 0; //index 
    int counter = -1;
    char dataInFrame[MAX_DATA_LENGTH]; //data that want to be inserted in frame
    int dataLength;
    vector<frame> v, frames;
    ifstream file(filename);
    cleanBuffer(dataInFrame,MAX_DATA_LENGTH);
    if (buffer_size <= MAX_DATA_LENGTH){
        cleanBuffer(buffer, buffer_size);
        file.read(buffer, buffer_size);
        while (counter != 0){
            if (counter == -1){
                counter++;
            }
            i = 0;   
            while (i < MAX_DATA_LENGTH){
                if ((counter < buffer_size) && (buffer[counter] != '\0')){
                    dataInFrame[i] = buffer[counter];
                    i++;
                    counter++;
                } else{
                    counter = 0;
                    if (file){
                        cleanBuffer(buffer, buffer_size);
                        file.read(buffer, buffer_size) ;
                    }else{
                        break;
                    }
                }
            }
            dataLength = i;
            lastSeqNum++;
            frame f(0x1, lastSeqNum, dataLength, dataInFrame, generateCheckSum(dataInFrame, dataLength));
            v.push_back(f);
            cleanBuffer(dataInFrame, MAX_DATA_LENGTH);
        }
    } else {
        while(file){
            cleanBuffer(buffer, buffer_size);
            file.read(buffer, buffer_size); 
            counter = 0;
            while  ((buffer[counter] != '\0') && (counter < buffer_size)){ // while still in buffer
                lastSeqNum++;
                dataLength = 1;
                i = 0;
                cleanBuffer(dataInFrame, MAX_DATA_LENGTH);
                while ((buffer[counter] != '\0') && (i < MAX_DATA_LENGTH)){
                    dataInFrame[i] = buffer[counter];
                    i++;
                    counter++;
                }

                if (buffer[counter] == '\0' || counter > buffer_size) {
                    cleanBuffer(buffer, buffer_size);
                    file.read(buffer, buffer_size);
                    counter = 0;
                    while ((buffer[counter] != '\0') && (i < MAX_DATA_LENGTH)) {
                        dataInFrame[i] = buffer[counter];
                        i++;
                        counter++;  
                    }
                }
                
                dataLength = i;

                frame f(0x1, lastSeqNum, dataLength, dataInFrame, generateCheckSum(dataInFrame, dataLength));
                v.push_back(f);
            }
        }
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
void printVectorFrameProperties(vector<frame> f){
    vector<frame>::iterator it;
    int i =0;
    for (it = f.begin(); it != f.end();it++){
        f.at(i).printProperties();
        i++;
    }
}

int initWinSock(WSADATA w){ // Inisialisasi Winsock
    return WSAStartup(MAKEWORD(2,2), &w);
}

ack parseToAck(char* buff){
    char stringOfNextSeqNum[5];
    memcpy(stringOfNextSeqNum, &buff[1],4);
    stringOfNextSeqNum[4] = '\0';
    char SOH = buff[0];
    int nextSeqNum = Char4ToInt(stringOfNextSeqNum);
    char checkSum = buff[5];
    ack receivedAck(SOH, nextSeqNum, checkSum);
    return receivedAck;
}

frame parseToFrame(char * buff){
    char SOH = buff[0];

    char stringOfSeqNum[5];
    memcpy(stringOfSeqNum, &buff[1],4);
    stringOfSeqNum[4] = '\0';
    int seqNum = Char4ToInt(stringOfSeqNum);

    char stringOfDataLength[5];
    memcpy(stringOfDataLength, &buff[5],4);
    stringOfDataLength[4] = '\0';
    unsigned int dataLength = Char4ToInt(stringOfDataLength);

    char stringOfData[1025];
    memcpy(stringOfData, &buff[9],1024);
    stringOfData[1024] = '\0';

    char checkSum = buff[1033];

    frame receivedFrame(SOH,seqNum,dataLength,stringOfData,checkSum);
    return receivedFrame;
}

ack makeAck(frame f){
    char SOH = f.getSOH();
    int nextSeqNum = f.getSeqNum()+1;
    char checkSum = generateCheckSum(f.getData(), f.getDataLength());
    return ack(SOH,nextSeqNum,checkSum);
}
