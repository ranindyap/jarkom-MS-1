#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "frame.hpp"
#include "ack.hpp"
#include <vector>
#include <fstream>
using namespace std;

void cleanBuffer(char* buffer, long length);
unsigned Char4ToInt(char* number);
ack parseToAck(char* buff);
frame parseToFrame(char * buff);
int dataLengthInBuffer(char* buffer, int length);
vector<frame> makeFrames(char*buffer, int length, int &lastSeqNum, char* filename);
ack makeAck(frame f);
unsigned char generateCheckSum(char* data, int dataLength);
void printVectorFrame(vector<frame> f);
void printVectorFrameProperties(vector<frame> f);
int initWinSock(WSADATA w);
int lengthDataInBuffer(char* buffer);
bool findAck(int seqNum, vector<ack> ackV);
#endif