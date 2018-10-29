#include "ack.hpp"

//construct
ack::ack(){
    this->idxAck = '\0';
    this->nextSeqNum = -1;
    this->checkSum = '\0';
}
ack::ack(char i, int nS, char c){
    this->idxAck = i;
    this->nextSeqNum = nS;
    this->checkSum = c;
}


//getter
char ack::getIdxAck(){
    return this->idxAck;
}
int ack::getNextSeqNum(){
    return this->nextSeqNum;
}
char ack::getCheckSum(){
    return this->checkSum;
}

//setter
void ack::setIdxAck(char i){
    this->idxAck = i;
}
void ack::setNextSeqNum(int nS){
    this->nextSeqNum = nS;
}
void ack::setCheckSum(char c){
     this->checkSum = c;
}

char* ack::toChars(){
    char* buffer = new char[7];
    buffer[0] = this->idxAck;
    buffer[4] = this->nextSeqNum>>24;
    buffer[3] = this->nextSeqNum>>16;
    buffer[2] = this->nextSeqNum>>8;
    buffer[1] = this->nextSeqNum;
    buffer[5] = this->checkSum;
    buffer[6] = '\0';
    return buffer;
}