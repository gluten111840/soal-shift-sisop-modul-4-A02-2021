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

Pada soal, diminta bahwa jika kita membuat suatu folder dengan awalan `AtoZ_` maka nama semua yang ada di dalam folder tersebut, akan ter-encode dengan algoritma atbash cipher (mirror) di mana mengubah setiap huruf dengan pasangannya dari belakang (A menjadi Z, B menjadi Y, dsb), seperti pada kode di bawah ini.

```C
void atBash(char *string, char *ext, int mode) {
	int i;
    bzero(ext, sizeof(ext));
    printf("\e[36mATBASH : %s%s >> ", string, ext);
    if(mode)
	    getFromBehind('.', string, ext);
	for(i = 0; i < strlen(string); i++) {
		if(string[i] >= 'A' && string[i] <= 'Z') {
            string[i] = 'Z' - (string[i] - 'A');
		}
		if(string[i] >= 'a' && string[i] <= 'z') {
			string[i] = 'z' - (string[i] - 'a');
		}
	}
    printf("%s%s\e[0m\n", string, ext);
}

static int xmp_mkdir(const char *path, mode_t mode) {
    int res;
    char filename[100], ext[20];
    char curPath[1000], fullPath[1280];
    ...
    if(strstr(curPath, "AtoZ_")) {
        atBash(filename, ext, 0);
        printf("\e[37m\t%s %s\e[0m\n", filename, ext);
        sprintf(fullPath, "%s%s%s%s", dirpath, curPath, filename, ext);
        
        FILE *log = fopen("fuseLog.txt", "a");
        fprintf(log, "MKDIR: %s\n", fullPath);
        fclose(log);
    }
    ...
}
```

**Output**</br>
![Hasil 1a](./Foto/1a.PNG)

### 1b
**Soal**</br>
Jika sebuah direktori di-rename dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.
</br>

**Penjelasan**</br>

Sama seperti pada soal 1a, kita mengimplementasikan encoding pada perintah rename. Ketika kita merename sebuah direktori dengan awalan `AtoZ_`, maka semua yang ada di dalam direktori tersebut akan terenkripsi seperti pada kode di bawah ini. Kita menggunakan fungsi recursive pada fungsi mengubah namanya.

```C
void renameRecursiveAtBash(const char *path) {
    DIR *dp;
    struct dirent *de;

    dp = opendir(path);

    if (dp == NULL) return;

    while ((de = readdir(dp)) != NULL) {
        if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")) continue;
        char filename[100];
        char oldPath[1100];
        char newPath[1100];
        char ext[20];

        bzero(filename, sizeof(filename));
        sprintf(filename, "%s", de->d_name);

        // printf("\e[31m%s\e[0m\n", filename);
        if(de->d_type == DT_DIR)
            atBash(filename, ext, 0);
        else
            atBash(filename, ext, 1);

        bzero(oldPath, sizeof(oldPath));
        bzero(newPath, sizeof(newPath));
        sprintf(oldPath, "%s/%s", path, de->d_name);
        sprintf(newPath, "%s/%s%s", path, filename, ext);

        if(de->d_type == DT_DIR) {
            renameRecursiveAtBash(oldPath);
        }

        rename(oldPath, newPath);

    }

    closedir(dp);
}

static int xmp_rename(const char *from, const char *to)
{
	int res;
    char curPath[1000], filename[200];
    char oldRPath[1000], newRPath[1000];

    bzero(curPath, sizeof(curPath));
    sprintf(curPath, "%s", from);

    bzero(filename, sizeof(filename));
    getFromBehind('/', curPath, filename);

    sprintf(oldRPath, "%s%s", dirpath, from);
    sprintf(newRPath, "%s%s", dirpath, to);
    printf("RENAME : %s --> %s\n", from, to);

    // printf("\e[31mRENAME > %s\e[0m\n", path);
    if(strstr(to, "AtoZ_") && !strstr(from, "AtoZ_")) {
        renameRecursiveAtBash(oldRPath);

        FILE *log = fopen("fuseLog.txt", "a");
        fprintf(log, "RENAME: %s -> %s\n", oldRPath, newRPath);
        fclose(log);
    } else if (strstr(from, "AtoZ_") && !strstr(to, "AtoZ_")) {
        renameRecursiveAtBash(oldRPath);
    }
    ...
}
```
**Output**</br>
![Hasil 1b](./Foto/1b_1.PNG)
![Hasil 1b](./Foto/1b_2.PNG)
![Hasil 1b](./Foto/1b_3.PNG)

