g++ -std=c++11 ack.cpp frame.cpp function.cpp sender.cpp -o sendfile -lws2_32
g++ -std=c++11 ack.cpp frame.cpp function.cpp receiver.cpp -o recvfile -lws2_32