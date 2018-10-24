#include "ack.hpp"
//construct
ack::ack(char i, int nS, char c){
    this.idxAck = i;
    this.nextSeqNum = nS;
    this.checkSum = c;
}

//getter
char ack::getIdxAck(){
    return this.idxAck;
}
int ack::getNextSeqNum(){
    return this.nextSeqNum;
}
char ack::getCheckSum(){
    return this.checkSum;
}

//setter
void ack::setIdxAck(char i){
    this.idxAck = i;
}
void ack::setNextSeqNum(int nS){
    this.nextSeqNum = nS;
}
void ack::setCheckSum(char c){
     this.checkSum = c;
}