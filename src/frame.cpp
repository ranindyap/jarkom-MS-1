#include "frame.hpp"
//construct
frame::frame( char SOH,int seqNum,int dataLength,char* data, unsigned char checksum){
    this->SOH = SOH;
    this->seqNum = seqNum;
    this->dataLength = dataLength;
    strcpy(this->data,data);
    this->checksum = checksum;
}
//getter
char frame::getSOH(){
    return this->SOH;
}
int frame::getSeqNum(){
    return this->seqNum;
}
int frame::getDataLength(){
    return this->dataLength;
}
char* frame::getData(){
    return this->data;
}
unsigned char frame::getCheckSum(){
    return this->checksum;
}

//setter
void frame::setSOH(char SOH){
    this->SOH = SOH;
}
void frame::setSeqNum(int SeqNum){
    this->seqNum = SeqNum;
}
void frame::setDataLength(int DataLength){
    this->dataLength = DataLength;
}
void frame::setData(char* Data){
    strcpy(this->data,Data);
}
void frame::setCheckSum(unsigned char CheckSum){
    this->checksum = CheckSum;
}
