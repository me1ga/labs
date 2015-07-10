#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strdup();
#include <getopt.h>
#include <dlfcn.h> /* заголовочный файл для работы с динамическими библиотеками */

#include "threadqsort.h" 

int main(int argc, char **argv){
    struct option loptions[]={
        {"in",      required_argument,  NULL,'i'},
        {"out",     required_argument,  NULL,'o'},
        {"threads", required_argument,  NULL,'t'},
        {"help",    no_argument,        NULL,'h'},
        {0,0,0,0}
    };
    int errflag=0,opt,helpflag=0;
    char *infile = NULL;
    char *outfile = NULL;
    int threadnum = 1;
    
    void *ext_library;

    unsigned int (*countFiles)(FILE *file);
    void (*readFiles)(FILE *file, char **strMass, unsigned int nos);
    int (*sortFiles)(char **strMass, unsigned int nThreads, unsigned int nos);
    void (*writeFiles)(FILE *file, char **strMass, unsigned int nos);

    //Зарузка библиотеки
    ext_library = dlopen("libmylabsort.so", RTLD_LAZY);
    if (!ext_library){
        fprintf(stderr, "dlopen() error: %s\n", dlerror());
        return 1;
    }

    while((opt = getopt_long(argc, argv, "i:o:t:", loptions, NULL)) != -1){
        switch(opt){
            case 'i': infile = optarg;          break;
            case 'o': outfile = optarg;         break;
            case 't': threadnum = atoi(optarg); break;
            case 'h': helpflag = 1;             break;
            case '0':                           break;
            case ':': errflag = 1;              break;
            case '?': errflag = 1;              break;
        }
    }
    
    if (infile == NULL) {
        errflag = 1;
        printf("Отсутствует путь входного файла!\n");
    }
    if (outfile == NULL) {
        errflag = 1;
        printf("Отсутствует путь выходного файла!\n");
    }
    if (errflag == 1 || helpflag == 1) {
        usage();
        return errflag;
    }
    if( threadnum<1 || threadnum>10 ){
        errflag = 1;
        usage();
        return errflag;
    }
    
    FILE *input,*output;
    unsigned int numberOfStrings=0;
    
    input=fopen(infile, "r");
    if(input == NULL || (output = fopen(outfile, "w")) == NULL) {
        perror("fopen");
        return -1;
    }
    
    countFiles = dlsym(ext_library, "countFiles");//загружаем из библиотеки процедуру countFiles
    numberOfStrings = (*countFiles)(input);
    
    if( numberOfStrings == 0 ) return -1;
    char **strings = (char**)malloc(sizeof(char*)*numberOfStrings);
    
    readFiles = dlsym(ext_library, "readFiles");//загружаем из библиотеки процедуру readFiles
    (*readFiles)(input, strings, numberOfStrings);

    sortFiles = dlsym(ext_library, "sortFiles");//загружаем из библиотеки процедуру sortFiles
    (*sortFiles)(strings, threadnum, numberOfStrings);

    writeFiles = dlsym(ext_library, "writeFiles");//загружаем из библиотеки процедуру writeFiles
    (*writeFiles)(output, strings, numberOfStrings);
    
    free(strings);
    fclose(input);
    fclose(output);
    dlclose(ext_library);
    
    //printf("%u\n", clock()*1000/CLOCKS_PER_SEC);
    return 0;
}

void usage(){
    printf("usage: lab1 \t[-i file | --in file] \n\t\t[-o file | --out file] \n\t\t[-t nthreads | --threads nthreads][default : 1] \n\t\t[--help]\n");
}