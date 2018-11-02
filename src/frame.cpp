#include "frame.hpp"
//construct
frame::frame(){
    this->SOH = '\0';
    this->seqNum = -1;
    this->dataLength = 0;
    this->data = new char[1024];
    this->checksum = '\0';
}
frame::frame( char SOH,int seqNum,int dataLength,char* data, unsigned char checksum){
    // std::cout <<"MASUK";
    this->SOH = SOH;
    this->seqNum = seqNum;
    this->dataLength = dataLength;
    this->data = new char[strlen(data)+1];
    strcpy(this->data,data);
    this->checksum = checksum;
}
//getter
char frame::getSOH(){
    return this->SOH;
}
int frame::getSeqNum() const {
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
    this->data = new char[strlen(Data)+1];
    strcpy(this->data,Data);
}
void frame::setCheckSum(unsigned char CheckSum){
    this->checksum = CheckSum;
}

//other
void frame::printData(){
    std::cout << this->getSeqNum() << " " << this->getDataLength() << std::endl << this->getData() << std::endl;
}

void frame::printProperties(){
     std::cout << this->getSeqNum() << " " << this->getDataLength() << std::endl;
}

char* frame::toChars(){
    char* buffer = new char[1035];
    buffer[0] = this->SOH;
    buffer[4] = this->seqNum>>24;
    buffer[3] = this->seqNum>>16;
    buffer[2] = this->seqNum>>8;
    buffer[1] = this->seqNum;
    buffer[8] = this->dataLength>>24 & 0xFF;
    buffer[7] = this->dataLength>>16 & 0xFF;
    buffer[6] = this->dataLength>>8 & 0xFF;
    buffer[5] = this->dataLength & 0xFF;
    int i = 9;
    int j = 0;
    for (j = 0; j < 1024; j++){
        buffer[i] = this->data[j];
        i++;
    }
    buffer[1033] = this->checksum;
    buffer[1034] = '\0';
    return buffer;
}