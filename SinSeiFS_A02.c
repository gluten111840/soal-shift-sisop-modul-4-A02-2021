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

static const char *dirpath = "/home/bayuekap/Downloads";
static const char *logpath = "/home/bayuekap/SinSeiFS.log";

void checkFile() {
    if(access("fuseLog.txt", F_OK )) {
		FILE *fp = fopen("fuseLog.txt", "w+");
		fclose(fp);
	} 
}

void makeLog(char *level, char *desc)
{
    FILE * fp;
    char waktu[100];
    char log[100];
    fp = fopen (logpath, "a+");
    time_t rawtime = time(NULL);
    struct tm *ambilrawtime = localtime(&rawtime);
    strftime(waktu, 100, "%d%m%y-%H:%M:%S", ambilrawtime);
    sprintf(log, "%s::%s::%s\n", level, waktu, desc);
    fputs(log, fp);

    fclose(fp);
}

char *strrev(char *str)
{
      char *p1, *p2;

      if (!str || !*str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

void getFromBehind(char delim, char string[], char substr[]) {
	int in = 0, cur = strlen(string) - 1;
	char temp[1024];
	strcpy(temp, string);
	while(temp[cur] != delim){
		if(cur < 0) break;
		substr[in] = temp[cur];
        in++;
        temp[cur] = '\0';
        cur--;
	}
	substr[in] = temp[cur];
    temp[cur] = '\0';
	substr[++in] = '\0';

	if(strcmp(temp, "")) {
		strcpy(string, temp);
		strrev(substr);
	} else {
		substr[0] = '\0';
	}
	printf("\e[36m%s\e[0m", string);

}

void atBash(char *string, char *ext, int mode) {
	int i;
    if(mode)
	    getFromBehind('.', string, ext);
	for(i = 0; i < strlen(string); i++) {
		if(string[i] >= 'A' && string[i] <= 'N') {
            string[i] = 'N' - (string[i] - 'A');
		}
		if(string[i] >= 'a' && string[i] <= 'z') {
			string[i] = 'z' - (string[i] - 'a');
		}
	}
    printf("ATBASH : %s%s\n", string, ext);
}

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

void vignereEncrypt(char *message, char *key, int mode) {
    printf("\e[31mENCRYPT : %s >>> ", message);
    char temp[1000];
    char ext[20];
    
    if(mode)
	    getFromBehind('.', message, ext);
    strcpy(temp, message);

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

void vignereDecrypt(char *message, char *key, int mode) {
    printf("\e[32mDECRYPT : %s >>> ", message);
    
    char temp[1000];
    char ext[20];

    if(mode)
	    getFromBehind('.', message, ext);
    strcpy(temp, message);

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

        strcpy(filename, de->d_name);

        if(de->d_type == DT_DIR)
            atBash(filename, ext, 0);
        else
            atBash(filename, ext, 1);

        sprintf(oldPath, "%s/%s", path, de->d_name);
        sprintf(newPath, "%s/%s%s", path, filename, ext);

        if(de->d_type == DT_DIR && !strstr(filename, "AtoZ_") && !strstr(de->d_name, "AtoZ_")) {
            renameRecursiveAtBash(oldPath);
        }

        printf("oldPath : %s\n", oldPath);
        printf("newPath : %s\n", newPath);
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

        strcpy(filename, de->d_name);
        
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

        if(de->d_type == DT_DIR /*&& !strstr(filename, "RX_") && !strstr(de->d_name, "RX_")*/) {
            if(mode)
                renameRecursiveVignere(oldPath, 1);
            else
                renameRecursiveVignere(oldPath, 0);
        }

        printf("oldPath : %s\n", oldPath);
        printf("newPath : %s\n", newPath);
        rename(oldPath, newPath);
    }

    closedir(dp);
}

static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fullPath[1000];

    sprintf(fullPath,"%s%s",dirpath,path);

    printf("\e[32mGETATTR  > %s\e[0m\n\n", fullPath);

    res = lstat(fullPath, stbuf);

    if (res == -1) return -errno;

    return 0;
}



static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char fullPath[1000];

    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fullPath,"%s",path);
    } else sprintf(fullPath, "%s%s",dirpath,path);

    printf("\e[32mREADDIR  > %s\e[0m\n\n", path);

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
    
    if(strcmp(path,"/") == 0)
    {
        path = dirpath;
        sprintf(fullPath,"%s",path);
    }
    else sprintf(fullPath, "%s%s",dirpath,path);

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

    printf("\e[31mMKDIR  > %s\e[0m\n\n", path);
    
    if(strstr(path, "AtoZ_")) {
        sprintf(fullPath, "%s%s", dirpath, path);

        FILE *log = fopen("fuseLog.txt", "a");
        fprintf(log, "MKDIR: %s\n", fullPath);
        fclose(log);
    } else {
        sprintf(fullPath, "%s%s", dirpath, path);
    }

    res = mkdir(fullPath, mode);

    if(res == -1) {
        return -errno;
    }

    char desc[100];
    sprintf(desc, "MKDIR::%s", fullPath);
    makeLog("INFO", desc);

    return 0;
}

static int xmp_rename(const char *from, const char *to)
{
	int res;
    char oldRPath[1000];
    char newRPath[1000];

    sprintf(oldRPath, "%s%s", dirpath, from);
    sprintf(newRPath, "%s%s", dirpath, to);
    printf("FROM --> %s\n", from);
    printf("TO   --> %s\n", to);

    // printf("\e[31mRENAME > %s\e[0m\n\n", path);
    if(strstr(to, "AtoZ_") && !strstr(from, "AtoZ_")) {
        FILE *log = fopen("fuseLog.txt", "a");
        fprintf(log, "RENAME: %s -> %s\n", oldRPath, newRPath);
        fclose(log);
        renameRecursiveAtBash(oldRPath);
    } else if (strstr(from, "AtoZ_") && !strstr(to, "AtoZ_")) {
        renameRecursiveAtBash(oldRPath);
    } else if(strstr(to, "RX_") && !strstr(from, "RX_")) {
        renameRecursiveAtBash(oldRPath);
        renameRecursiveVignere(oldRPath, 0);
    } else if(!strstr(to, "RX_") && strstr(from, "RX_")) {
        renameRecursiveVignere(oldRPath, 1);
        renameRecursiveAtBash(oldRPath);
    }

	res = rename(oldRPath, newRPath);
	if (res == -1)
		return -errno;

    char desc[100];
    sprintf(desc, "RENAME::%s::%s", oldRPath, newRPath);
    makeLog("INFO", desc);

	return 0;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .mkdir = xmp_mkdir,
    .rename = xmp_rename,
    .read = xmp_read,
};

int  main(int  argc, char *argv[])
{
    umask(0);
    checkFile();

    return fuse_main(argc, argv, &xmp_oper, NULL);
}