### 1c
**Soal**</br>
Apabila direktori yang terenkripsi di-rename menjadi tidak ter-encode, maka isi direktori tersebut akan terdecode.
</br>

**Penjelasan**</br>
Pada soal ini, kita diminta mendecode apabila sebuah direktori direname dengan menghapus `AtoZ_`, maka isi dari direktori tersebut akan kembali menjadi nama yang sebenarnya.

**Output**</br>
![Hasil 1c](./Foto/1c_1.PNG)
![Hasil 1c](./Foto/1c_2.PNG)

### 1d
**Soal**</br>
Setiap pembuatan direktori ter-encode (mkdir atau rename) akan tercatat ke sebuah log. Format : **/home/[USER]/Downloads/[Nama Direktori]** → **/home/[USER]/Downloads/AtoZ_[Nama Direktori]**
</br>

**Penjelasan**</br>
Dari semua soal 1a sampai 1d akan disimpan lognya ke dalam sebuah file yang berformatkan **/home/[USER]/Downloads/[Nama Direktori]** → **/home/[USER]/Downloads/AtoZ_[Nama Direktori]**
</br>

**Output**</br>
![Hasil 1d](./Foto/1d.PNG)
### 1e
**Soal**</br>
Metode encode pada suatu direktori juga berlaku terhadap direktori yang ada di dalamnya. (rekursif)
</br>

**Penjelasan**</br>
```C
void renameRecursiveAtBash(const char *path) {
    DIR *dp;
    struct dirent *de;

    dp = opendir(path);

    if (dp == NULL) return;

    while ((de = readdir(dp)) != NULL) {
        if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")) continue;
        char filename[100];
        char oldPath[1100];
        char newPath[1100];
        char ext[20];

        bzero(filename, sizeof(filename));
        sprintf(filename, "%s", de->d_name);

        // printf("\e[31m%s\e[0m\n", filename);
        if(de->d_type == DT_DIR)
            atBash(filename, ext, 0);
        else
            atBash(filename, ext, 1);

        bzero(oldPath, sizeof(oldPath));
        bzero(newPath, sizeof(newPath));
        sprintf(oldPath, "%s/%s", path, de->d_name);
        sprintf(newPath, "%s/%s%s", path, filename, ext);

        if(de->d_type == DT_DIR) {
            renameRecursiveAtBash(oldPath);
        }

        rename(oldPath, newPath);

    }

    closedir(dp);
}
```

## Soal 2
### Narasi Soal
Selain itu Sei mengusulkan untuk membuat metode enkripsi tambahan agar data pada komputer mereka semakin aman. Berikut rancangan metode enkripsi tambahan yang dirancang oleh Sei.  

### 2a
**Soal**</br>
Jika sebuah direktori dibuat dengan awalan “RX_[Nama]”, maka direktori tersebut akan menjadi direktori terencode beserta isinya dengan perubahan nama isi sesuai kasus nomor 1 dengan algoritma tambahan ROT13 (Atbash + ROT13).
</br>

