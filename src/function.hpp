#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "frame.hpp"
#include "ack.hpp"
#include <vector>
#include <fstream>
using namespace std;

void cleanBuffer(char* buffer, long length);
int32_t Char4ToInt(char* number);
ack parseToAck(char* buff);
frame parseToFrame(char * buff);
int dataLengthInBuffer(char* buffer, int length);
vector<frame> makeFrames(char*buffer, int length, int &lastSeqNum, char* filename);
ack makeAck(frame f);
unsigned char generateCheckSum(char* data, int dataLength);
void printVectorFrame(vector<frame> f);
void printVectorFrameProperties(vector<frame> f);
void sendFrames(vector<frame> f, char* ipAddress, unsigned short port);
int initWinSock(WSADATA w);
struct addrinfo makeAddressFromInfo();
int lengthDataInBuffer(char* buffer);

#endif