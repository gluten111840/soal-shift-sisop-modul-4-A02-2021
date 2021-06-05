#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

#define KEY "SISOP"
#define dirListSize 1000

static const char *dirpath = "/home/ananda/Downloads";
static const char *logpath = "/home/ananda/SinSeiFS.log";

int lastRXIndex;

struct encryptedDir{
    char filepath[1000];
    /*  encodeType = 1 >>> atBash + rot13
        encodeType = 2 >>> atBash + vigenere
    */
    int encodeType;
};

struct encryptedDir dirList[dirListSize];

void checkFile() {
    if(access("fuseLog.txt", F_OK )) {
		FILE *fp = fopen("fuseLog.txt", "w+");
		fclose(fp);
	} 
}

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

void getFromBehind(char delim, char string[], char substr[]) {
    bzero(substr, sizeof(substr));
    // strcpy(substr, strrchr(string, delim));
    sprintf(substr, "%s", strrchr(string, delim));
    
    char *ptr = strrchr(string, delim);
    *ptr = '\0';
    if(!strcmp(substr, "")) {
        *substr = '\0';
    }
}

/*
    Mode 0 = folder
    Mode 1 = file
*/
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

/*
    Mode 0 = folder
    Mode 1 = file
*/
void vignereEncrypt(char *message, char *key, int mode) {
    printf("\e[34mE VIGNERE : %s >>> ", message);
    char temp[1000];
    char ext[20];
    
    if(mode)
	    getFromBehind('.', message, ext);
    sprintf(temp, "%s", message);

    int i = 0, curKey = 0;
    for(i = 0; i < strlen(message); i++) {
        if(message[i] >= 'a' && message[i] <= 'z') {
            message[i] = message[i] - 'a' + 'A';
        }
    }

    for(int i = 0; i < strlen(message); i++) {
        if(curKey == strlen(key)) curKey = 0;

        if(message[i] >= 'A' && message[i] <= 'Z')
            message[i] = ((message[i] + key[curKey]) % 26);
            
        if(temp[i] >= 'a' && temp[i] <= 'z')
            message[i] += 'a';
        else if(temp[i] >= 'A' && temp[i] <= 'Z')
            message[i] += 'A';
        else
            curKey--;
        
        curKey++;
    }

    if(mode)
        strcat(message, ext);

    printf("%s\e[0m\n", message);
}

/*
    Mode 0 = folder
    Mode 1 = file
*/
void vignereDecrypt(char *message, char *key, int mode) {
    printf("\e[34mD VIGNERE : %s >>> ", message);
    
    char temp[1000];
    char ext[20];

    if(mode)
	    getFromBehind('.', message, ext);
    sprintf(temp, "%s", message);

    int i = 0, curKey = 0;
    for(i = 0; i < strlen(message); i++) {
        if(message[i] >= 'a' && message[i] <= 'z') {
            message[i] = message[i] - 'a' + 'A';
        }
    }

    for(int i = 0; i < strlen(message); i++) {
        if(curKey == strlen(key)) curKey = 0;

        if(message[i] >= 'A' && message[i] <= 'Z') {
            message[i] = message[i] - key[curKey];

            if(message[i] < 0)
                message[i] += 26;
        }

        if(temp[i] >= 'a' && temp[i] <= 'z')
            message[i] += 'a';
        else if(temp[i] >= 'A' && temp[i] <= 'Z')
            message[i] += 'A';
        else
            curKey--;
        
        curKey++;
    }

    if(mode)
        strcat(message, ext);

    printf(" %s\e[0m\n", message);
}

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

