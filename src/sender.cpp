#include "function.hpp"

int main(int argc, char* argv[]){
    char* filename;
    int window_size;
    int buffer_size;
    char* dest_IP;
    char* dest_port; 
    vector<frame> frames;
    vector<frame> framePerBuffer;

    if (argc != 6){
        cout << "There must be 5 arguments.";
    } else{
        filename = argv[1];
        window_size = stoi(argv[2]);
        buffer_size = stoi(argv[3]);
        dest_IP = argv[4];
        dest_port = argv[5];
        char buffer[buffer_size];
        int lastSeqNum = -1;
        // Read file
        
        ifstream file(filename);
        while (file){ // While file still has content   
            cleanBuffer(buffer, buffer_size);
            file.read(buffer, buffer_size);
            // cout << buffer; //test
            //make frames
            framePerBuffer = makeFrames(buffer, buffer_size,lastSeqNum);
            frames.insert(frames.end(),framePerBuffer.begin(),framePerBuffer.end());
        }
        //check if frame is good
        printVectorFrame(frames);
        //send frames

    }
    return 0;
}