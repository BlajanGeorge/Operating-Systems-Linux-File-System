#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int nrSectiuni;
int version;

void listareRecursivaCuConditie(const char * path, int sizeConditie, char * stringEnd) {
    DIR * dir = NULL;
    struct dirent * entry = NULL;

    dir = opendir(path);
    if (dir == NULL)
        return;

    if (sizeConditie != -1) {
        struct stat statbuf;
        char newPath[1000];
        while ((entry = readdir(dir)) != NULL) {
            strcpy(newPath, path);
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                strcat(newPath, "/");
                strcat(newPath, entry->d_name);
                if (lstat(newPath, & statbuf) == 0) {
                    if (S_ISREG(statbuf.st_mode)) {
                        if (statbuf.st_size > sizeConditie)
                            printf("%s\n", newPath);
                    }
                }
                listareRecursivaCuConditie(newPath, sizeConditie, stringEnd);
            }
        }
    } else {
        if (stringEnd != NULL) {
            char * c = (char * ) malloc(strlen(stringEnd));
            char newPath[1000];
            while ((entry = readdir(dir)) != NULL) {
                strcpy(newPath, path);
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                    strncpy(c, entry->d_name + strlen(entry->d_name) - strlen(stringEnd), strlen(stringEnd));
                    strcat(newPath, "/");
                    strcat(newPath, entry->d_name);
                    if (strcmp(c, stringEnd) == 0) {
                        printf("%s\n", newPath);
                    }
                    listareRecursivaCuConditie(newPath, sizeConditie, stringEnd);
                }
            }
            free(c);
        }
    }
    closedir(dir);
    return;
}

//-----------------------------------------------
void listareRecursivaFaraConditie(const char * path) {
    DIR * dir = NULL;
    struct dirent * entry = NULL;
    struct stat statbuf;
    dir = opendir(path);
    if (dir == NULL)
        return;

    char newPath[1000];


    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            strcpy(newPath, path);
            strcat(newPath, "/");
            strcat(newPath, entry->d_name);
            if (lstat(newPath, & statbuf) == 0) {
                printf("%s\n", newPath);
                if (S_ISDIR(statbuf.st_mode)) {
                    listareRecursivaFaraConditie(newPath);
                }
            }
        }
    }
    closedir(dir);
    return;
}

