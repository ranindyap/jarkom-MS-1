#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "frame.hpp"
#include "ack.hpp"
#include <vector>
#include <fstream>
using namespace std;

void cleanBuffer(char* buffer, long length);
int dataLengthInBuffer(char* buffer, int length);
vector<frame> makeFrames(char*buffer, int length, int &lastSeqNum);
unsigned char generateCheckSum(char* data, int dataLength);
void printVectorFrame(vector<frame> f);
#endif