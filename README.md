# soal-shift-sisop-modul-4-A02-2021

Nama Anggota Kelompok:
1. Bayu Eka Prawira         (05111940000042)
2. Putu Ananda Satria Adi   (05111940000113)

## Soal 1
### Narasi Soal
Di suatu jurusan, terdapat admin lab baru yang super duper gabut, ia bernama Sin. Sin baru menjadi admin di lab tersebut selama 1 bulan. Selama sebulan tersebut ia bertemu orang-orang hebat di lab tersebut, salah satunya yaitu Sei. Sei dan Sin akhirnya berteman baik. Karena belakangan ini sedang ramai tentang kasus keamanan data, mereka berniat membuat filesystem dengan metode encode yang mutakhir. Berikut adalah filesystem rancangan Sin dan Sei:  
```txt
NOTE : 
Semua file yang berada pada direktori harus ter-encode menggunakan Atbash cipher(mirror).
Misalkan terdapat file bernama kucinglucu123.jpg pada direktori DATA_PENTING
“AtoZ_folder/DATA_PENTING/kucinglucu123.jpg” → “AtoZ_folder/WZGZ_KVMGRMT/pfxrmtofxf123.jpg”
Note : filesystem berfungsi normal layaknya linux pada umumnya, Mount source (root) filesystem adalah directory /home/[USER]/Downloads, dalam penamaan file ‘/’ diabaikan, dan ekstensi tidak perlu di-encode.
Referensi : https://www.dcode.fr/atbash-cipher
```

### 1a
**Soal**</br>
Jika sebuah direktori dibuat dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.
</br>

**Penjelasan**</br>

</br>

**Output**</br>

### 1b
**Soal**</br>
Jika sebuah direktori di-rename dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.
</br>

**Penjelasan**</br>

</br>

**Output**</br>

### 1c
**Soal**</br>
Apabila direktori yang terenkripsi di-rename menjadi tidak ter-encode, maka isi direktori tersebut akan terdecode.
</br>

**Penjelasan**</br>

</br>

**Output**</br>

### 1d
**Soal**</br>
Setiap pembuatan direktori ter-encode (mkdir atau rename) akan tercatat ke sebuah log. Format : **/home/[USER]/Downloads/[Nama Direktori]** → **/home/[USER]/Downloads/AtoZ_[Nama Direktori]**
</br>

**Penjelasan**</br>

</br>

**Output**</br>

### 1e
**Soal**</br>
Metode encode pada suatu direktori juga berlaku terhadap direktori yang ada di dalamnya. (rekursif)
</br>

**Penjelasan**</br>

</br>

**Output**</br>


## Soal 2
### Narasi Soal
Selain itu Sei mengusulkan untuk membuat metode enkripsi tambahan agar data pada komputer mereka semakin aman. Berikut rancangan metode enkripsi tambahan yang dirancang oleh Sei.  

### 2a
**Soal**</br>
Jika sebuah direktori dibuat dengan awalan “RX_[Nama]”, maka direktori tersebut akan menjadi direktori terencode beserta isinya dengan perubahan nama isi sesuai kasus nomor 1 dengan algoritma tambahan ROT13 (Atbash + ROT13).
</br>

**Penjelasan**</br>

</br>

**Output**</br>

### 2b
**Soal**</br>
Jika sebuah direktori di-rename dengan awalan “RX_[Nama]”, maka direktori tersebut akan menjadi direktori terencode beserta isinya dengan perubahan nama isi sesuai dengan kasus nomor 1 dengan algoritma tambahan Vigenere Cipher dengan key “SISOP” (Case-sensitive, Atbash + Vigenere).
</br>

**Penjelasan**</br>

</br>

**Output**</br>

### 2c
**Soal**</br>
Apabila direktori yang terencode di-rename (Dihilangkan “RX_” nya), maka folder menjadi tidak terencode dan isi direktori tersebut akan terdecode berdasar nama aslinya.
</br>

**Penjelasan**</br>

</br>

**Output**</br>

### 2d
**Soal**</br>
Setiap pembuatan direktori terencode (mkdir atau rename) akan tercatat ke sebuah log file beserta methodnya (apakah itu mkdir atau rename).
</br>

**Penjelasan**</br>

</br>

**Output**</br>

