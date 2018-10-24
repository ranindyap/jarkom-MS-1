void cleanBuffer(char* buffer, long length){
    int i;
    i = 0;
    while (i < length){
        buffer[i] = '\0';
        i++;
    }
}

int dataLengthInBuffer(char* buffer){
    int i;
    bool f = false;
    i = 0;
    while ((i < length) && (f == false)){
        if (buffer[i] != '\0'){
            f = true;
        } else{
            i++;
        }
    }
    return i+1;
}
frame* makeFrame(char*buffer){
    
}