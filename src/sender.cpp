#include "function.hpp"
#include "window.hpp"
// GLOBAL
#define FRAMESIZE 1034
#define ACKSIZE 6

int SWS;
int buffer_size;
struct timeOut{
    int seqNum;
    clock_t time;
} T;
vector<timeOut> timeOutVector;
vector<ack> ackVector;
int buffer_start = 0;
bool boleh = false;

bool notEmptyTimeOut(vector<timeOut> T){
    if (T.size() != 0){
        return true;
    } else{
        return false;
    }
}
int initSocketSender(int* sender_socket, struct sockaddr_in *sender_address, char* ipAddress, unsigned short port){
     //Initialize Winsock
    int initResult;
    char* frame_buff;
    ack realAck;
    int slen=sizeof(sender_address);
    char buf[BUF_MAX_LENGTH], ackString[ACK_MAX_LENGTH+5];
    char message[BUF_MAX_LENGTH];
    WSADATA wsa;

    initResult = initWinSock(wsa);
    if (initResult != 0){
        cout << "Initialize sender WinSock failed with code: " << WSAGetLastError()<< endl;
        exit(EXIT_FAILURE); 
    } else{
        cout << "Initialize sender WinSock successful" << endl;
    }
    //Create sender_socket
    *sender_socket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (*sender_socket == INVALID_SOCKET){
        cout << "Creating sender Socket failed with code: " << WSAGetLastError()<< endl;
        exit(EXIT_FAILURE); 
    } else{
        cout << "Creating sender Socket successful" << endl;
    }

    //set sender_address
    memset(sender_address, 0, sizeof(*sender_address));
    sender_address->sin_family = AF_INET;
    sender_address->sin_port = htons(port);
    sender_address->sin_addr.S_un.S_addr = inet_addr(ipAddress);
    cout << "Address success in IP " << ipAddress << " on port " << port<< endl;
    fflush(stdout);
    return 1; 
}

int main(int argc, char* argv[]){
    char* filename;
    char* dest_IP;
    unsigned short dest_port; 
    vector<frame> frames;
    vector<frame> framePerBuffer;

    if (argc != 6){
        cout << "There must be 5 arguments.";
    } else{
        filename = argv[1];
        SWS = stoi(argv[2]);
        buffer_size = stoi(argv[3]);
        dest_IP = argv[4];
        dest_port = stoi(argv[5]);
        char buffer[buffer_size];
        int lastSeqNum = -1;

        int sender_socket;
        struct sockaddr_in sender_address;

        // Read file
        
        //check if frame is good
        // 
        //send frames
        frames = makeFrames(buffer, buffer_size, lastSeqNum, filename);
        printVectorFrameProperties(frames);
        initSocketSender(&sender_socket, &sender_address, dest_IP, dest_port);

        int idx = 0;
        
        fd_set read_fd;
        fd_set write_fd;
        fd_set timeout_fd;
        while(!(frames.size() == ackVector.size())){
            // timeout setting
            FD_ZERO(&read_fd);
            if (idx < buffer_size){
                FD_SET(sender_socket, &read_fd);
            }
            
            FD_ZERO(&write_fd);
            if (idx < buffer_size){
                FD_SET(sender_socket, &write_fd);
            }

            FD_ZERO(&timeout_fd);
            if (idx>=0){
                FD_SET(sender_socket,&timeout_fd);
            }
            int activity = select(32, &read_fd, &write_fd, &timeout_fd, NULL);
            switch(activity){
                case -1:
                    perror("select()");
                    exit(EXIT_FAILURE);    
                case 0:
                    cout << "Select() return 0" << endl;
                    exit(EXIT_FAILURE);
                default:
                    if (FD_ISSET(sender_socket, &timeout_fd)){
                        cout<< "SET TIMEOUT" << endl;
                        vector<timeOut>::iterator itTimeOut;
                        int i = 0;
                        for (itTimeOut = timeOutVector.begin(); itTimeOut != timeOutVector.end();itTimeOut++){
                            if ((!findAck(timeOutVector.at(i).seqNum, ackVector)) && (timeOutVector.at(i).time - clock() >= TIME_OUT)){
                                idx = timeOutVector.at(i).seqNum;
                                break;
                            }
                            i++;
                        }
                    }
                    if (FD_ISSET(sender_socket, &read_fd)) {
                        cout<< "SET READ" << endl;
                        char ackString[ACK_MAX_LENGTH+5];
                        int slen = sizeof(sender_address);
                        ack realAck;
                        if (boleh){
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
                                if(realAck.getIdxAck() != 0x1){
                                    ackVector.push_back(realAck);
                                } else{
                                    idx = realAck.getNextSeqNum();
                                }
                            }
                        }
                    }

                    if (FD_ISSET(sender_socket, &write_fd)) {
                        cout<< "SET WRITE" << endl;
                        vector<frame>::iterator it;
                        char* frame_buff;
                        int size = static_cast<int>(frames.size());
                        int i =0;
                        int j = 0;
                        int window_start = 0;
                        if(idx < buffer_start+buffer_size){
                            if(idx < window_start+SWS){
                                //sending
                                bool sent = false;
                                int timeOutCurr;
                                if (notEmptyTimeOut(timeOutVector)){
                                    vector<timeOut>::iterator itTimeOut;
                                    int i = 0;
                                    for (itTimeOut = timeOutVector.begin(); itTimeOut != timeOutVector.end();itTimeOut++){
                                        if (timeOutVector.at(i).seqNum == idx){
                                            sent = true;
                                            timeOutCurr = timeOutVector.at(i).time;
                                        }
                                        i++;
                                    }
                                }
                                //  || (sent && !findAck(idx,ackVector) && (timeOutCurr-clock()>=TIME_OUT))
                                if (!sent){
                                    frame_buff = frames.at(idx).toChars();    
                                    // cout << "HERE";
                                    if (sendto(sender_socket, frame_buff, sizeof(frame_buff) , 0 , (struct sockaddr *) &sender_address, sizeof(sender_address)) == SOCKET_ERROR)
                                    {
                                        cout << "Sending from sender failed with code : " << WSAGetLastError() << endl;
                                        boleh = false;
                                        exit(EXIT_FAILURE);
                                    } else{
                                        cout << "Sending from sender frame number " << frames.at(idx).getSeqNum() << " success" << endl;
                                        boleh = true;
                                        struct timeOut T;
                                        T.seqNum = frames.at(idx).getSeqNum();
                                        T.time = clock();
                                        timeOutVector.push_back(T);
                                        idx++;
                                    }
                                } else{
                                    idx++;
                                }
                                if ((findAck(window_start, ackVector)) && (window_start+SWS < buffer_size)){
                                    window_start++;
                                }
                            }
                            if (SWS+window_start == buffer_size+buffer_start){
                                buffer_start += buffer_size;
                                window_start += SWS; 
                            }
                        }
                    }
            }
        }
        if (sendto(sender_socket, FINISH_MESSAGE, sizeof(FINISH_MESSAGE) , 0 , (struct sockaddr *) &sender_address, sizeof(sender_address)) == SOCKET_ERROR)
        {
            cout << "Sending FINISH from sender failed with code : " << WSAGetLastError() << endl;
            exit(EXIT_FAILURE);
        } else{
            cout << "Sending FINISH from sender success" << endl;
        }
        closesocket(sender_socket);
        WSACleanup();
    }
    return 0;
}