### 2e
**Soal**</br>
Pada metode enkripsi ini, file-file pada direktori asli akan menjadi terpecah menjadi file-file kecil sebesar 1024 bytes, sementara jika diakses melalui filesystem rancangan Sin dan Sei akan menjadi normal. Sebagai contoh, Suatu_File.txt berukuran 3 kiloBytes pada directory asli akan menjadi 3 file kecil yakni:  
  
Suatu_File.txt.0000  
Suatu_File.txt.0001  
Suatu_File.txt.0002  
  
Ketika diakses melalui filesystem hanya akan muncul Suatu_File.txt  

</br>

**Penjelasan**</br>

</br>

**Output**</br>


## Soal 3
### Narasi Soal
Karena Sin masih super duper gabut akhirnya dia menambahkan sebuah fitur lagi pada filesystem mereka.  


### 3a
**Soal**</br>
Jika sebuah direktori dibuat dengan awalan “A_is_a_”, maka direktori tersebut akan menjadi sebuah direktori spesial.
</br>

**Penjelasan**</br>

</br>

**Output**</br>

### 3b
**Soal**</br>
Jika sebuah direktori di-rename dengan memberi awalan “A_is_a_”, maka direktori tersebut akan menjadi sebuah direktori spesial.

</br>

**Penjelasan**</br>

</br>

**Output**</br>

### 3c
**Soal**</br>
Apabila direktori yang terenkripsi di-rename dengan menghapus “A_is_a_” pada bagian awal nama folder maka direktori tersebut menjadi direktori normal.

</br>

**Penjelasan**</br>

</br>

**Output**</br>

### 3d
**Soal**</br>
Direktori spesial adalah direktori yang mengembalikan enkripsi/encoding pada direktori “AtoZ_” maupun “RX_” namun masing-masing aturan mereka tetap berjalan pada direktori di dalamnya (sifat recursive  “AtoZ_” dan “RX_” tetap berjalan pada subdirektori).
</br>

**Penjelasan**</br>

</br>

**Output**</br>

### 3e
**Soal**</br>
Pada direktori spesial semua nama file (tidak termasuk ekstensi) pada fuse akan berubah menjadi lowercase **insensitive** dan diberi ekstensi baru berupa nilai desimal dari binner perbedaan namanya.  
  
Contohnya jika pada direktori asli nama filenya adalah “FiLe_CoNtoH.txt” maka pada fuse akan menjadi “file_contoh.txt.1321”. 1321 berasal dari biner 10100101001.

</br>

**Penjelasan**</br>

</br>

**Output**</br>


## Soal 4
### Narasi Soal
Untuk memudahkan dalam memonitor kegiatan pada filesystem mereka Sin dan Sei membuat sebuah log system dengan spesifikasi sebagai berikut.  

### 4a
**Soal**</br>
Log system yang akan terbentuk bernama “SinSeiFS.log” pada direktori home pengguna (/home/[user]/SinSeiFS.log). Log system ini akan menyimpan daftar perintah system call yang telah dijalankan pada filesystem.
</br>

**Penjelasan**</br>

</br>

**Output**</br>

### 4b
**Soal**</br>
Karena Sin dan Sei suka kerapian maka log yang dibuat akan dibagi menjadi dua level, yaitu INFO dan WARNING.
</br>

**Penjelasan**</br>

</br>

**Output**</br>

### 4c
**Soal**</br>
Untuk log level WARNING, digunakan untuk mencatat syscall rmdir dan unlink.

</br>

**Penjelasan**</br>

</br>

**Output**</br>

### 4d
**Soal**</br>
Sisanya, akan dicatat pada level INFO.
</br>

**Penjelasan**</br>

</br>

**Output**</br>

### 4e
**Soal**</br>
Format untuk logging yaitu:  
  
[Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC :: DESC]  
  
Level : Level logging, dd : 2 digit tanggal, mm : 2 digit bulan, yyyy : 4 digit tahun, HH : 2 digit jam (format 24 Jam),MM : 2 digit menit, SS : 2 digit detik, CMD : System Call yang terpanggil, DESC : informasi dan parameter tambahan  
  
INFO::28052021-10:00:00:CREATE::/test.txt  
INFO::28052021-10:01:00:RENAME::/test.txt::/rename.txt  

</br>

**Penjelasan**</br>

</br>

**Output**</br>

