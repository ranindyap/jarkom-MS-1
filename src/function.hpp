#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "frame.hpp"
#include "ack.hpp"
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

void cleanBuffer(char* buffer, long length);
int dataLengthInBuffer(char* buffer, int length);
vector<frame> makeFrames(char*buffer, int length);
unsigned char generateCheckSum(char* data, int dataLength);
#endif