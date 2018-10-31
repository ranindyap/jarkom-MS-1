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
void sendFrames(vector<frame> f, char* ipAddress, unsigned short port);
int initWinSock(WSADATA w);
struct addrinfo makeAddressFromInfo();
int lengthDataInBuffer(char* buffer);
// void sendMessage(vector<frame> frames, int buffer_start, int buffer_size, int window_size, int sender_socket, struct sockaddr_in sender_address ) ;
// void receiveMessage(int vectorLength, int sender_socket, struct sockaddr_in sender_address);
// void sendMultiThread(vector<frame> frames, int buffer_size, int window_size, char* ipAddress, unsigned short port);
bool findAck(int seqNum, vector<ack> ackV);
#endif