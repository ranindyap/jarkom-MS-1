#include "function.hpp"
// #define PORT 8888   //The port on which to listen for incoming data
   
int RWS;
vector<frame> bufferFrame;
vector<frame> recvFrame;
vector<int> ackSent;
int window_start = 0;

bool isExist(int seqNum){
    vector<int>::iterator it;
    bool is = false;
    int i= 0;
    for (it = ackSent.begin(); it != ackSent.end();it++){
        if (ackSent.at(i) == seqNum){
            is = true;
        }
        i++;
    }
    return is;
}
int maxSeqNum(vector<frame> recvFrame){
    vector<frame>::iterator it;
    int max = -1;
    int i= 0;
    for (it = recvFrame.begin(); it != recvFrame.end();it++){
        if (recvFrame.at(i).getSeqNum() > max){
            max = recvFrame.at(i).getSeqNum();
        }
        i++;
    }
    return max;
}
bool isReceived(int seqNum){
    vector<frame>::iterator it;
    bool is = false;
    int i= 0;
    for (it = recvFrame.begin(); it != recvFrame.end();it++){
        if (recvFrame.at(i).getSeqNum() == seqNum){
            is = true;
        }
        i++;
    }
    return is;
}
int main(int argc, char *argv[]) 
{ 
    char* filename;
    struct sockaddr_in receiver_address, sender_address;
    SOCKET sock;
    WSADATA wsa;
    int buffer_size;
    unsigned int port;
    char buf[BUF_MAX_LENGTH];
    char* ack_buff = new char[ACK_MAX_LENGTH+1];
    ack sendAck;
    frame receivedFrame;
    int recv_len, sender_len;
    int initResult, bindResult;
    if (argc != 5){
        cout << "There must be 4 arguments.";
    } else{
        filename = argv[1];
        RWS = stoi(argv[2]);
        buffer_size = stoi(argv[3]);
        port = stoi(argv[4]);
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
        
        fd_set read_fd;
        fd_set write_fd;
        fd_set timeout_fd;
        ofstream outfile(filename);
        while(strcmp(buf, FINISH_MESSAGE) != 0){
            FD_ZERO(&read_fd);
            FD_SET(sock, &read_fd);
            
            FD_ZERO(&write_fd);
            FD_SET(sock, &write_fd);

            int activity = select(sock + 1, &read_fd, &write_fd, NULL, NULL);
            switch(activity){
                case -1:
                    perror("select()");
                    exit(EXIT_FAILURE);    
                case 0:
                    cout << "Select() return 0" << endl;
                    exit(EXIT_FAILURE);
                default:
                    if (FD_ISSET(sock, &read_fd)){
                        if (bufferFrame.size() <= buffer_size){
                            memset(buf, '\0', FRAME_LENGTH);
                            if ((recv_len = recvfrom(sock, buf, FRAME_LENGTH, 0, (struct sockaddr *) &sender_address, &sender_len)) == SOCKET_ERROR)
                            {
                                cout << "receiver receiving failed with code :" << WSAGetLastError() << endl;
                                exit(EXIT_FAILURE);
                            } else{
                                // cout << buf;
                                if (strcmp(buf, FINISH_MESSAGE) == 0){
                                    cout << "Finish received";
                                    break;
                                } else{
                                    //print details of the sender/peer and the data received
                                    // cout << "Received frame from " << inet_ntoa(sender_address.sin_addr) << ":" << ntohs(sender_address.sin_port) << endl;
                                    receivedFrame = parseToFrame(buf);
                                    if ((receivedFrame.getSeqNum() < window_start+RWS) && (receivedFrame.getSeqNum() >= window_start)){
                                        bufferFrame.push_back(receivedFrame);
                                        recvFrame.push_back(receivedFrame);
                                        cout << "Frame SeqNum :" << receivedFrame.getSeqNum() << endl;
                                    }
                                    // cout << "Data: " << buf << endl;
                                }
                            }
                            if (isReceived(window_start)){
                                window_start+= maxSeqNum(recvFrame) + 1;
                            }
                        }
                        // write file buffer kosongin
                        vector<frame>::iterator it;
                        int k = 0;
                        for(it = bufferFrame.begin(); it != bufferFrame.end(); it++){
                            outfile << bufferFrame.at(k).getData();
                            k++;
                        }
                    }
                    if (FD_ISSET(sock, &write_fd)){
                        vector<frame>::iterator it;
                        int i = 0;
                        for(it = recvFrame.begin(); it != recvFrame.end(); it++){
                            if (!isExist(recvFrame.at(i).getSeqNum())){
                                sendAck = makeAck(receivedFrame);
                                if (recvFrame.at(i).getCheckSum() != generateCheckSum(recvFrame.at(i).getData(), recvFrame.at(i).getDataLength())){
                                    sendAck.setIdxAck(0x0);
                                } 
                                ack_buff = sendAck.toChars();
                                // cout << "MAKE";
                                //now reply the sender with the same data
                                if (sendto(sock, ack_buff, sizeof(ack_buff), 0, (struct sockaddr*) &sender_address, sender_len) == SOCKET_ERROR)
                                {
                                    cout << "Sending ACK from receiver failed with code : " << WSAGetLastError() << endl;
                                    exit(EXIT_FAILURE);
                                } else{
                                    cout << "Sending ACK with for frame: " << sendAck.getNextSeqNum()-1 << " from receiver successful" << endl;
                                    ackSent.push_back(sendAck.getNextSeqNum()-1);
                                    // cout << sizeof(ack_buff);
                                }
                            }
                            i++;
                        }
                    }
            }
        }
        outfile.close();
        closesocket(sock);
        WSACleanup();
    }
    return 0; 
} 