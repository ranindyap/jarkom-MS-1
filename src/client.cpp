#include "function.hpp"
   
int main(int argc, char const *argv[]) 
{ 
    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    // char *hello = "Hello from client"; 
    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } else{
        cout << "Socket creation success.";
    }
    while (1){
        //wait
    }
    return 0; 
} 