**Penjelasan**</br>
Untuk soal no 2 poin A, diminta untuk melakukan enkripsi menggunakan AtBash + ROT13 jika sebuah folder **dibuat** dengan awalan "RX_[Nama]". Pertama-tama kita membuat sebuah struct dan array untuk menyimpan daftar directory yang dibuat dan direname dengan awalan "RX_". Selain itu kita juga membuat fungsi untuk mencari apakah directory sekarang termasuk dalam list directory tersebut.
```c
int lastRXIndex;

struct encryptedDir{
    char filepath[1000];
    /*  encodeType = 1 >>> atBash + rot13
        encodeType = 2 >>> atBash + vigenere
    */
    int encodeType;
};

struct encryptedDir dirList[dirListSize];

int findRXDir(char *filename) {
    int i;
    for(i = 0; i <= lastRXIndex; i++) {
        if(!strcmp(dirList[i].filepath, filename))
            return i;
    }
    return 0;
}

void removeRXDir(char *filename) {
    int i, found = 0;
    for(i = 0; i < lastRXIndex; i++) {
        if(!found) {
            if(!strcmp(dirList[i].filepath, filename)) {
                found = 1;
                dirList[i] = dirList[i+1];
            }
        } else {
            dirList[i] = dirList[i+1];
        }
    }
    lastRXIndex--;
}
```
Fungsi findRXDir() digunakan untuk mencari apakah directory tersebut termasuk dalam list. Fungsi removeRXDir() digunakan untuk menghapus sebuah directory dari list

Berikut fungsi AtBash yang digunakan sama dengan fungsi AtBash pada no 1. Sedangkan berikut adalah fungsi untuk enkripsi ROT13:
```c
/*
    Mode 0 = folder
    Mode 1 = file
*/
void rot13(char *string, char *ext, int mode) {
	int i;
    if(mode)
	    getFromBehind('.', string, ext);
	for(i = 0; i < strlen(string); i++) {
        if(string[i] >= 'A' && string[i] <= 'Z') {
            if(string[i] - 'N' >= 0)
                string[i] = string[i] - 'N' + 'A';
            else
                string[i] = string[i] + 13;
		}
		if(string[i] >= 'a' && string[i] <= 'z') {
			if(string[i] - 'n' >= 0)
                string[i] = string[i] - 'n' + 'a';
            else
                string[i] = string[i] + 13;
		}
	}
    strcat(string, ext);
    ext[0] = '\0';
    printf("ROT13  : %s%s\n", string, ext);
}
```

Kemudian dalam fungsi mkdir, implementasinya adalah sebagai berikut:
```c
static int xmp_mkdir(const char *path, mode_t mode) {
    int res;
    char filename[100], ext[20];
    char curPath[1000], fullPath[1280];

    bzero(curPath, sizeof(curPath));
    sprintf(curPath, "%s", path);

    bzero(filename, sizeof(filename));
    getFromBehind('/', curPath, filename);

    if(strstr(filename, "RX_")) {
        strcpy(dirList[lastRXIndex].filepath, filename);
        dirList[lastRXIndex].encodeType = 1;
        lastRXIndex++;
    }

    if(strstr(curPath, "AtoZ_")) {
        ...
    } else if (strstr(curPath, "RX_")) {
        char rxfilename[200];
        sprintf(rxfilename, "%s", strstr(curPath, "RX_"));
        int fileIndex = findRXDir(rxfilename);

        if(dirList[fileIndex].encodeType == 1) {
            atBash(filename, ext, 0);
            rot13(filename, ext, 0);
        } else if(dirList[fileIndex].encodeType == 2) {
            atBash(filename, ext, 0);
            vignereEncrypt(filename, ext, 0);
        }
        
        ...
    } else {
        ...
    }

    printf("\e[36mMKDIR  > %s\e[0m\n", fullPath);

    res = mkdir(fullPath, mode);

    if(res == -1) {
        return -errno;
    }

    ...

    return 0;
}
```
Sehingga jika sebuah folder dibuat dengan awalan "RX_" nama asli folder tersebut akan disimpan ke list. Selain itu dicek juga jika sebuah folder dibuat didalam sebuah directory dengan awalan "RX_" maka folder tersebut akan dienkripsi sebelum dibuat.
</br>

