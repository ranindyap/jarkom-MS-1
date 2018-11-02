#include "function.hpp"
#include <algorithm>
// #define PORT 8888   //The port on which to listen for incoming data
   
int RWS;
vector<frame> bufferFrame;
vector<frame> recvFrame;
vector<int> ackSent;
vector<int> nakSent;
int window_start = 0;

bool isExist(int seqNum, vector<int> ackSent){
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
void eraseElement(int seqNum, vector<int> *a){
    if (isExist(seqNum, *a)){
        vector<int>::iterator it;
        int i= 0;
        for (it = a->begin(); it != a->end();it++){
            if (a->at(i) == seqNum){
                a->erase(a->begin() + i);
                break;
            }
            i++;
        }
    }
}
void printIntVector(vector<int> intVector){
    vector<int>::iterator it;
    int i= 0;
    for (it = intVector.begin(); it != intVector.end();it++){
        cout << intVector.at(i)<< ",";
        i++;
    }
    // cout << endl;
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
bool compareBySeqNum(const frame &f1, const frame &f2) {
    return f1.getSeqNum() < f2.getSeqNum();
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
        ofstream outfile;
        outfile.open(filename);
        while(strcmp(buf, FINISH_MESSAGE) != 0){
            // printIntVector(nakSent);
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
                        if (buffer_size == bufferFrame.size()){
                            sort(bufferFrame.begin(), bufferFrame.end(), compareBySeqNum);
                            // write file buffer kosongin
                            vector<frame>::iterator it;
                            int k = 0;
                            for(it = bufferFrame.begin();it != bufferFrame.end(); it++){
                                outfile << bufferFrame.at(k).getData();
                                k++;
                            }
                            bufferFrame.clear();
                        }
                        // cout << "Buffsize : " << bufferFrame.size() << endl;
                        if (bufferFrame.size() <= buffer_size){
                            memset(buf, '\0', FRAME_LENGTH);
                            if ((recv_len = recvfrom(sock, buf, FRAME_LENGTH+1, 0, (struct sockaddr *) &sender_address, &sender_len)) == SOCKET_ERROR)
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
                                    // for(int i= 9; i<1033; i++){
                                    //     cout << buf[i]; 
                                    // }
                                    receivedFrame = parseToFrame(buf);
                                    cout << "SeqNum : " << receivedFrame.getSeqNum() << endl;
                                    cout << "window_start : " << window_start << endl;
                                    // cout<< receivedFrame.getData() << ' ' << receivedFrame.getDataLength() << endl;
                                    receivedFrame.setDataLength(lengthDataInBuffer(receivedFrame.getData()));
                                    cout << receivedFrame.getDataLength() << endl;
                                    if ((receivedFrame.getSeqNum()%buffer_size < window_start+RWS) && (receivedFrame.getSeqNum()%buffer_size >= window_start)){
                                        bufferFrame.push_back(receivedFrame);
                                        recvFrame.push_back(receivedFrame);
                                        eraseElement(receivedFrame.getSeqNum(), &nakSent);
                                        cout << "Frame SeqNum :" << receivedFrame.getSeqNum() << endl;
                                    }
                                }
                            }
                            // cout << (maxSeqNum(recvFrame) + 1)%buffer_size +RWS << endl;
                            if (isReceived(window_start) && !((maxSeqNum(recvFrame) + 1)%buffer_size +RWS> buffer_size)){
                                window_start = (maxSeqNum(recvFrame) + 1)%buffer_size;
                            }
                        }
                    }
                    if (FD_ISSET(sock, &write_fd)){
                        vector<frame>::iterator it;
                        int i = 0;
                        for(it = recvFrame.begin(); it != recvFrame.end(); it++){
                            if ((!isExist(recvFrame.at(i).getSeqNum(), ackSent)) && (!isExist(recvFrame.at(i).getSeqNum(), nakSent))){
                                sendAck = makeAck(recvFrame.at(i));
                                // Gagalin checksum
                                 if (recvFrame.at(i).getSeqNum() == 2){
                                     recvFrame.at(i).setCheckSum(2);
                                 }
                                if (recvFrame.at(i).getCheckSum() != generateCheckSum(recvFrame.at(i).getData(), recvFrame.at(i).getDataLength())){
                                    sendAck.setIdxAck(0x0);
                                    sendAck.setNextSeqNum(sendAck.getNextSeqNum()-1);
                                } 
                                ack_buff = sendAck.toChars();
                                // cout << "MAKE";
                                //now reply the sender with the same data
                                if (sendto(sock, ack_buff, ACK_MAX_LENGTH+1, 0, (struct sockaddr*) &sender_address, sender_len) == SOCKET_ERROR)
                                {
                                    cout << "Sending ACK from receiver failed with code : " << WSAGetLastError() << endl;
                                    exit(EXIT_FAILURE);
                                } else{
                                    if (sendAck.getIdxAck() == 0x1){
                                        cout << "Sending ACK with for frame: " << sendAck.getNextSeqNum()-1 << " from receiver successful" << endl;
                                        ackSent.push_back(sendAck.getNextSeqNum()-1);
                                        eraseElement(sendAck.getNextSeqNum()-1, &nakSent);
                                    } else{
                                        if (!isExist(sendAck.getNextSeqNum(), nakSent)){
                                            cout << "Sending NAK with for frame: " << sendAck.getNextSeqNum()<< " from receiver successful" << endl;
                                            nakSent.push_back(sendAck.getNextSeqNum());
                                        }
                                    }
                                    // cout << sizeof(ack_buff);
                                }
                            }
                            i++;
                        }
                    }
            }
        }
        if(bufferFrame.size() != 0){
            sort(bufferFrame.begin(), bufferFrame.end(), compareBySeqNum);
            vector<frame>::iterator it;
            int l = 0;
            for(it = bufferFrame.begin();it != bufferFrame.end(); it++){
                outfile << bufferFrame.at(l).getData();
                l++;
            }
        }
        outfile.close();
        closesocket(sock);
        WSACleanup();
    }
    return 0; 
} 