void renameRecursiveRot13(const char *path) {
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
            rot13(filename, ext, 0);
        else
            rot13(filename, ext, 1);

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

static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fullPath[1000];

    sprintf(fullPath,"%s%s",dirpath,path);

    if(!strstr(fullPath, "/."))
        printf("\e[32m    GETATTR > %s\e[0m\n", path);

    res = lstat(fullPath, stbuf);

    if (res == -1) return -errno;

    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char fullPath[1000];

    if(strcmp(path,"/") == 0) {
        path=dirpath;
        sprintf(fullPath,"%s",path);
    } else
        sprintf(fullPath, "%s%s",dirpath,path);

    printf("\e[33m  READDIR > %s\e[0m\n", path);

    int res = 0;

    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;

    dp = opendir(fullPath);

    if (dp == NULL) return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        res = (filler(buf, de->d_name, &st, 0));

        if(res!=0) break;
    }

    closedir(dp);

    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fullPath[1000];
    
    if(strcmp(path,"/") == 0) {
        path = dirpath;
        sprintf(fullPath,"%s",path);
    }
    else
        sprintf(fullPath, "%s%s",dirpath,path);

    int res = 0;
    int fd = 0 ;

    (void) fi;

    fd = open(fullPath, O_RDONLY);
    if (fd == -1) return -errno;

    res = pread(fd, buf, size, offset);
    if (res == -1) res = -errno;

    close(fd);

    return res;
}

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
        atBash(filename, ext, 0);
        printf("\e[37m\t%s %s\e[0m\n", filename, ext);
        sprintf(fullPath, "%s%s%s%s", dirpath, curPath, filename, ext);
        
        FILE *log = fopen("fuseLog.txt", "a");
        fprintf(log, "MKDIR: %s\n", fullPath);
        fclose(log);
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
        
        sprintf(fullPath, "%s%s%s%s", dirpath, curPath, filename, ext);
        FILE *log = fopen("fuseLog.txt", "a");
        fprintf(log, "MKDIR: %s\n", fullPath);
        fclose(log);
    } else {
        sprintf(fullPath, "%s%s", dirpath, path);
    }
    printf("\e[36mMKDIR  > %s\e[0m\n", fullPath);

    res = mkdir(fullPath, mode);

    if(res == -1) {
        return -errno;
    }

    char desc[1024];
    sprintf(desc, "MKDIR::%s", fullPath);
    makeLog("INFO", desc);

    return 0;
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
    } else if(strstr(to, "RX_") && !strstr(from, "RX_")) {
        
        strcpy(dirList[lastRXIndex].filepath, filename);
        dirList[lastRXIndex].encodeType = 2;
        lastRXIndex++;
    
        renameRecursiveAtBash(oldRPath);
        renameRecursiveVignere(oldRPath, 0);
        
        FILE *log = fopen("fuseLog.txt", "a");
        fprintf(log, "RENAME: %s -> %s\n", oldRPath, newRPath);
        fclose(log);
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

    char desc[1512];
    sprintf(desc, "RENAME::%s::%s", oldRPath, newRPath);
    makeLog("INFO", desc);

	return 0;
}

static int xmp_rmdir(const char *path)
{
	int res;
    char fullPath[1000];

    sprintf(fullPath, "%s%s", dirpath, path);

	res = rmdir(fullPath);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_unlink(const char *path)
{
	int res;
    char fullPath[1000];

    sprintf(fullPath, "%s%s", dirpath, path);

	res = unlink(fullPath);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi) {

    (void) fi;

    int res;
    char leftPath[1000], filename[200], ext[50];
    char fullPath[1000];

    bzero(leftPath, sizeof(leftPath));
    sprintf(leftPath, "%s", path);

    bzero(filename, sizeof(filename));
    getFromBehind('/', leftPath, filename);

    
    if(strstr(leftPath, "AtoZ_")) {
        atBash(filename, ext, 1);

        sprintf(fullPath, "%s%s%s%s", dirpath, leftPath, filename, ext);

        FILE *log = fopen("fuseLog.txt", "a");
        fprintf(log, "CREATE: %s\n", fullPath);
        fclose(log);
    } else if(strstr(leftPath, "RX_")) {
        atBash(filename, ext, 1);
        vignereEncrypt(filename, ext, 1);

        sprintf(fullPath, "%s%s%s%s", dirpath, leftPath, filename, ext);
        
        FILE *log = fopen("fuseLog.txt", "a");
        fprintf(log, "CREATE: %s\n", fullPath);
        fclose(log);
    } else {
        sprintf(fullPath, "%s%s", dirpath, path);
    }

    res = creat(fullPath, mode);
    if(res == -1)
	return -errno;

    close(res);

    return 0;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .rename = xmp_rename,
    .unlink	= xmp_unlink,
    .create = xmp_create,
	.rmdir = xmp_rmdir,
    .mkdir = xmp_mkdir,
    .read = xmp_read,
};

int  main(int  argc, char *argv[])
{
    umask(0);
    checkFile();

    return fuse_main(argc, argv, &xmp_oper, NULL);
}
