# jarkom-MS-1

# Penjelasan Fungsi
## File ack.hpp
| Fungsi | Penjelasan |
| --- | --- |
| ack(char i, int nS, char c); | Konstruktor dari kelas Ack dengan input idxAck, nextSeqNum, dan checkSum|
| ack(); | Konstruktor default Kelas Ack tanpa masukkan |
| char getIdxAck(); | Getter atribut IdxAck |
| int getNextSeqNum(); | Getter atribut NextSeqNum |
| char getCheckSum(); | Getter atribut CheckSum |
| void setIdxAck(char i); | Setter atribut IdxAck |
| void setNextSeqNum(int nS); | Setter atribut NextSeqNum |
| void setCheckSum(char c); | Setter atribut CheckSum |
| char* toChars(); | Mengubah objek Ack menjadi char*|

## File frame.hpp
| Fungsi | Penjelasan |
| --- | --- |
| frame(); | Konstruktor default Kelas Frame |
| frame( char SOH,int seqNum,int dataLength,char* data, unsigned char checksum);  | KOnstruktor Kelas Frame dengan input inisial setiap atributnya |
| char getSOH(); | Getter atributSOH |
| int getSeqNum(); | Getter atribut SeqNum |
| int getDataLength(); | Getter atribut DataLength|
| char* getData(); | Getter atribut Data |
| unsigned char getCheckSum(); | Getter atribut CheckSum|
| void setSOH(char SOH); | Setter atribut SOH |
| void setSeqNum(int SeqNum); | Setter atribut  SeqNum |
| void setDataLength(int DataLength); | Setter atribut Data  DataLength|
| void setData(char* Data); | Setter atribut  |
| void setCheckSum(unsigned char CheckSum); | Setter atribut CheckSum |
| void printData(); | Mencetak isi data pada frame ke layar |
| void printProperties(); | Mencetak seqNum dan dataLength ke layar |
| char* toChars(); | Mengubah objek frame menjadi char* |

## File function.hpp
| Fungsi | Penjelasan |
| --- | --- |
| void cleanBuffer(char* buffer, long length); | Membersihkan buffer dengan cara melakukan assign null |
| unsigned Char4ToInt(char* number); | Melakukan konversi dari char 4 byte ke integer |
| ack parseToAck(char* buff); | Melakukan parsing bagian-bagian ACK (ACK, Next Sequence Number, Checksum) |
| frame parseToFrame(char * buff); | Melakukan parsing bagian-bagian frame (SOH, Sequence Number, Data Length, Data, Checksum) |
| int dataLengthInBuffer(char* buffer, int length); | Mengembalikan nilai data length |
| vector<frame> makeFrames(char*buffer, int length, int &lastSeqNum, char* filename); | Membuat frame dari buffer |
| ack makeAck(frame f); | Membuat ACK dari frame yang diterima |
| unsigned char generateCheckSum(char* data, int dataLength); | Menghasilkan nilai checksum |
| void printVectorFrame(vector<frame> f); | Mencetak isi vector frame |
| void printVectorFrameProperties(vector<frame> f); | Mencetak seqNum dan dataLength dari frame dalam vector frame |
| int initWinSock(WSADATA w); | Melakukan inisialisasi Winsock |
| int lengthDataInBuffer(char* buffer); | Mengembalikan nilai data length |
| bool findAck(int seqNum, vector<ack> ackV); | Mencari ACK dari frame dengan sequence number tertentu |

## File receiver.cpp
## File sender.cpp