### 2b
**Soal**</br>
Jika sebuah direktori di-rename dengan awalan “RX_[Nama]”, maka direktori tersebut akan menjadi direktori terencode beserta isinya dengan perubahan nama isi sesuai dengan kasus nomor 1 dengan algoritma tambahan Vigenere Cipher dengan key “SISOP” (Case-sensitive, Atbash + Vigenere).
</br>

**Penjelasan**</br>
Untuk soal no 2 poin B, diminta untuk melakukan enkripsi menggunakan AtBash + Vignere jika sebuah folder **di-*rename*** dengan awalan "RX_[Nama]". Karena tadi kita sudah membuat struct untuk menyimpan daftar directory dengan awalan "RX_" serta tipe enkripsinya, maka disini kita menggunakan fungsi tersebut. Sehingga untuk fungsi rename menjadi sebagai berikut:
```c
static int xmp_rename(const char *from, const char *to)
{
	int res;
    char curPath[1000], filename[200];
    char oldRPath[1000], newRPath[1000];

    bzero(curPath, sizeof(curPath));
    sprintf(curPath, "%s", from);

    bzero(filename, sizeof(filename));
    getFromBehind('/', curPath, filename);

    sprintf(oldRPath, "%s%s", dirpath, from);
    sprintf(newRPath, "%s%s", dirpath, to);
    printf("RENAME : %s --> %s\n", from, to);

    // printf("\e[31mRENAME > %s\e[0m\n", path);
    if(strstr(to, "AtoZ_") && !strstr(from, "AtoZ_")) {
        ...
    } else if (strstr(from, "AtoZ_") && !strstr(to, "AtoZ_")) {
        ...
    } else if(strstr(to, "RX_") && !strstr(from, "RX_")) {
        
        strcpy(dirList[lastRXIndex].filepath, filename);
        dirList[lastRXIndex].encodeType = 2;
        lastRXIndex++;
    
        renameRecursiveAtBash(oldRPath);
        renameRecursiveVignere(oldRPath, 0);
        
        ...
    } else if(!strstr(to, "RX_") && strstr(from, "RX_")) {
        ...
    }

	res = rename(oldRPath, newRPath);
	if (res == -1)
		return -errno;

    ...

	return 0;
}
```
Pada potongan kode diatas, jika kita melakukan rename nama file awal tanpa awalan "RX_" dengan nama file akhir dengan awalan "RX_" maka kita akan melakukan enkripsi file. Tentunya hal pertama yang dilakukan adalah menyimpan nama file yang baru kedalam daftar directory dengan awalan "RX_". Setelah itu kita dapat memanggil fungsi renameRecursiveAtBash() untuk mengenkripsi dengan AtBash. Kemudian memanggil fungsi renameRecursiveVignere() dengan mode 0 untuk mengenkripsi menggunakan Vignere.

