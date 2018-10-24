using namespace std;
#include <iostream>
#include "function.hpp"


int main(int argc, char** argv){
    char* filename;
    long window_size;
    long buffer_size;
    char* dest_IP;
    char* dest_port; 
    vector<frame> frames;

    if (argc != 6){
        cout << "There must be 5 arguments.";
    } else{
        strcpy(filename, argv[1]);
        window_size = argv[2];
        buffer_size = argv[3];
        strcpy(dest_IP, argv[4]);
        strcpy(dest_port, argv[5]);
        char[buffer_size] buffer;
        // Read file
        ifstream file(filename, ios::in | ios::binary);
        while (file){ // While file still has content   
            cleanBuffer(buffer, buffer_size);
            file.read(buffer, buffer_size);
            //make frames
            frames = makeFrame(buffer, buffer_size);
            //send frames
            
        }
    }
    return 0;
}