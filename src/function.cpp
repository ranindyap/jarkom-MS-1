#include "function.hpp"

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
int32_t Char4ToInt(char* number){
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
vector<frame> makeFrames(char*buffer, int buffer_size, int &lastSeqNum, char* filename){\
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
            cleanBuffer(dataInFrame,MAX_DATA_LENGTH);
        }
    } else {
        while(file){
            cleanBuffer(buffer, buffer_size);
            file.read(buffer, buffer_size); 
            counter = 0;
            while  ((buffer[counter] != '\0') && (counter < buffer_size)){ // while still in buffer
                lastSeqNum++;
                dataLength = 1;
                cleanBuffer(dataInFrame,MAX_DATA_LENGTH);
                i = 0;
                while ((buffer[counter] != '\0') && (i < MAX_DATA_LENGTH)){
                    dataInFrame[i] = buffer[counter];
                    i++;
                    counter++;
                }
                dataLength = i;
                // cout << lastSeqNum << " " << dataInFrame;
                // nowChar == '\0' or i > MAX_DATA LENGTH or i > buffer_size
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

void sendFrames(vector<frame> frames, char* ipAddress, unsigned short port){
    struct sockaddr_in sender_address;
    int initResult;
    char* frame_buff;
    ack realAck;
    int sender_socket, slen=sizeof(sender_address);
    char buf[BUF_MAX_LENGTH], ackString[ACK_MAX_LENGTH+5];
    char message[BUF_MAX_LENGTH];
    WSADATA wsa;

     //Initialize Winsock
    initResult = initWinSock(wsa);
    if (initResult != 0){
        cout << "Initialize sender WinSock failed with code: " << WSAGetLastError()<< endl;
        exit(EXIT_FAILURE); 
    } else{
        cout << "Initialize sender WinSock successful" << endl;
    }
    //Create socket
    sender_socket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (sender_socket == INVALID_SOCKET){
        cout << "Creating sender Socket failed with code: " << WSAGetLastError()<< endl;
        exit(EXIT_FAILURE); 
    } else{
        cout << "Creating sender Socket successful" << endl;
    }

    //set sender_address
    memset((char *) &sender_address, 0, sizeof(sender_address));
    sender_address.sin_family = AF_INET;
    sender_address.sin_port = htons(port);
    sender_address.sin_addr.S_un.S_addr = inet_addr(ipAddress);

    vector<frame>::iterator it;
    int i =0;
    for (it = frames.begin(); it != frames.end();it++){
        //sending
        frame_buff = frames.at(i).toChars();
        // cout << "HERE";
        if (sendto(sender_socket, frame_buff, sizeof(frame_buff) , 0 , (struct sockaddr *) &sender_address, slen) == SOCKET_ERROR)
        {
            cout << "Sending from sender failed with code : " << WSAGetLastError() << endl;
            exit(EXIT_FAILURE);
        } else{
            cout << "Sending from sender frame number " << frames.at(i).getSeqNum() << " success" << endl;
        }

        //receive ack
        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(ackString,'\0', ACK_MAX_LENGTH+5);
        //try to receive some data, this is a blocking call
        if (recvfrom(sender_socket, ackString, ACK_MAX_LENGTH+5, 0, (struct sockaddr *) &sender_address, &slen) == SOCKET_ERROR)
        {
            cout << "sender receiving ack failed with code : " << WSAGetLastError() << endl;
            exit(EXIT_FAILURE);
        } else{
            cout << "sender receiving ack successful" << endl;
            realAck = parseToAck(ackString);
            cout << "Ack for frame "<< realAck.getNextSeqNum() - 1 << " received" << endl;
        }
        i++;
    }
    // //start communication
    // while(1)
    // {
    //     // cout << "Enter message : ";
    //     // gets(message);
    //     //send the message
    //     if (sendto(sender_socket, message, strlen(message) , 0 , (struct sockaddr *) &sender_address, slen) == SOCKET_ERROR)
    //     {
    //         cout << "Sending from sender failed with code : " << WSAGetLastError() << endl;
    //         exit(EXIT_FAILURE);
    //     } else{
    //         cout << "Sending from sender successful" << endl;
    //     }
         
    //     //receive a reply and print it
    //     //clear the buffer by filling null, it might have previously received data
    //     memset(buf,'\0', BUF_MAX_LENGTH);
    //     //try to receive some data, this is a blocking call
    //     if (recvfrom(sender_socket, buf, BUF_MAX_LENGTH, 0, (struct sockaddr *) &sender_address, &slen) == SOCKET_ERROR)
    //     {
    //         cout << "sender receiving failed with code : " << WSAGetLastError() << endl;
    //         exit(EXIT_FAILURE);
    //     } else{
    //         cout << "sender receiving successful" << endl;
    //     }
         
    //     puts(buf);
    // }
    if (sendto(sender_socket, FINISH_MESSAGE, sizeof(FINISH_MESSAGE) , 0 , (struct sockaddr *) &sender_address, slen) == SOCKET_ERROR)
    {
        cout << "Sending FINISH from sender failed with code : " << WSAGetLastError() << endl;
        exit(EXIT_FAILURE);
    } else{
        cout << "Sending FINISH from sender success" << endl;
    }
    closesocket(sender_socket);
    WSACleanup();
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
    int dataLength = Char4ToInt(stringOfSeqNum);

    char stringOfData[1025];
    memcpy(stringOfData, &buff[9],1024);
    stringOfData[1024] = '\0';

    char checkSum = buff[1024];

    frame receivedFrame(SOH,seqNum,dataLength,stringOfData,checkSum);
    return receivedFrame;
}

ack makeAck(frame f){
    char SOH = f.getSOH();
    int nextSeqNum = f.getSeqNum()+1;
    char checkSum = generateCheckSum(f.getData(), f.getDataLength());
    return ack(SOH,nextSeqNum,checkSum);
}