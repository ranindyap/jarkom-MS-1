# Milestone 1 IF3130 Jaringan Komputer - Lossless Data Transfer

## Petunjuk Penggunaan Program
1. Program menggunakan operasi sistem Windows.
2. Jalankan makefile.
3. Buka dua console terpisah untuk sender dan receiver
4. Jalankan command berikut pada console receiver:

```./recvfile <filename> <windowsize> <buffersize> <port>```

5. Jalankan command berikut pada sender:

```./sendfile <filename> <windowsize> <buffersize> <destination_ip> <destination_port>```

## Cara Kerja Sliding Window
Mekanisme Sliding Window menggunakan *Selective Repeat Automatic Repeat Request*.
Pertama-tama, sender akan membaca data yang ingin dikirim dari file eksternal yang kemudian dibagi menjadi beberapa frame. Frame-frame ini akan dimasukkan ke dalam buffer sebesar `buffer_size`. Frame pada buffer akan dikirimkan pada sebuah window berukuran `window_size`.

Pada sisi sender, frame pada range window tersebut akan dikirimkan secara simultan. Kemudian receiver akan menerima frame dan melakukan deteksi error melalui checksum. Apabila checksum yang dihitung pada receiver tidak sama dengan checksum frame yang diterima, maka receiver akan mengirimkan *NAK*. Sebaliknya, frame tidak ditemukan error, sehingga receiver akan mengirimkan *ACK*. Ketika sender menerima NAK, sender akan mengirimkan kembali frame yang error.

Setiap frame yang dikirimkan sender pada window memiliki timeout. Ketika receiver belum mengirimkan NAK/ACK, sender akan mengirimkan frame tersebut.

## Penjelasan Fungsi
### File ack.hpp
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

### File frame.hpp
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

### File function.hpp
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

### File receiver.cpp
### File sender.cpp
| Fungsi | Penjelasan |
| --- | --- |
|bool isAllAckExistinWindow(int window_start, int window_size, vector ack ackV) | Mengecek apakah semua frame di dalam window sudah diterima ack-nya |
| void eraseElement(int seqNum, vector ack *a) | Menghapus elemen dengan seqNum di vector ack a |
| bool isExist(int seqNum, vector ack a)| Mengecek apakah suatu ack dengan seqNum tertentu pada vector ack a|
| void printAckVector(vector ack ackVector) | Untuk mencetak semua seqNum ack dalam vector ke layar. |
| bool notEmptyTimeOut(vector<timeOut> T) | Mengecek apakah vektor TimeOut not empty |
|int initSocketSender(int* sender_socket, struct sockaddr_in *sender_address, char* ipAddress, unsigned short port)| Menginisialisasi sender socket|
| int main(int argc, char* argv[]) | Fungsi utama yang menjalankan program sender |
