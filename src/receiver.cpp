#include "function.hpp"
// #define PORT 8888   //The port on which to listen for incoming data
   
int main(int argc, char const *argv[]) 
{ 
    struct sockaddr_in receiver_address, sender_address;
    SOCKET sock;
    WSADATA wsa;
    char buf[BUF_MAX_LENGTH];
    int recv_len, sender_len;
    int initResult, bindResult;

    sender_len = sizeof(sender_address);
    //Initialize Winsock
    initResult = initWinSock(wsa);
    if (initResult != 0){
        cout << "Initialize receiver WinSock failed with code: " << WSAGetLastError()<< endl;
        exit(EXIT_FAILURE); 
    } else{
        cout << "Initialize receiver WinSock successful" << endl;
    }
    //Create socket
    sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (sock == INVALID_SOCKET){
        cout << "Creating receiver Socket failed with code: " << WSAGetLastError()<< endl;
        exit(EXIT_FAILURE); 
    } else{
        cout << "Creating receiver Socket successful" << endl;
    }
    //Prepare receiver_address
    receiver_address.sin_family = AF_INET;
    receiver_address.sin_addr.s_addr = INADDR_ANY;
    receiver_address.sin_port = htons(DEFAULT_RECEIVER_PORT);

    //binding
    bindResult = bind(sock, (struct sockaddr*)&receiver_address, sizeof(receiver_address));
    if (bindResult == SOCKET_ERROR){
        cout << "Binding receiver Socket failed with code: " << WSAGetLastError()<< endl;
        exit(EXIT_FAILURE); 
    } else{
        cout << "Binding receiver Socket successful" << endl;
    }
    while(1){
        cout << "Waiting ..."<< endl;
         
        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', 1024);
         
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(sock, buf, 1024, 0, (struct sockaddr *) &sender_address, &sender_len)) == SOCKET_ERROR)
        {
            cout << "receiver receiving failed with code :" << WSAGetLastError() << endl;
            exit(EXIT_FAILURE);
        } else{
            //print details of the sender/peer and the data received
            cout << "Received packet from " << inet_ntoa(sender_address.sin_addr) << ":" << ntohs(sender_address.sin_port) << endl;
            cout << "Data: " << buf << endl;
        }
         
        //now reply the sender with the same data
        if (sendto(sock, buf, recv_len, 0, (struct sockaddr*) &sender_address, sender_len) == SOCKET_ERROR)
        {
            cout << "Sending from receiver failed with code : " << WSAGetLastError() << endl;
            exit(EXIT_FAILURE);
        } else{
            cout << "Sending from receiver successful" << endl;
        }
    }
    closesocket(sock);
    WSACleanup();
    return 0; 
} 