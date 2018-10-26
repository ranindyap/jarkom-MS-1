#include "function.hpp"

int main(int argc, char* argv[]){
    char* filename;
    int window_size;
    int buffer_size;
    char* dest_IP;
    unsigned short dest_port; 
    vector<frame> frames;
    vector<frame> framePerBuffer;

    if (argc != 6){
        cout << "There must be 5 arguments.";
    } else{
        filename = argv[1];
        window_size = stoi(argv[2]);
        buffer_size = stoi(argv[3]);
        dest_IP = argv[4];
        dest_port = stoi(argv[5]);
        char buffer[buffer_size];
        int lastSeqNum = -1;
        // Read file
        
        //check if frame is good
        // 
        //send frames
        frames = makeFrames(buffer, buffer_size, lastSeqNum, filename);
        printVectorFrameProperties(frames);
        sendFrames(frames, dest_IP, dest_port);
    }
    return 0;
}