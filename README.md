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