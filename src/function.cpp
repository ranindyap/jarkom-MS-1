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
vector<frame> makeFrames(char*buffer, int buffer_size, int &lastSeqNum){\
    const int MAX_DATA_LENGTH = 1024;
    char nowChar = buffer[0]; // buffer character that is read now
    int i = 0; //index 
    int counter = 0;
    char dataInFrame[MAX_DATA_LENGTH]; //data that want to be inserted in frame
    int dataLength;
    vector<frame> v;

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
    WSADATA wsaData;
    int init = initWinSock(wsaData);
    if (init != 0) {
        cout << "WSAStartup failed: " << init << endl;
    } else{
        cout << "WSAStartup Success" << endl;
    }

    struct addrinfo *result = NULL, *ptr = NULL, addressInfoHints;
    addressInfoHints = makeAddressFromInfo();

    // Resolve the local address and port to be used by the server
    init = getaddrinfo("localhost", DEFAULT_SERVER_PORT, &addressInfoHints, &result);
    if (init != 0) {
        cout << "getaddrinfo failed: %d\n"<< init << endl;
        WSACleanup();
    } else{
        //cout << "getaddrinfo success " << InetNtoA(result->ai_addr) <<endl;
    }

    // Make Socket
    SOCKET server_socket = INVALID_SOCKET;
    server_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (server_socket == INVALID_SOCKET){
        perror("build socket failed"); 
        cout << server_socket;
        exit(EXIT_FAILURE); 
    } else{
        cout << "build socket success" << endl;
    }

    //Binding
    init = bind(server_socket, result->ai_addr, (int)result->ai_addrlen);
    if (init == SOCKET_ERROR) {
        cout << "bind failed with error: " << WSAGetLastError() << endl;
        freeaddrinfo(result);
        closesocket(server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    } else{
        cout << "bind success." << endl;
        freeaddrinfo(result);
    }

    //Listening
    if ( listen(server_socket, SOMAXCONN ) == SOCKET_ERROR ) {
        cout << "Listen failed with error: " << WSAGetLastError() << endl;
        closesocket(server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    } else{
        cout << "Listening..." << endl;
    }
}

