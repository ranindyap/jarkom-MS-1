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

void cleanBuffer(char* buffer, long length){
    int i;
    i = 0;
    while (i < length){
        buffer[i] = '\0';
        i++;
    }
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
vector<frame> makeFrames(char*buffer, int buffer_size, int &lastSeqNum, ifstream file){\
    const int MAX_DATA_LENGTH = 1024;
    char nowChar = buffer[0]; // buffer character that is read now
    int i = 0; //index 
    int counter = 0;
    char dataInFrame[MAX_DATA_LENGTH]; //data that want to be inserted in frame
    int dataLength;
    vector<frame> v, frames;

    if (buffer_size <= MAX_DATA_LENGTH){
        cleanBuffer(buffer, buffer_size);
        file.read(buffer, buffer_size);
        while (counter != 0){
            i = 0;   
            while (i < MAX_DATA_LENGTH){
                if (counter < buffer_size){
                    dataInFrame[i] = buffer[counter];
                    i++;
                    counter++;
                } else{
                    counter = 0;
                    if (file){
                        cleanBuffer(buffer, buffer_size);
                        file.read(buffer, buffer_size) ;
                    }else{
                        i = MAX_DATA_LENGTH;
                    }
                }
            }
            frame f(0x1, lastSeqNum, dataLength, dataInFrame, generateCheckSum(dataInFrame, dataLength));
            v.push_back(f);
        }
    } else {
        cleanBuffer(buffer, buffer_size);
        file.read(buffer, buffer_size); 
        while  ((nowChar != '\0') && (counter < buffer_size)){ // while still in buffer
            lastSeqNum++;
            dataLength = 1;
            cleanBuffer(dataInFrame,MAX_DATA_LENGTH);
            i = 0;
            while ((nowChar != '\0') && (i < MAX_DATA_LENGTH) && (counter < buffer_size)){
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
        frames.insert(frames.end(),v.begin(),v.end());
    }
    return frames;
}
void printVectorFrame(vector<frame> f){
    vector<frame>::iterator it;
    int i =0;
    for (it = f.begin(); it != f.end();it++){
        f.at(i).printData();
        i++;
    }
}

int initWinSock(WSADATA w){ // Inisialisasi Winsock
    return WSAStartup(MAKEWORD(2,2), &w);
}

struct addrinfo makeAddressFromInfo(){
    struct addrinfo addressInfoHints;
    ZeroMemory(&addressInfoHints, sizeof (addressInfoHints));
    addressInfoHints.ai_family = AF_INET;
    addressInfoHints.ai_socktype = SOCK_DGRAM;
    addressInfoHints.ai_protocol = IPPROTO_UDP;
    addressInfoHints.ai_flags = AI_PASSIVE;
    return addressInfoHints;
}
void sendFrames(vector<frame> f, char* ipAddress, char* port){
    struct sockaddr_in sender_address;
    int initResult;
    int sender_socket, slen=sizeof(sender_address);
    char buf[BUF_MAX_LENGTH];
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
    sender_address.sin_port = htons(DEFAULT_RECEIVER_PORT);
    sender_address.sin_addr.S_un.S_addr = inet_addr(RECEIVER);

    //start communication
    while(1)
    {
        cout << "Enter message : ";
        gets(message);
         
        //send the message
        if (sendto(sender_socket, message, strlen(message) , 0 , (struct sockaddr *) &sender_address, slen) == SOCKET_ERROR)
        {
            cout << "Sending from sender failed with code : " << WSAGetLastError() << endl;
            exit(EXIT_FAILURE);
        } else{
            cout << "Sending from sender successful" << endl;
        }
         
        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', BUF_MAX_LENGTH);
        //try to receive some data, this is a blocking call
        if (recvfrom(sender_socket, buf, BUF_MAX_LENGTH, 0, (struct sockaddr *) &sender_address, &slen) == SOCKET_ERROR)
        {
            cout << "sender receiving failed with code : " << WSAGetLastError() << endl;
            exit(EXIT_FAILURE);
        } else{
            cout << "sender receiving successful" << endl;
        }
         
        puts(buf);
    }
 
    closesocket(sender_socket);
    WSACleanup();
}