//------------------------------------------------
void listareIterativaCuConditie(const char * path, int sizeConditie, char * stringEnd) {
    DIR * dir = NULL;
    struct dirent * entry = NULL;

    dir = opendir(path);
    if (dir == NULL) {
        closedir(dir);
        perror("ERROR\n");
        return;
    }

    if (sizeConditie != -1) {
        struct stat statbuf;
        char newPath[1000];
        printf("SUCCESS\n");
        while ((entry = readdir(dir)) != NULL) {
            strcpy(newPath, path);
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                strcat(newPath, "/");
                strcat(newPath, entry->d_name);
                if (lstat(newPath, & statbuf) == 0) {
                    if (S_ISREG(statbuf.st_mode)) {
                        if (statbuf.st_size > sizeConditie)
                            printf("%s\n", newPath);
                    }
                }
            }

        }
    } else {
        if (stringEnd != NULL) {
            char * c = (char * ) malloc(strlen(stringEnd));
            printf("SUCCESS\n");
            while ((entry = readdir(dir)) != NULL) {
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                    strncpy(c, entry->d_name + strlen(entry->d_name) - strlen(stringEnd), strlen(stringEnd));
                    if (strcmp(c, stringEnd) == 0) {
                        printf("%s/%s\n", path, entry->d_name);
                    }
                }
            }
            free(c);
        }
    }
    closedir(dir);
    return;
}
//-----------------------------------------------
void listareIterativaFaraConditie(const char * path) {
    DIR * dir = NULL;
    struct dirent * entry = NULL;


    dir = opendir(path);
    if (dir == NULL) {
        perror("ERROR\n");
        return;
    }
    printf("SUCCESS\n");
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            printf("%s/%s\n", path, entry->d_name);
    }
    closedir(dir);
    return;

}
//-----------------------------------------------
void getPath(int argc, char ** argv, char ** path) {
    char s1[] = "path=";
    char * p;
    for (int i = 2; i < argc; i++) {
        p = strstr(argv[i], s1);
        if (p) {
            * path = (char * ) malloc(strlen(argv[i]) - 4);
            strncpy( * path, argv[i] + 5, strlen(argv[i]) - 5);
            return;
        }
    }
    return;
}
//---------------------------------------------
int getSizeConditie(int argc, char ** argv) {
    char s1[] = "size_greater=";
    char * p;
    int size = 0;
    for (int i = 2; i < argc; i++) {
        p = strstr(argv[i], s1);
        if (p) {
            char * c = (char * ) malloc(strlen(argv[i]) - 12);
            strncpy(c, argv[i] + 13, strlen(argv[i]) - 13);
            sscanf(c, "%d", & size);
            free(c);
            return size;
        }
    }
    return -1;
}
//-----------------------------------------------
void getStringEnd(int argc, char ** argv, char ** stringEnd) {
    char s1[] = "name_ends_with=";
    char * p;
    for (int i = 2; i < argc; i++) {
        p = strstr(argv[i], s1);
        if (p) {
            * stringEnd = (char * ) malloc(strlen(argv[i]) - 14);
            strncpy( * stringEnd, argv[i] + 15, strlen(argv[i]) - 15);
            return;
        }
    }
    return;
}
//---------------------------------------------
void prelucrareComandaList(int argc, char ** argv) {
    char * path = NULL;
    getPath(argc, argv, & path);
    int sizeConditie = getSizeConditie(argc, argv);
    char * stringEnd = NULL;
    getStringEnd(argc, argv, & stringEnd);

    if (path == NULL) {
        perror("ERROR\n");
        free(path);
        free(stringEnd);
        return;
    }
    if (argc == 3) {
        listareIterativaFaraConditie(path);
        free(path);
        free(stringEnd);
        return;
    }

    if (argc == 4) {
        for (int i = 2; i < argc; i++)
            if (strcmp(argv[i], "recursive") == 0) {
                printf("SUCCESS\n");
                listareRecursivaFaraConditie(path);
                free(path);
                free(stringEnd);
                return;
            }

        listareIterativaCuConditie(path, sizeConditie, stringEnd);
        free(path);
        free(stringEnd);
        return;
    }
    if (argc == 5) {
        printf("SUCCESS\n");
        listareRecursivaCuConditie(path, sizeConditie, stringEnd);
        free(path);
        free(stringEnd);
        return;
    }

    perror("ERROR\n");
    return;
}
//-----------------------------------------------
int prelucrareComandaParse2(char * path) {
    int fd;
    fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("ERROR\n");
        close(fd);
        return -1;
    }
    char s[4];
    if (read(fd, s, 4) == -1) {
        perror("ERROR\n");
        close(fd);
        return -2;
    }
    if (strcmp(s, "SFMY")) {
        close(fd);
        return -3;
    }

    lseek(fd, 6, SEEK_SET);
    unsigned char c;
    if (read(fd, & c, 1) == -1) {
        perror("ERROR\n");
        return -1;
    }
    version = c;
    if (c < 99 || c > 195) {
        close(fd);
        return -4;
    }

    if (read(fd, & c, 1) == -1) {
        perror("ERROR\n");
        return -1;
    }
    if (c < 3 || c > 10) {
        close(fd);
        return -5;
    }

    nrSectiuni = (int) c;
    int i = 1;
    char s2[2];
    while (i <= nrSectiuni) {
        lseek(fd, 11, SEEK_CUR);
        if (read(fd, s2, 2) == -1) {
            perror("ERROR\n");
            return -1;
        }
        if ((s2[0] != 'C' && s2[0] != 'J') || (s2[1] != 0)) {
            close(fd);
            return -6;
        }
        lseek(fd, -13, SEEK_CUR);
        lseek(fd, 21, SEEK_CUR);
        i++;
    }

    close(fd);
    return 0;
}
//----------------------------------------------
void prelucrareComandaParse(int argc, char ** argv) {
    char * path = NULL;
    getPath(argc, argv, & path);
    int rez = prelucrareComandaParse2(path);
    if (rez == -3) {
        printf("ERROR\n");
        printf("wrong magic\n");
        free(path);
        return;
    }
    if (rez == -4) {
        printf("ERROR\n");
        printf("wrong version\n");
        free(path);
        return;
    }
    if (rez == -5) {
        printf("ERROR\n");
        printf("wrong sect_nr\n");
        free(path);
        return;
    }
    if (rez == -6) {
        printf("ERROR\n");
        printf("wrong sect_types\n");
        free(path);
        return;
    }
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("ERROR\n");
        return;
    }
    printf("SUCCESS\n");
    printf("version=%d\n", version);
    printf("nr_sections=%d\n", nrSectiuni);
    char * name = (char * ) malloc(11);
    int size = 0;
    lseek(fd, 8, SEEK_SET);
    int i = 1;
    char s2[2];
    while (i <= nrSectiuni) {
        printf("section%d: ", i);
        if (read(fd, name, 11) == -1) {
            perror("ERROR\n");
            return;
        }
        printf("%s ", name);
        if (read(fd, s2, 2) == -1) {
            perror("ERROR\n");
            return;
        }
        printf("%d ", s2[0]);
        lseek(fd, 4, SEEK_CUR);
        if (read(fd, & size, 4) == -1) {
            perror("ERROR\n");
            return;
        }
        printf("%d", size);
        printf("\n");
        i++;
    }
    free(name);
    free(path);
    close(fd);
}
//--------------------------------------------
int getSectionNumber(int argc, char ** argv) {
    char s1[] = "section=";
    char * p;
    int numb = 0;
    for (int i = 2; i < argc; i++) {
        p = strstr(argv[i], s1);
        if (p) {
            char * c = (char * ) malloc(strlen(argv[i]) - 7);
            strncpy(c, argv[i] + 8, strlen(argv[i]) - 8);
            sscanf(c, "%d", & numb);
            free(c);
            return numb;
        }
    }
    return -1;
}
//--------------------------------------------
int getLineNumber(int argc, char ** argv) {
    char s1[] = "line=";
    char * p;
    int numb = 0;
    for (int i = 2; i < argc; i++) {
        p = strstr(argv[i], s1);
        if (p) {
            char * c = (char * ) malloc(strlen(argv[i]) - 4);
            strncpy(c, argv[i] + 5, strlen(argv[i]) - 5);
            sscanf(c, "%d", & numb);
            free(c);
            return numb;
        }
    }
    return -1;
}
//--------------------------------------------
int prelucrareComandaExtract2(char * path, int numberSect, int numberLine) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("ERROR\n");
        close(fd);
        return -1;
    }
    lseek(fd, 8 + 21 * (numberSect - 1) + 13, SEEK_SET);// #1 posibil
    int offset = 0;
    int sizeSect = 0;
    if (read(fd, &offset, 4) == -1) {
        perror("ERROR\n");
        return -1;
    }
    if (read(fd, & sizeSect, 4) == -1) {
        perror("ERROR\n");
        return -1;
    }
    lseek(fd, offset, SEEK_SET);
    int lines = 1;
    int i = sizeSect-1; //#3 posibil
    char * c = (char * ) malloc(sizeSect);
    if (read(fd, c, sizeSect) == -1) {
        perror("ERROR\n");
        return -1;
    }
    printf("SUCCESS\n");
    while (lines <= numberLine && i >= 0) {//#3
        if (c[i] == '\n' && i>0) {
            if(c[i-1] == '\r')    // #2 posibil
            lines++;
        } else {
            if (lines == numberLine)
                printf("%c", c[i]);

        }
        i--;
    }
    free(c);
    close(fd);
    return 0;
}
//-------------------------------------------
void prelucrareComandaExtract(int argc, char ** argv) {
    char * path = NULL;
    getPath(argc, argv, & path);
    if (argc != 5) {
        perror("ERROR");
        free(path);
        return;
    }
    if (prelucrareComandaParse2(path)) {
        printf("ERROR\n");
        printf("invalid file\n");
        free(path);
        return;
    }
    int numbSect = getSectionNumber(argc, argv);
    if (numbSect < 1 || numbSect > nrSectiuni) {
        printf("ERROR\n");
        printf("invalid section\n");
        free(path);
        return;
    }
    int numbLine = getLineNumber(argc, argv);
    if (numbLine < 1) {
        printf("ERROR\n");
        printf("invalid line\n");
        free(path);
        return;
    }
    prelucrareComandaExtract2(path, numbSect, numbLine);
    free(path);
}
//------------------------------------------------
typedef struct memo {
    int offSet;
    int size;
}
memo;
//-------------------------------------------------
int countLines( int size, char array[]) {
    int nr = 0;
        for (int i = 0; i < size-1; i++) {
            if ((array[i]) == '\r' && array[i+1]=='\n')
                nr++;
            if (nr > 13)
                return nr;
        }
        return nr;
    
    return 0;
}
//.....................................................
void prelucrareComandaFindAll3(char * path) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("ERROR\n");
        return;
    }
    memo * structForSections = (memo * ) calloc(nrSectiuni, sizeof(memo));
    lseek(fd, 8, SEEK_SET);
    for (int i = 0; i < nrSectiuni; i++) {
        lseek(fd, 13, SEEK_CUR);
        if (read(fd, & ((structForSections + i)->offSet), sizeof(int)) == -1) {
            perror("ERROR\n");
            return;
        }
        if (read(fd, & ((structForSections + i)->size), sizeof(int)) == -1) {
            perror("ERROR\n");
            return;
        }
    }
    int numarLinii = 0;
    int numarSectiuniBune = 0;
   for(int i=0;i<nrSectiuni;i++)
   {   numarLinii=1;
       lseek(fd,(structForSections+i)->offSet,SEEK_SET);
       char* c=(char*)malloc((structForSections+i)->size);
       read(fd,c,(structForSections+i)->size);
       numarLinii+=countLines((structForSections+i)->size,c);
       if(numarLinii==13)
       numarSectiuniBune++;

       free(c);
   }
   if(numarSectiuniBune>=3)
   printf("%s\n",path);
    
    close(fd);
    free(structForSections);
    return;
}
//------------------------------------------------
int prelucrareComandaFindAll2(char * path) {
    DIR * dir = NULL;
    struct dirent * entry = NULL;
    struct stat statbuf;
    char newPath[1000];
    dir = opendir(path);
    if (dir == NULL)
        return 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            strcpy(newPath, path);
            strcat(newPath, "/");
            strcat(newPath, entry->d_name);
            if (lstat(newPath, & statbuf) == 0) {
                if (S_ISREG(statbuf.st_mode))
                    if (prelucrareComandaParse2(newPath) == 0)
                        prelucrareComandaFindAll3(newPath);
            }
            prelucrareComandaFindAll2(newPath);
        }
    }
    closedir(dir);
    return 0;
}
//------------------------------------------------
int pathCheck(char * path) {
    struct stat statbuf;
    if (lstat(path, & statbuf) == 0) {
        if (S_ISDIR(statbuf.st_mode) == 0) {
            return -1;
        }
    }
    if (strstr(path, "..") != NULL || strstr(path, ".") != NULL)
        return -1;
    printf("SUCCESS\n");
    return prelucrareComandaFindAll2(path);
}
//-------------------------------------------------
void prelucrareComandaFindAll(int argc, char ** argv) {
    char * path = NULL;
    getPath(argc, argv, & path);
    if (pathCheck(path) == -1) {
        free(path);
        printf("ERROR\n");
        printf("invalid directory path\n");
        return;
    }
    free(path);
    return;
}
//------------------------------------------------
int main(int argc, char ** argv) {
    if (argc >= 2) {
        if (strcmp(argv[1], "variant") == 0) {
            printf("98237\n");
        } else {
            if (strcmp(argv[1], "list") == 0)
                prelucrareComandaList(argc, argv);
            else {
                if (strcmp(argv[1], "parse") == 0)
                    prelucrareComandaParse(argc, argv);
                else {
                    if (strcmp(argv[1], "extract") == 0)
                        prelucrareComandaExtract(argc, argv);
                    else {
                        if (strcmp(argv[1], "findall") == 0)
                            prelucrareComandaFindAll(argc, argv);
                        else {
                            printf("ERROR\n");
                            printf("invalid command\n");
                            return -1;
                        }
                    }
                }

            }

        }
        return 0;
    }
}