Fungsi renameRecursiveAtbash yang digunakan sama dengan fungsi renameRecursiveAtBash() pada no 1. Berikut adalah implementasi fungsi renameRecursiveVignere():
```c
/*
    Mode 0 = Encrypt
    Mode 1 = Decrypt
*/
void renameRecursiveVignere(const char *path, int mode) {
    DIR *dp;
    struct dirent *de;

    dp = opendir(path);

    if (dp == NULL) return;

    while ((de = readdir(dp)) != NULL) {
        if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")) continue;
        char filename[100];
        char oldPath[1100];
        char newPath[1100];

        sprintf(filename, "%s", de->d_name);
        
        if(mode){
            if(de->d_type == DT_DIR)
                vignereDecrypt(filename, KEY, 0);
            else
                vignereDecrypt(filename, KEY, 1);
        } else {
            if(de->d_type == DT_DIR)
                vignereEncrypt(filename, KEY, 0);
            else
                vignereEncrypt(filename, KEY, 1);
        }

        sprintf(oldPath, "%s/%s", path, de->d_name);
        sprintf(newPath, "%s/%s", path, filename);

        if(de->d_type == DT_DIR) {
            if(mode)
                renameRecursiveVignere(oldPath, 1);
            else
                renameRecursiveVignere(oldPath, 0);
        }

        rename(oldPath, newPath);
    }

    closedir(dp);
}
```
Fungsi ini dapat melakukan Encrypt dan Decrypt secara rekursif. Jika mode yang di-passing adalah 1 maka akan melakukan Decrypt sedangkan jika di-passing 0 maka akan melakukan Encrypt. Pertama path saat ini dibuka menggunakan fungsi opendir(), kemudian masuk ke while loop untuk melakukan looping terhadap isi dari folder tersebut. Jika dalam while loop file adalah sebuah directory/folder maka akan memanggil fungsi renameRecursiveVignere() kembali untuk melakukan folder tersebut dan melakukan rekursi.
</br>

**Output**</br>
![image](https://user-images.githubusercontent.com/57354564/121799569-71e93380-cc5f-11eb-8652-59b9558ffa53.png)

![image](https://user-images.githubusercontent.com/57354564/121799586-904f2f00-cc5f-11eb-8e55-52f1229187e8.png)

### 2c
**Soal**</br>
Apabila direktori yang terencode di-rename (Dihilangkan “RX_” nya), maka folder menjadi tidak terencode dan isi direktori tersebut akan terdecode berdasar nama aslinya.
</br>

**Penjelasan**</br>
Untuk soal no 2 poin C, kite diminta untuk melakukan decode terhadap sebuah directory/folder jika awalan "RX_" pada directory/folder tersebut dihilangkan. Pengimplementasiannya adalah sebagai berikut:
```c
static int xmp_rename(const char *from, const char *to)
{
	int res;
    char curPath[1000], filename[200];
    char oldRPath[1000], newRPath[1000];

    bzero(curPath, sizeof(curPath));
    sprintf(curPath, "%s", from);

    bzero(filename, sizeof(filename));
    getFromBehind('/', curPath, filename);

    sprintf(oldRPath, "%s%s", dirpath, from);
    sprintf(newRPath, "%s%s", dirpath, to);
    printf("RENAME : %s --> %s\n", from, to);

    // printf("\e[31mRENAME > %s\e[0m\n", path);
    if(strstr(to, "AtoZ_") && !strstr(from, "AtoZ_")) {
        ...
    } else if (strstr(from, "AtoZ_") && !strstr(to, "AtoZ_")) {
        ...
    } else if(strstr(to, "RX_") && !strstr(from, "RX_")) {
        ...
    } else if(!strstr(to, "RX_") && strstr(from, "RX_")) {
        int fileIndex = findRXDir(filename);
        if(dirList[fileIndex].encodeType == 1) {
            renameRecursiveRot13(oldRPath);
            renameRecursiveAtBash(oldRPath);
        } else if(dirList[fileIndex].encodeType == 2) {
            renameRecursiveVignere(oldRPath, 1);
            renameRecursiveAtBash(oldRPath);
        }

        removeRXDir(filename);
    }

	res = rename(oldRPath, newRPath);
	if (res == -1)
		return -errno;

    ...

	return 0;
}
```
Pada potongan kode diatas, jika kita melakukan rename nama file awal memiliki awalan "RX_" dengan nama file akhir tanpa awalan "RX_" maka kita akan melakukan dekripsi file. Hal pertama yang dilakukan adalah mencari pada index berapa nama file tersebut dapat ditemukan, kemudian mengecek tipe enkripsi yang digunakan apakah AtBash + ROT13 atau AtBash + Vignere. Jika encodeType = 1 maka untuk dekripsi, dipanggil fungsi renameRecursiveRot13() terlebih dahulu diikuti dengan fungsi renameRecursiveAtBash(). Sedangkan jika encodeType = 2 maka untuk dekripsi, dipanggil fungsi renameRecursiveVignere() terlebih dahulu diikuti dengan fungsi renameRecursiveAtBash(). Setelah semua file didekripsi secara rekursif, file yang direname kemudian dihapus dari daftar directory dengan awalan "RX_".

</br>

**Output**</br>
![image](https://user-images.githubusercontent.com/57354564/121799582-8c231180-cc5f-11eb-9d48-6d5f3c87d414.png)

![image](https://user-images.githubusercontent.com/57354564/121799575-79104180-cc5f-11eb-898b-323f43425f9b.png)


### 2d
**Soal**</br>
Setiap pembuatan direktori terencode (mkdir atau rename) akan tercatat ke sebuah log file beserta methodnya (apakah itu mkdir atau rename).
</br>

**Penjelasan**</br>
Pada poin soal ini kita diminta untuk mencatat setiap penggunaan mkdir() atau rename() kedaam sebuah log file.
```c
void checkFile() {
    if(access("fuseLog.txt", F_OK )) {
		FILE *fp = fopen("fuseLog.txt", "w+");
		fclose(fp);
	} 
}

static int xmp_mkdir(const char *path, mode_t mode) {
    ...

    if(strstr(curPath, "AtoZ_")) {
        ...
    } else if (strstr(curPath, "RX_")) {
        ...

        sprintf(fullPath, "%s%s%s%s", dirpath, curPath, filename, ext);
        FILE *log = fopen("fuseLog.txt", "a");
        fprintf(log, "MKDIR: %s\n", fullPath);
        fclose(log);
    } else {
        ...
    }

    ...

    return 0;
}

static int xmp_rename(const char *from, const char *to)
{
    ...
    
    if(strstr(to, "AtoZ_") && !strstr(from, "AtoZ_")) {
        ...
    } else if (strstr(from, "AtoZ_") && !strstr(to, "AtoZ_")) {
        ...
    } else if(strstr(to, "RX_") && !strstr(from, "RX_")) {
        ...

        FILE *log = fopen("fuseLog.txt", "a");
        fprintf(log, "RENAME: %s -> %s\n", oldRPath, newRPath);
        fclose(log);
    } else if(!strstr(to, "RX_") && strstr(from, "RX_")) {
        ...
    }

    ...

	return 0;
}
```
Pertama kita membuat sebuah fungsi untuk mengecek apakah file log sudah terbuat atau belum. Jika belum maka fungsi tersebut akan membuat file tersebut. Kemudian pada fungsi mkdir() dan rename() ditambahkan potongan kode seperti diatas untuk mencatat log tersebut kedalam file yang sudah dibuat.
</br>

**Output**</br>
![image](https://user-images.githubusercontent.com/57354564/121799607-a65cef80-cc5f-11eb-9034-bdbf5f6a9e49.png)

### 2e
**Soal**</br>
Pada metode enkripsi ini, file-file pada direktori asli akan menjadi terpecah menjadi file-file kecil sebesar 1024 bytes, sementara jika diakses melalui filesystem rancangan Sin dan Sei akan menjadi normal. Sebagai contoh, Suatu_File.txt berukuran 3 kiloBytes pada directory asli akan menjadi 3 file kecil yakni:  
  
Suatu_File.txt.0000  
Suatu_File.txt.0001  
Suatu_File.txt.0002  
  
Ketika diakses melalui filesystem hanya akan muncul Suatu_File.txt  

</br>

**Penjelasan**</br>
```c
void splitFile(char *path) {
    DIR *dp;
    struct dirent *de;

    dp = opendir(path);

    if (dp == NULL) return;

    while ((de = readdir(dp)) != NULL) {
        if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
            continue;

        char curPath[1000];
        bzero(curPath, sizeof(curPath));
        sprintf(curPath, "%s/%s", path, de->d_name);
        printf("<<->> %s\n", curPath);

        if(de->d_type == DT_DIR) {
            splitFile(curPath);
        }

        if(de->d_type == DT_REG) {
            FILE *curFile = fopen(curPath, "rb");

            struct stat fileStat;
            memset(&fileStat, 0, sizeof(fileStat));
            lstat(curPath, &fileStat);
            long fileSize = fileStat.st_size;

            printf("\e[35mPartitioning File: %s\e[34m\n", curPath);
            int fileIndex = 0, curSize;
            while(fileSize > 0) {
                char newFilePath[1100];
                char buffer[1024];

                sprintf(newFilePath, "%s.%03d", curPath, fileIndex);
                fileIndex++;

                if(fileSize > 1024) {
                    curSize = 1024;
                    fileSize -= 1024;
                } else {
                    curSize = fileSize;
                    fileSize = 0;
                }

                bzero(buffer, sizeof(buffer));
                fread(buffer, 1, curSize, curFile);

                printf(".%03d ", fileIndex);
                FILE *newFile = fopen(newFilePath, "wb");
                fwrite(buffer, 1, curSize, newFile);
                fclose(newFile);
            }
            printf("\e[0m\n");
            fclose(curFile);
            remove(curPath);
        }
    }
}

void unsplitFile(char *path) {
    DIR *dp;
    struct dirent *de;

    dp = opendir(path);

    if (dp == NULL) return;
    printf("\e[31mIn Folder: %s\e[0m\n", path);

    while ((de = readdir(dp)) != NULL) {
        if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
            continue;

        char curPath[1000];
        sprintf(curPath, "%s/%s", path, de->d_name);

        if(de->d_type == DT_DIR) {
            unsplitFile(curPath);
        }

        if(de->d_type == DT_REG) {
            char fileNumber[10], filename[100];
            bzero(fileNumber, sizeof(fileNumber));
            bzero(filename, sizeof(filename));
            sprintf(filename, "%s", de->d_name);

            getFromBehind('.', filename, fileNumber);
            if(strcmp(fileNumber, ".000")) continue;

            char realFilepath[1000];
            sprintf(realFilepath, "%s/%s", path, filename);
            printf("\e[35mCombining File: %s\e[0m\n", realFilepath);

            struct stat fileStat;
            memset(&fileStat, 0, sizeof(fileStat));
            
            FILE *realFile = fopen(realFilepath, "wb");
            int fileIndex = 0;
            printf("\e[34mFile Parts: \n");
            while(1) {
                char partFilepath[1100];
                char buffer[1024];

                sprintf(partFilepath, "%s.%03d", realFilepath, fileIndex);
                fileIndex++;

                if(access(partFilepath, F_OK)) break;

                FILE *curPart = fopen(partFilepath, "rb");
                printf(".%03d ", fileIndex);

                lstat(partFilepath, &fileStat);
                int curSize = fileStat.st_size;
                
                bzero(buffer, sizeof(buffer));

                fread(buffer, 1, curSize, curPart);
                fwrite(buffer, 1, curSize, realFile);
                fclose(curPart);
                remove(partFilepath);
            }
            fclose(realFile);
            printf("\e[0m\n");
        }
    }
}
```
Diatas merupakan percobaan kami untuk membagi file mencari potongan-potongan 1024 bytes. Untuk membagi file menjadi potongan dan menggabungkannya kembali menjadi satu file berhasil kami lakukan, namun untuk implementasi dalam file system kami mengalami kebingungan sehingga tidak dapat kami selesaikan.
</br>

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

File Log digunakan untuk menyimpan semua syscall yang telah dilakukan pada filesystem. Karena log bersifat menambah terus menerus, maka kita menggunakan append untuk menambah log.

```C
static const char *logpath = "/home/[USER]/SinSeiFS.log";

void makeLog(char *level, char *desc)
{
    FILE * fp;
    char waktu[1000];
    char log[3000];
    fp = fopen (logpath, "a+");
    time_t rawtime = time(NULL);
    struct tm *ambilrawtime = localtime(&rawtime);
    strftime(waktu, 3001, "%d%m%Y-%H:%M:%S", ambilrawtime);
    sprintf(log, "%s::%s::%s\n", level, waktu, desc);
    printf("%s\n", log);
    fputs(log, fp);

    fclose(fp);
}
```
**Output**</br>
![Hasil 4a](./Foto/4a.PNG)
### 4b
**Soal**</br>
Karena Sin dan Sei suka kerapian maka log yang dibuat akan dibagi menjadi dua level, yaitu INFO dan WARNING.
</br>

**Penjelasan**</br>

```C
static int xmp_rename(const char *from, const char *to)
{
    ...
    char desc[1512];
    sprintf(desc, "RENAME::%s::%s", oldRPath, newRPath);
    makeLog("INFO", desc);
    ...
}

static int xmp_rmdir(const char *path)
{
    ...
    char desc[1512];
    sprintf(desc, "RMDIR::%s", fullPath);
    makeLog("WARNING", desc);
    ...
}
```

### 4c
**Soal**</br>
Untuk log level WARNING, digunakan untuk mencatat syscall rmdir dan unlink.

</br>

**Penjelasan**</br>
```C
static int xmp_rmdir(const char *path)
{
    ...
    char desc[1512];
    sprintf(desc, "RMDIR::%s", fullPath);
    makeLog("WARNING", desc);
    ...
}

static int xmp_unlink(const char *path)
{
    ...
    char desc[1512];
    sprintf(desc, "UNLINK::%s", fullPath);
    makeLog("WARNING", desc);
    ...
}
```

### 4d
**Soal**</br>
Sisanya, akan dicatat pada level INFO.
</br>

**Penjelasan**</br>

```C
static int xmp_mkdir(const char *path, mode_t mode) {
    ...
    char desc[1024];
    sprintf(desc, "MKDIR::%s", fullPath);
    makeLog("INFO", desc);
    ...
}

static int xmp_rename(const char *from, const char *to)
{
    ...
    char desc[1512];
    sprintf(desc, "RENAME::%s::%s", oldRPath, newRPath);
    makeLog("INFO", desc);
    ...
}

static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi) {
    ...
    char desc[1512];
    sprintf(desc, "CREATE::%s", fullPath);
    makeLog("INFO", desc);
    ...
}
```

### 4e
**Soal**</br>
Format untuk logging yaitu:  
  
[Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC :: DESC]  
  
Level : Level logging, dd : 2 digit tanggal, mm : 2 digit bulan, yyyy : 4 digit tahun, HH : 2 digit jam (format 24 Jam),MM : 2 digit menit, SS : 2 digit detik, CMD : System Call yang terpanggil, DESC : informasi dan parameter tambahan  
  
INFO::28052021-10:00:00:CREATE::/test.txt  
INFO::28052021-10:01:00:RENAME::/test.txt::/rename.txt  

</br>

**Penjelasan**</br>
Untuk waktu diambil menggunakan `time_t` yang dapat menampilkan waktu saat sebuah perintah dijalankan.
```C
static const char *logpath = "/home/[USER]/SinSeiFS.log";

void makeLog(char *level, char *desc)
{
    FILE * fp;
    char waktu[1000];
    char log[3000];
    fp = fopen (logpath, "a+");
    time_t rawtime = time(NULL);
    struct tm *ambilrawtime = localtime(&rawtime);
    strftime(waktu, 3001, "%d%m%Y-%H:%M:%S", ambilrawtime);
    sprintf(log, "%s::%s::%s\n", level, waktu, desc);
    printf("%s\n", log);
    fputs(log, fp);

    fclose(fp);
}
```
**Output**</br>
![Hasil 4e](./Foto/4e.PNG)